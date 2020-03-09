#include <AsgTools/MessageCheck.h>
#include <LowPtAnalysis/TruthAnalysis.h>

#include <xAODEventInfo/EventInfo.h>

#include <xAODTruth/TruthParticleContainer.h>
#include <xAODTruth/xAODTruthHelpers.h>
#include <xAODTracking/TrackParticleContainer.h>



//ROOT includes
#include "TFile.h"

TruthAnalysis :: TruthAnalysis (const std::string& name,
                                  ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm (name, pSvcLocator)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  This is also where you
  // declare all properties for your algorithm.  Note that things like
  // resetting statistics variables or booking histograms should
  // rather go into the initialize() function.

  // Properties
  declareProperty("lep1_min_pt", lep1_min_pt = 25*GeV);
  declareProperty("lep2_min_pt", lep2_min_pt = 20*GeV);
  declareProperty("lep_max_eta", lep_max_eta = 2.5);
  declareProperty("dilep_min_pt", dilep_min_pt = 30.0*GeV);
  declareProperty("dilep_min_mass", dilep_min_mass = 20.0*GeV);
  declareProperty("tracks_min_pt", tracks_min_pt = 400*MeV,
		  "minimum reconstructible pT");
  declareProperty("tracks_max_eta", tracks_max_eta = 2.5,
		  "maximum reconstructible eta");
  declareProperty("tracks_max_n", tracks_max_n = 0,
		  "exclusivity selection");
  declareProperty("input_trk_eff_file", input_trk_eff_file = "",
		  "tracking efficiency input. if empty, use hard-coded numbers");
  declareProperty("filter_by_selections", filter_by_selections = false,
		  "if true, only events passing selections are stored in the output");
  declareProperty("random_seed", random_seed=29873,
		  "Random seed for tracking efficiency");

}



