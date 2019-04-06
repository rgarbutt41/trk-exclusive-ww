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
}



StatusCode TruthAnalysis :: initialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  ANA_MSG_INFO ("Initializing");

  // Properties
  declareProperty("lep1_min_pt", lep1_min_pt = 25*GeV);
  declareProperty("lep2_min_pt", lep2_min_pt = 20*GeV);
  declareProperty("lep_max_eta", lep_max_eta = 2.5);
  declareProperty("dilep_min_pt", dilep_min_pt = 30.0*GeV);
  declareProperty("dilep_min_mass", dilep_min_mass = 20.0*GeV);
  declareProperty("tracks_min_pt", tracks_min_pt = 400*MeV); //minimum reconstructible pT
  declareProperty("tracks_max_eta", tracks_max_eta = 2.5); //maximum reconstructible eta
  declareProperty("tracks_max_n", tracks_max_n = 0); //exclusivity selection
  declareProperty("input_trk_eff_file", input_trk_eff_file = ""); //if empty, use hard-coded numbers
  declareProperty("filter_by_selections", filter_by_selections = false); //if true, only events passing selections are stored in the output

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
  
  ANA_CHECK (book ( TH1F ("cutflow", "Cutflow", ncuts, -0.5, ncuts-0.5) ));
  ANA_CHECK (book ( TH1F ("num_fiducial_leptons", "Number of fiducial leptons", 10, 0, 10) ));
  ANA_CHECK (book ( TH1F ("dilep_m", "m(e#mu) (GeV)", 60, 0, 300.) ));
  ANA_CHECK (book ( TH1F ("dilep_pt", "p_{T} (e#mu) (GeV)", 60, 0, 300.) ));
  ANA_CHECK (book ( TH1F ("num_fiducial_tracks", "Number of fiducial tracks", 50, 0, 50) ));
  ANA_CHECK (book ( TH1F ("sr_dilep_pt", "p_{T} (e#mu) after all selections (GeV);p_{T}(e#mu) [GeV];Events/5 GeV", 60, 0, 300.) ));

  h_trk_eff_pt = nullptr;

  return StatusCode::SUCCESS;
}

StatusCode TruthAnalysis :: execute ()
{
  //retrieve tracking efficiency, if needed
  if (h_trk_eff_pt == nullptr) {
    if (not input_trk_eff_file.empty()) {
      TFile *f_trk_eff = TFile::Open(input_trk_eff_file.c_str());
      h_trk_eff_pt = dynamic_cast<TH1F*>(f_trk_eff->Get("h_trk_eff_pt"));
      if (h_trk_eff_pt == nullptr) {
	ANA_MSG_ERROR("Error loading tracking efficiency (h_trk_eff_pt) from:" << input_trk_eff_file);
	return StatusCode::FAILURE;
      }
    }
  }

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
    if (part->pt() < std::min(tracks_min_pt,lep2_min_pt)) continue;
    if (part->abseta() > std::max(tracks_max_eta,lep_max_eta)) continue;
    if (part->charge() == 0) continue;
    if (part->auxdata<int>("status") != 1) continue;
    if (part->auxdata<int>("barcode") > 200000) continue;
    ANA_MSG_VERBOSE("Pass pre-fiducial cuts");

    int pdgid = part->auxdata<int>("pdgId");
    if ( (abs(pdgid) == 11) or (abs(pdgid)==13) ) {
      if ((part->pt() > lep2_min_pt) and
	  (part->abseta() < lep_max_eta) ) {      
	ANA_MSG_VERBOSE ("Store as lepton");
	m_lep_pt->push_back(part->pt());
	m_lep_eta->push_back(part->eta());
	m_lep_phi->push_back(part->phi());
	m_lep_charge->push_back(static_cast<int>(part->charge()));
	m_lep_pdgid->push_back(pdgid);
	continue; //do not consider it as track
      }
    }

    if (part->pt() < tracks_min_pt) continue;
    if (part->abseta() > tracks_max_eta) continue;
    ANA_MSG_VERBOSE("Store as track");
    
    //apply parametrized tracking efficiency, if requested
    //TODO..

    //store basic kinematic and particle info
    m_trk_pt->push_back(part->pt());
    m_trk_eta->push_back(part->eta());
    m_trk_phi->push_back(part->phi());
    m_trk_charge->push_back(static_cast<int>(part->charge()));
    m_trk_pdgid->push_back(pdgid);
    
  } // end for loop over truth particles

  // Now evaluate event-level selections  
  ANA_MSG_VERBOSE("Checking event selections");
  ANA_MSG_DEBUG("NLep = " << m_lep_pt->size() << ", NTracks = " << m_trk_pt->size());
  hist("num_fiducial_leptons")->Fill(m_lep_pt->size());
  if (m_lep_pt->size() != 2) {saveTree(); return StatusCode::SUCCESS;}
  if (m_lep_charge->at(0)*m_lep_charge->at(1) != -1) {saveTree(); return StatusCode::SUCCESS;}
  if (abs(m_lep_pdgid->at(0)*m_lep_pdgid->at(1)) != 11*13) {saveTree(); return StatusCode::SUCCESS;}
  passCut(cut_lep_ocof);
  ANA_MSG_VERBOSE("Pass cut_lep_ocof");

  if (std::min(m_lep_pt->at(0), m_lep_pt->at(1)) < lep2_min_pt) {saveTree(); return StatusCode::SUCCESS;}
  if (std::max(m_lep_pt->at(0), m_lep_pt->at(1)) < lep1_min_pt) {saveTree(); return StatusCode::SUCCESS;}
  passCut(cut_lep_minpt);
  ANA_MSG_VERBOSE("Pass cut_lep_minpt");

  TLorentzVector lep1, lep2;
  int index_lep1=0;
  int index_lep2=1;
  if (m_lep_pt->at(1) > m_lep_pt->at(0)) {
    index_lep1=1;
    index_lep2=0;
  }
  lep1.SetPtEtaPhiM(m_lep_pt->at(index_lep1), m_lep_eta->at(index_lep1), m_lep_phi->at(index_lep1), 0.0);
  lep2.SetPtEtaPhiM(m_lep_pt->at(index_lep2), m_lep_eta->at(index_lep2), m_lep_phi->at(index_lep2), 0.0);
  m_dilep_m = (lep1+lep2).M();
  m_dilep_pt = (lep1+lep2).Pt();

  hist("dilep_m")->Fill(m_dilep_m);
  if (m_dilep_m < dilep_min_mass) {saveTree(); return StatusCode::SUCCESS;}
  passCut(cut_m_ll);
  ANA_MSG_VERBOSE("Pass cut_m_ll");


  hist("dilep_pt")->Fill(m_dilep_pt);
  if (m_dilep_pt < dilep_min_pt) {saveTree(); return StatusCode::SUCCESS;}
  passCut(cut_pt_ll);
  ANA_MSG_VERBOSE("Pass cut_pt_ll");

  hist("num_fiducial_tracks")->Fill(m_trk_pt->size());
  if (m_trk_pt->size() > tracks_max_n) {saveTree(); return StatusCode::SUCCESS;}
  passCut(cut_exclusive);
  ANA_MSG_VERBOSE("Pass cut_exclusive");

  //Plots after all selections
  hist("sr_dilep_pt")->Fill(m_dilep_pt);

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