StatusCode TruthAnalysis :: initialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  ANA_MSG_INFO ("Initializing");

  // Random generator
  m_rnd = new TRandom3(0);

  // Output tree/histograms
  ANA_CHECK (book (TTree ("analysis", "Truth analysis ntuple")));
  TTree* mytree = tree ("analysis");

  //Event-level
  mytree->Branch ("dilep_pt", &m_dilep_pt);
  mytree->Branch ("dilep_m", &m_dilep_m);
  m_pass_sel = new std::vector<char>(ncuts);
  mytree->Branch("pass_sel", m_pass_sel);

  //Leptons
  m_lep_pt = new std::vector<float>();
  mytree->Branch ("lep_pt", &m_lep_pt);
  m_lep_eta = new std::vector<float>();
  mytree->Branch ("lep_eta", &m_lep_eta);
  m_lep_phi = new std::vector<float>();
  mytree->Branch ("lep_phi", &m_lep_phi);
  m_lep_charge = new std::vector<int>();
  mytree->Branch ("lep_charge", &m_lep_charge);
  m_lep_pdgid = new std::vector<int>();
  mytree->Branch ("lep_pdgid", &m_lep_pdgid);

  //Tracks
  m_trk_pt = new std::vector<float>();
  mytree->Branch ("trk_pt", &m_trk_pt);
  m_trk_eta = new std::vector<float>();
  mytree->Branch ("trk_eta", &m_trk_eta);
  m_trk_phi = new std::vector<float>();
  mytree->Branch ("trk_phi", &m_trk_phi);
  m_trk_charge = new std::vector<int>();
  mytree->Branch ("trk_charge", &m_trk_charge);
  m_trk_pdgid = new std::vector<int>();
  mytree->Branch ("trk_pdgid", &m_trk_pdgid);
  m_weights = new std::vector<float>();
  mytree->Branch("weights", &m_weights);
  
  ANA_CHECK (book ( TH1F ("cutflow", "Cutflow", ncuts, -0.5, ncuts-0.5) ));
  ANA_CHECK (book ( TH1F ("num_fiducial_leptons", "Number of fiducial leptons", 10, 0, 10) ));
  ANA_CHECK (book ( TH1F ("dilep_m", "m(ll) (GeV)", 60, 0, 300.) ));
  ANA_CHECK (book ( TH1F ("dilep_pt", "p_{T} (ll) (GeV)", 60, 0, 300.) ));
  ANA_CHECK (book ( TH1F ("num_fiducial_tracks", "Number of fiducial tracks", 50, 0, 50) ));
  ANA_CHECK(book (TH1F ("delta_phi", "del_phi",60, 0, 3.14) ));
  ANA_CHECK (book ( TH1F ("sr_dilep_pt", "p_{T} (ll) after all selections (GeV);p_{T}(ll) [GeV];Events/5 GeV", 60, 0, 300.) ));
  ANA_CHECK (book ( TH1F ("track_weights","Track Weights", 60, 0, 1.) ));
  ANA_CHECK (book ( TH1F ("sr_dilep_pt_weights", "p_{T} (ll) after all selections (GeV);p_{T}(ll) [GeV];Events/5 GeV", 60, 0, 300.) ));
  
  ANA_CHECK (book ( TH1F ("num_HSpart_pt", "Num. non-lepton particles from Hard Scatter;p_{T}(particle) [MeV];Tracks/50 MeV", 60, 0, 3000.) ));

  //set bin labels for cutflow
  for (int i=1; i<=ncuts;i++) {
    hist("cutflow")->GetXaxis()->SetBinLabel(i, cuts_labels[i-1].c_str());
  }

  //retrieve tracking efficiency, if needed
  h_trk_eff_pt = nullptr;
  if (not input_trk_eff_file.empty()) {
    TFile *f_trk_eff = TFile::Open(input_trk_eff_file.c_str());
    h_trk_eff_pt = static_cast<TH1F*>(f_trk_eff->Get("h_trk_eff_pt"));
    if (h_trk_eff_pt == nullptr) {
      ANA_MSG_ERROR("Error loading tracking efficiency (h_trk_eff_pt) from:" << input_trk_eff_file);
      return StatusCode::FAILURE;
    }
    ANA_MSG_INFO("Loaded tracking efficiency from " << input_trk_eff_file);
  }

  //print properties values
  ANA_MSG_INFO("Properties values:");
  ANA_MSG_INFO("Random Number = " << m_rnd->Rndm() );
  ANA_MSG_INFO("lep1_min_pt = " << lep1_min_pt);
  ANA_MSG_INFO("lep2_min_pt = " << lep2_min_pt);
  ANA_MSG_INFO("lep_max_eta = " << lep_max_eta);
  ANA_MSG_INFO("dilep_min_pt = " << dilep_min_pt);
  ANA_MSG_INFO("tracks_min_pt = " << tracks_min_pt);
  ANA_MSG_INFO("tracks_max_eta = " << tracks_max_eta);
  ANA_MSG_INFO("filter_by_selections = " << filter_by_selections);
  ANA_MSG_INFO("input_trk_eff_file = " << input_trk_eff_file);

  return StatusCode::SUCCESS;
}

StatusCode TruthAnalysis :: execute ()
{
  //reset variables
  m_dilep_pt = -1.0;
  m_dilep_m = -1.0;
  m_lep_pt->clear();
  m_lep_eta->clear();
  m_lep_phi->clear();
  m_lep_charge->clear();
  m_lep_pdgid->clear();
  m_trk_pt->clear();
  m_trk_eta->clear();
  m_trk_phi->clear();
  m_trk_charge->clear();
  m_trk_pdgid->clear();
  m_weights->clear();
  
  for (int i=0; i<ncuts;i++)
    m_pass_sel->at(i)=false;

  // get truth particle container of interest
  const xAOD::TruthParticleContainer* truthParts = 0;
  ANA_CHECK (evtStore()->retrieve( truthParts, "TruthParticles"));
  ANA_MSG_DEBUG ("execute(): number of truth particles = " << truthParts->size());

  passCut(cut_nocut);

  // trigger, assumed 100% efficiency after other selections
  passCut(cut_trigger);

  // loop over the particles in the container
  // and select fiducial leptons and tracks
  for (const xAOD::TruthParticle *part : *truthParts) {    
    //select fiducial truth particles
    ANA_MSG_VERBOSE ("Particle PDG " << part->auxdata<int>("pdgId") << ", pT=" << part->pt() << ", eta=" << part->eta() << ", charge=" << part->charge() << ", status=" << part->auxdata<int>("status") << ", barcode=" << part->auxdata<int>("barcode"));
    //if (part->pt() < std::min(tracks_min_pt,lep2_min_pt)) continue;
    if (part->abseta() > std::max(tracks_max_eta,lep_max_eta)) continue;
    if (part->charge() == 0) continue;
    if (part->auxdata<int>("status") != 1) continue;
    if (part->auxdata<int>("barcode") > 200000) continue;
    ANA_MSG_VERBOSE("Pass pre-fiducial cuts");

    int pdgid = part->auxdata<int>("pdgId");
    if ( (abs(pdgid) == 11) or (abs(pdgid)==13) ) {
      if  ( (part->pt() > lep2_min_pt) and (part->abseta() < lep_max_eta) )
	{      
	ANA_MSG_VERBOSE ("Store as lepton");
	m_lep_pt->push_back(part->pt());
	m_lep_eta->push_back(part->eta());
	m_lep_phi->push_back(part->phi());
	m_lep_charge->push_back(static_cast<int>(part->charge()));
	m_lep_pdgid->push_back(pdgid);
	continue; //do not consider it as track
      }
    }

    hist("num_HSpart_pt")->Fill(part->pt());

    if (part->pt() < tracks_min_pt) continue;
    if (part->abseta() > tracks_max_eta) continue;
    ANA_MSG_VERBOSE("Candidate track");
    
    //apply parametrized tracking efficiency, if requested
    if (h_trk_eff_pt != nullptr) {
      int ibin = h_trk_eff_pt->FindBin(part->pt());
     float trk_eff = 0.0; //default if underflow
      if (ibin > 0) {
	//use last bin if overflow
	if (ibin > h_trk_eff_pt->GetNbinsX()) ibin = h_trk_eff_pt->GetNbinsX();
	trk_eff = h_trk_eff_pt->GetBinContent(ibin)-0.05;      
      }  
    
    /* if (h_trk_eff_pt != nullptr) {
      //int ibin = h_trk_eff_pt_eta->FindBin(part->pt(), part->eta());
      int xbin = h_trk_eff_pt->GetXaxis()->FindBin(part->pt());
      int ybin = h_trk_eff_pt->GetYaxis()->FindBin(part->eta());
      float trk_eff = 0.0; //default if underflow
      if ( xbin > 0 && ybin > 0 ) {
	//use last bin if overflow
	if (xbin > h_trk_eff_pt->GetNbinsX()) xbin = h_trk_eff_pt->GetNbinsX();
	if (ybin > h_trk_eff_pt->GetNbinsY()) ybin = h_trk_eff_pt->GetNbinsY();
	  trk_eff = h_trk_eff_pt->GetBinContent(xbin, ybin)+h_trk_eff_pt->GetBinError(xbin,ybin);
	}
    */

      //increment a per-event "weight" with 1-trk_eff  
      m_weights->push_back( 1- trk_eff );

      if (m_rnd->Rndm() > trk_eff)
	continue;
    }

    //store basic kinematic and particle info
    ANA_MSG_VERBOSE("Store track");
    m_trk_pt->push_back(part->pt());
    m_trk_eta->push_back(part->eta());
    m_trk_phi->push_back(part->phi());
    m_trk_charge->push_back(static_cast<int>(part->charge()));
    m_trk_pdgid->push_back(pdgid);
    
  } // end for loop over truth particles
  
  //The "tracking weight" is done
  float  tracking_weight = 1;
  for(int i = 0; (i < abs(m_weights->size())); i++)
    {
      tracking_weight *= m_weights->at(i);
    }

  // Now evaluate event-level selections  
  ANA_MSG_VERBOSE("Checking event selections");
  ANA_MSG_DEBUG("NLep = " << m_lep_pt->size() << ", NTracks = " << m_trk_pt->size());
  hist("num_fiducial_leptons")->Fill(m_lep_pt->size());
  if (m_lep_pt->size() != 2) {saveTree(); return StatusCode::SUCCESS;}
  if (m_lep_charge->at(0)*m_lep_charge->at(1) != -1) {saveTree(); return StatusCode::SUCCESS;}
  //if ( ( abs(m_lep_pdgid->at(0)*m_lep_pdgid->at(1)) != 11*13 ) && ( abs(m_lep_pdgid->at(0)*m_lep_pdgid->at(1) ) !=  11*11 ) && ( abs(m_lep_pdgid->at(0)*m_lep_pdgid->at(1) ) !=  13*13 ) ) {saveTree(); return StatusCode::SUCCESS;} //For all e and mu selections 
  //if (abs(m_lep_pdgid->at(0)*m_lep_pdgid->at(1)) != 11*13){ saveTree(); return StatusCode::SUCCESS;} //For just emu
  if ( abs(m_lep_pdgid->at(0)*m_lep_pdgid->at(1) ) !=  13*13 )  {saveTree(); return StatusCode::SUCCESS;}// mumu
  
  passCut(cut_lep_ocof);
  ANA_MSG_VERBOSE("Pass cut_lep_ocof");

  if (std::min(m_lep_pt->at(0), m_lep_pt->at(1)) < lep2_min_pt) {saveTree(); return StatusCode::SUCCESS;}
  if (std::max(m_lep_pt->at(0), m_lep_pt->at(1)) < lep1_min_pt) {saveTree(); return StatusCode::SUCCESS;}
  passCut(cut_lep_minpt);
  ANA_MSG_VERBOSE("Pass cut_lep_minpt");

  TLorentzVector lep1, lep2;
  int index_lep1=0;
  int index_lep2=1;
  float m_delta_phi;
  if (m_lep_pt->at(1) > m_lep_pt->at(0)) {
    index_lep1=1;
    index_lep2=0;
  }
  lep1.SetPtEtaPhiM(m_lep_pt->at(index_lep1), m_lep_eta->at(index_lep1), m_lep_phi->at(index_lep1), 0.0);
  lep2.SetPtEtaPhiM(m_lep_pt->at(index_lep2), m_lep_eta->at(index_lep2), m_lep_phi->at(index_lep2), 0.0);
  m_dilep_m = (lep1+lep2).M();
  m_dilep_pt = (lep1+lep2).Pt();
  m_delta_phi = abs((lep1).Phi()-(lep2).Phi());
  if (m_delta_phi > 3.1415 ) { m_delta_phi = 6.183 - m_delta_phi;}

  hist("dilep_m")->Fill(m_dilep_m/GeV);
  if ( (m_dilep_m < dilep_min_mass) || ((m_dilep_m> 70*GeV ) && ( m_dilep_m < 110*GeV ) ) ) {saveTree(); return StatusCode::SUCCESS;}
  passCut(cut_m_ll);
  ANA_MSG_VERBOSE("Pass cut_m_ll");


  hist("dilep_pt")->Fill(m_dilep_pt/GeV);
  if (m_dilep_pt < dilep_min_pt) {saveTree(); return StatusCode::SUCCESS;}
  passCut(cut_pt_ll);
  ANA_MSG_VERBOSE("Pass cut_pt_ll");

  hist("sr_dilep_pt_weights")->Fill(m_dilep_pt/GeV, tracking_weight);
  hist("track_weights")->Fill(tracking_weight);
  hist("num_fiducial_tracks")->Fill(m_trk_pt->size());
  if (m_trk_pt->size() > tracks_max_n) {saveTree(); return StatusCode::SUCCESS;}
  passCut(cut_exclusive);
  ANA_MSG_VERBOSE("Pass cut_exclusive");

  //Plots after all selections
  hist("sr_dilep_pt")->Fill(m_dilep_pt/GeV);
  hist("delta_phi")->Fill(m_delta_phi);
  
  

  // Fill the event into the tree
  saveTree();
  return StatusCode::SUCCESS;
}


StatusCode TruthAnalysis :: finalize ()
{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.
  return StatusCode::SUCCESS;
}


TruthAnalysis :: ~TruthAnalysis () {
  if (m_pass_sel) {
    delete m_lep_pt;
    delete m_lep_eta;
    delete m_lep_phi;
    delete m_lep_charge;
    delete m_lep_pdgid;
    delete m_weights;
    
    delete m_trk_pt;
    delete m_trk_eta;
    delete m_trk_phi;
    delete m_trk_charge;
    delete m_trk_pdgid;
    
    delete m_pass_sel;
  }
}

void TruthAnalysis :: saveTree ()
{
  //save if requested or pass all selections
  if ((not filter_by_selections) or (m_pass_sel->at(ncuts-1)))
    tree ("analysis")->Fill ();
}

void TruthAnalysis :: passCut (int cut)
{
  hist("cutflow")->Fill(cut);
  m_pass_sel->at(cut)=true;
}
