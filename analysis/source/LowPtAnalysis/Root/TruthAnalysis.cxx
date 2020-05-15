#include <AsgTools/MessageCheck.h>
#include <LowPtAnalysis/TruthAnalysis.h>

#include <xAODEventInfo/EventInfo.h>

#include <xAODTruth/TruthParticleContainer.h>
#include <xAODTruth/xAODTruthHelpers.h>
#include <xAODTracking/TrackParticleContainer.h>

#include <cmath>
#include <string>
#include<stdio.h>
#include<math.h>
#include<iostream>
#include<fstream>

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
  declareProperty("lep1_min_pt", lep1_min_pt = 27*GeV);
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
  declareProperty("input_lepton_reco_eff_file", input_lepton_reco_eff_file = "",
		  "e/mu tracking efficiency input. if empty, use hard-coded numbers");
  declareProperty("filter_by_selections", filter_by_selections = false,
		  "if true, only events passing selections are stored in the output");
  declareProperty("random_seed", random_seed=29873,
		  "Random seed for tracking efficiency");
  declareProperty("input_pu_file", input_pu_file = "", 
		  "PU info input, if empty, use hard-coded numbers. Also includes Fakes info");

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
  mytree->Branch("numPUtracks", &m_numPUtracks);
  mytree->Branch("mcWeight" , m_mcWeight);

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
  muon_eff = 0.0;
  electron_eff = 0.0;

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
    ANA_CHECK (book ( TH1F ("weighted_number_of_events", "Weighted number of events that pass selections prior to 0 trk requirement.", 60, 0, 300.) ));
  ANA_CHECK (book ( TH1D ("hCutFlow_Sum", "Cut Flow Sum", 3, 0, 3.) ));

  ANA_CHECK (book ( TH1F ("num_HSpart_pt", "Num. non-lepton particles from Hard Scatter;p_{T}(particle) [MeV];Tracks/50 MeV", 60, 0, 3000.) ));

   ANA_CHECK (book ( TH1D ("pass_2leps", "passes 2leps", 60, 0, 3000.) ));
   ANA_CHECK (book ( TH1D ("pass_2leps_OS", "passes 2leps", 60, 0, 3000.) ));
   ANA_CHECK (book ( TH1D ("pass_2leps_OS_OF", "passes 2leps", 60, 0, 3000.) ));
   ANA_CHECK (book ( TH1D ("pass_2leps_OS_OF_sublep", "passes 2leps", 60, 0, 3000.) ));
   ANA_CHECK (book ( TH1D ("pass_2leps_OS_OF_sublep_leadlep", "passes 2leps", 60, 0, 3000.) ));
   ANA_CHECK (book ( TH1D ("pass_2leps_OS_OF_sublep_leadlep_mll", "passes 2leps", 60, 0, 3000.) ));
   ANA_CHECK (book ( TH1D ("pass_2leps_OS_OF_sublep_leadlep_mll_ptll", "passes 2leps", 60, 0, 3000.) ));
   ANA_CHECK (book ( TH1D ("pass_2leps_OS_OF_sublep_leadlep_mll_ptll_0trk", "passes 2leps", 60, 0, 3000.) ));
   ANA_CHECK (book ( TH1D ("pass_2leps_OS_OF_sublep_leadlep_mll_ptll_0trk_withPU", "passes 2leps", 60, 0, 3000.) ));
  ANA_CHECK (book ( TH1D ("electron_eff","Electron efficiency", 60, 0, 1.) ));
  ANA_CHECK (book ( TH1D ("pileup_weights","Pileup weighting", 60, 0, 1.) ));
  ANA_CHECK (book ( TH1D ("total_weighting","Cumulative weighting of all efficiencies plus the ncr reweighting", 60, 0, 1.) ));
  ANA_CHECK (book ( TH1D ("muon_eff","Muon efficiency", 60, 0, 1.) ));
  ANA_CHECK (book ( TH1F ("mcWeight","mcWeight per event",60,0,1.) ));
   //set bin labels for cutflow
  for (int i=1; i<=ncuts;i++) {
    hist("cutflow")->GetXaxis()->SetBinLabel(i, cuts_labels[i-1].c_str());
  }
  
  //retrieve tracking efficiency, if needed
  h_trk_eff_pt_eta = nullptr; //2D
  // h_trk_eff_pt = nullptr;
  h_electron_eff = nullptr;
  h_muon_eff = nullptr;
  if ( not input_lepton_reco_eff_file.empty()) {
    TFile *f_trk_eff = TFile::Open(input_trk_eff_file.c_str());
    TFile *f_lepton_reco_eff = TFile::Open(input_lepton_reco_eff_file.c_str());
    h_trk_eff_pt_eta = static_cast<TProfile2D*>(f_trk_eff ->Get("Tracking_Eff_2D")); //2D
    //h_trk_eff_pt = static_cast<TH1F*>(f_trk_eff->Get("h_trk_eff_pt"));
    h_electron_eff= static_cast<TProfile2D*>(f_lepton_reco_eff->Get("Electron_Eff_2D"));
    h_muon_eff= static_cast<TProfile2D*>(f_lepton_reco_eff->Get("Muon_Eff_2D"));

    if (h_trk_eff_pt_eta == nullptr) {
      ANA_MSG_ERROR("Error loading tracking efficiency (h_trk_eff_pt_eta) from:" << input_trk_eff_file);
      return StatusCode::FAILURE;
    }
    if (h_electron_eff == nullptr) {
      ANA_MSG_ERROR("Error loading electron efficiency from:" << input_lepton_reco_eff_file);
      return StatusCode::FAILURE;
    }
    if (h_muon_eff == nullptr) {
      ANA_MSG_ERROR("Error loading muon efficiency from:" << input_lepton_reco_eff_file);
      return StatusCode::FAILURE;
    }
    ANA_MSG_INFO("Loaded tracking efficiency from " << input_trk_eff_file);
  }
  

  //retricve pu eff
  h_pu_info=nullptr;
  if (not input_pu_file.empty()) {
    TFile *f_pu = TFile::Open(input_pu_file.c_str());
    std::string PU = "PU_dist_min";
    std::string PU_hist;
    PU_hist = PU + std::to_string((int)tracks_min_pt);//+"_half";
    h_pu_info = static_cast<TH1D*>(f_pu->Get(PU_hist.c_str()));
    if(h_pu_info == nullptr) {
      ANA_MSG_ERROR("Error loading pu info from:" <<input_pu_file);
      ANA_MSG_ERROR("PU Hist Name:" << PU_hist);
      return StatusCode::FAILURE;
    }
    ANA_MSG_INFO("Loaded pu info from " << input_pu_file);
  }
  //retricve Fakes eff
  
  h_fakes_info=nullptr;
  if (not input_pu_file.empty()) {
    TFile *f_fakes = TFile::Open(input_pu_file.c_str());
    std::string Fakes = "Fakes_dist_min";
    std::string Fakes_hist;
    Fakes_hist = Fakes + std::to_string((int)tracks_min_pt);//+"_half";
    h_fakes_info = static_cast<TH1D*>(f_fakes->Get(Fakes_hist.c_str()));
    if(h_fakes_info == nullptr) {
      ANA_MSG_ERROR("Error loading pu info from:" <<input_pu_file);
      ANA_MSG_ERROR("Fakes Hist Name:" << Fakes_hist);
      return StatusCode::FAILURE;
    }
    ANA_MSG_INFO("Loaded Fakes info from " << input_pu_file);
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
  ANA_MSG_INFO("input_pu_file = " << input_pu_file);

  return StatusCode::SUCCESS;
}

StatusCode TruthAnalysis :: execute ()
{
  //reset variables
  m_dilep_pt = -1.0;
  m_dilep_m = -1.0;
  m_numPUtracks = 0;
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
  m_mcWeight = -1;

  /*
  m_truedilep_pt = -1.;
  m_truedilep_m = -1.;
  //m_numTrueHSparticles = -1;
  m_truelep_pt->clear();
  m_truelep_eta->clear();
  m_truelep_phi->clear();
  m_truelep_charge->clear();
  m_truelep_pdgid->clear();
  m_truetrk_pt->clear();
  m_truetrk_eta->clear();
  m_truetrk_phi->clear();
  m_truetrk_charge->clear();
  m_truetrk_pdgid->clear();
  m_true_twoleps = false;
  m_true_twoOSleps = false;
  m_true_absproduct_of_lepton_pdgidvals = 0;
  m_true_leadlep_ptpass = false;
  m_true_subleadlep_ptpass = false;
  m_true_dilep_masspass = false;
  m_true_dilep_ptpass = false;
  m_true_numCh_and_PU = -1;
  */
  for (int i=0; i<ncuts;i++)
    m_pass_sel->at(i)=false;
  std::ofstream myfile;
  myfile.open("test_output.txt");
  //get event info
  const xAOD::EventInfo* eventInfo;
  ANA_CHECK(evtStore()->retrieve(eventInfo,"EventInfo"));
  double mcWeight = 1;
  const std::vector< float > weights = eventInfo->mcEventWeights();
  if( weights.size() > 0 ) mcWeight = weights[0];

  hist("hCutFlow_Sum")->Fill(0.5, 1);
  hist("hCutFlow_Sum")->Fill(1.5,mcWeight);
  hist("hCutFlow_Sum")->Fill(2.5,mcWeight*mcWeight);

  m_mcWeight = mcWeight;

  //How many PU tracks in window? Compare rand against the integral of the PDF of the track multiplicity

  if(h_pu_info != nullptr) {
    double rand = m_rnd->Rndm();
    m_rand = rand;
    //double integral = 0;
    Pileup_eff = h_pu_info->GetBinContent(1);
    /* for(int b = 1; b < h_pu_info -> GetNbinsX(); b++){
      integral += h_pu_info->GetBinContent(b);
      if(integral > rand){
	m_numPUtracks = b-1;
	break;
	}
	}*/
  }

  //How many Fakes in window?
  if(h_fakes_info != nullptr) {
  Fakes_eff = h_fakes_info->GetBinContent(1);
  }

  // get truth particle container of interest
  const xAOD::TruthParticleContainer* truthParts = 0;
  ANA_CHECK (evtStore()->retrieve( truthParts, "TruthParticles"));
  ANA_MSG_DEBUG ("execute(): number of truth particles = " << truthParts->size());

  passCut(cut_nocut);

  // trigger, assumed 100% efficiency after other selections
  passCut(cut_trigger);

  int tmpsize = 0;
  int tmpsizetruth = 0;

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

    //Pre-Lepton reco efficiency
    /* int pdgid = part->auxdata<int>("pdgId");
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
    */
int pdgid = part->auxdata<int>("pdgId");
    if ( (abs(pdgid) == 11) or (abs(pdgid)==13) ) {
      if ((part->pt() > lep2_min_pt) and
	  (part->abseta() < lep_max_eta) ) {      
	/*
	m_truelep_pt->push_back(part->pt());
	m_truelep_eta->push_back(part->eta());
	m_truelep_phi->push_back(part->phi());
	m_truelep_charge->push_back(static_cast<int>(part->charge()));
	m_truelep_pdgid->push_back(pdgid);
	*/
	if ( h_electron_eff != nullptr && (abs(pdgid) == 11) ) {
	  ANA_MSG_VERBOSE ("Candidate electron; is it reconstructed?");
	  int xbin = h_electron_eff->GetXaxis()->FindBin(part->pt());
	  int ybin = h_electron_eff->GetYaxis()->FindBin(part->eta());
	  //float trk_eff = 0.0; //default if underflow
	  if ( xbin > 0 && ybin > 0 ) {
	    //use last bin if overflow
	    if (xbin > h_electron_eff->GetNbinsX()){
	      xbin = h_electron_eff->GetNbinsX();
	    }
	    electron_eff = h_electron_eff->GetBinContent(xbin, ybin);      
	    // if ( std::abs(part->eta()) > 2.5 ){electron_eff = 0.0;}
	  }    
	  //if (m_rnd->Rndm() > trk_eff){
	    //continue;
	  //}
	}

	if ( h_muon_eff != nullptr && (abs(pdgid) == 13) ) {
	  ANA_MSG_VERBOSE ("Candidate muon; is it reconstructed?");
	  int xbin = h_muon_eff->GetXaxis()->FindBin(part->pt());
	  int ybin = h_muon_eff->GetYaxis()->FindBin(part->eta());
	  // float trk_eff = 0.0; //default if underflow
	  muon_eff = 0.0; //default if underflow
	  if ( xbin > 0 && ybin > 0 ) {
	    //use last bin if overflow
	    if (xbin > h_muon_eff->GetNbinsX()){
	      xbin = h_muon_eff->GetNbinsX();
	    }
	    muon_eff = h_muon_eff->GetBinContent(xbin, ybin);      
	    if ( std::abs(part->eta()) > 2.5 ){
	      muon_eff = 0.0;
	    }
	  }    
	  // if (m_rnd->Rndm() > trk_eff)
	  //continue;
	}


	ANA_MSG_VERBOSE ("It was recoed.  Store as lepton");
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

    /*
    m_truetrk_pt->push_back(part->pt());
    m_truetrk_eta->push_back(part->eta());
    m_truetrk_phi->push_back(part->phi());
    m_truetrk_charge->push_back(static_cast<int>(part->charge()));
    m_truetrk_pdgid->push_back(pdgid);
    */
    tmpsizetruth += 1;

    //apply parametrized tracking efficiency, if requested

    //Old 1D track efficiency
    /*
    if (h_trk_eff_pt != nullptr) {
      int ibin = h_trk_eff_pt->FindBin(part->pt());
      float trk_eff = 0.0; //default if underflow
      if (ibin > 0) {
	//use last bin if overflow
	if (ibin > h_trk_eff_pt->GetNbinsX()) ibin = h_trk_eff_pt->GetNbinsX();
	trk_eff = h_trk_eff_pt->GetBinContent(ibin);      
      }  
    */
    //2D track efficiency
    if (h_trk_eff_pt_eta != nullptr) {
      int xbin = h_trk_eff_pt_eta->GetXaxis()->FindBin(part->pt());
      int ybin = h_trk_eff_pt_eta->GetYaxis()->FindBin(part->eta());
      float trk_eff = 0.0; //default if underflow
      if ( xbin > 0 && ybin > 0 ) {
	//use last bin if overflow
	if (xbin > h_trk_eff_pt_eta->GetNbinsX()) xbin = h_trk_eff_pt_eta->GetNbinsX();
	if (ybin > h_trk_eff_pt_eta->GetNbinsY()) ybin = h_trk_eff_pt_eta->GetNbinsY();
	trk_eff = h_trk_eff_pt_eta->GetBinContent(xbin, ybin); 
	//if (std::abs(part->eta()) > 2.5) { trk_eff= 0.0; }
	//myfile << part->pt()<< " " << part->eta()<< " " << trk_eff << "\n";
	}
    
      //increment a per-event "weight" with 1-trk_eff  
      m_weights->push_back( 1- trk_eff );
    }
    tmpsize += 1 ;

    //store basic kinematic and particle info
    ANA_MSG_VERBOSE("Store track");
    m_trk_pt->push_back(part->pt());
    m_trk_eta->push_back(part->eta());
    m_trk_phi->push_back(part->phi());
    m_trk_charge->push_back(static_cast<int>(part->charge()));
    m_trk_pdgid->push_back(pdgid);
    
} // end for loop over truth particles
  
  int num_track_in_window = m_trk_pt->size() + m_numPUtracks;

  //The "tracking weight" is done
float  tracking_weight = 1;
  for(int i = 0; (i < abs(m_weights->size())); i++)
    {
      tracking_weight *= m_weights->at(i);
    }

  // Now evaluate event-level selections  
  ANA_MSG_VERBOSE("Checking event selections");
  ANA_MSG_DEBUG("NLep = " << m_lep_pt->size() << ", NTracks = " << num_track_in_window);
  hist("num_fiducial_leptons")->Fill(m_lep_pt->size());
  if (m_lep_pt->size() != 2) {saveTree(); return StatusCode::SUCCESS;}
  hist("pass_2leps")->Fill(1,mcWeight*electron_eff*muon_eff);
  if (m_lep_charge->at(0)*m_lep_charge->at(1) != -1) {saveTree(); return StatusCode::SUCCESS;}
  //if ( ( abs(m_lep_pdgid->at(0)*m_lep_pdgid->at(1)) != 11*13 ) && ( abs(m_lep_pdgid->at(0)*m_lep_pdgid->at(1) ) !=  11*11 ) && ( abs(m_lep_pdgid->at(0)*m_lep_pdgid->at(1) ) !=  13*13 ) ) {saveTree(); return StatusCode::SUCCESS;} //For all e and mu selections 
  hist("pass_2leps_OS")->Fill(1,mcWeight*electron_eff*muon_eff);
  if (abs(m_lep_pdgid->at(0)*m_lep_pdgid->at(1)) != 11*13){ saveTree(); return StatusCode::SUCCESS;} //For just emu
  // if ( abs(m_lep_pdgid->at(0)*m_lep_pdgid->at(1) ) !=  13*13 )  {saveTree(); return StatusCode::SUCCESS;}// mumu
  hist("pass_2leps_OS_OF")->Fill(1,mcWeight*electron_eff*muon_eff);
  passCut(cut_lep_ocof);
  ANA_MSG_VERBOSE("Pass cut_lep_ocof");

  if (std::min(m_lep_pt->at(0), m_lep_pt->at(1)) < lep2_min_pt) {saveTree(); return StatusCode::SUCCESS;}
  hist("pass_2leps_OS_OF_sublep")->Fill(1,mcWeight*electron_eff*muon_eff);
  if (std::max(m_lep_pt->at(0), m_lep_pt->at(1)) < lep1_min_pt) {saveTree(); return StatusCode::SUCCESS;}
  hist("pass_2leps_OS_OF_sublep_leadlep")->Fill(1,mcWeight*electron_eff*muon_eff);
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
  if (m_delta_phi > 3.1415 ) { m_delta_phi = 6.283 - m_delta_phi;}

  hist("dilep_m")->Fill(m_dilep_m/GeV);
  if (m_dilep_m < dilep_min_mass) //||  ((m_dilep_m> 70*GeV ) && ( m_dilep_m < 110*GeV ) ) ) m(ll) cut
    {saveTree(); return StatusCode::SUCCESS;}
  hist("pass_2leps_OS_OF_sublep_leadlep_mll")->Fill(1,mcWeight*electron_eff*muon_eff);
  passCut(cut_m_ll);
  ANA_MSG_VERBOSE("Pass cut_m_ll");


  hist("dilep_pt")->Fill(m_dilep_pt/GeV);
  if (m_dilep_pt < dilep_min_pt) {saveTree(); return StatusCode::SUCCESS;}
  passCut(cut_pt_ll);
  hist("pass_2leps_OS_OF_sublep_leadlep_mll_ptll")->Fill(1,mcWeight*electron_eff*muon_eff);
  ANA_MSG_VERBOSE("Pass cut_pt_ll");

  //hist("sr_dilep_pt_weights")->Fill(m_dilep_pt/GeV, tracking_weight*electron_eff*muon_eff);
  hist("track_weights")->Fill(tracking_weight);
  hist("electron_eff")->Fill(electron_eff);
  hist("muon_eff")->Fill(muon_eff);
  hist("num_fiducial_tracks")->Fill(m_trk_pt->size(),mcWeight);
  hist("pileup_weights")->Fill(Pileup_eff);
  hist("total_weighting")->Fill(Pileup_eff*mcWeight*tracking_weight*electron_eff*muon_eff*Fakes_eff);
  hist("sr_dilep_pt_weights")->Fill(m_dilep_pt/GeV);//, tracking_weight*electron_eff*muon_eff*Pileup_eff*mcWeight);//*Fakes_eff);
  hist("mcWeight")->Fill(mcWeight);
  hist("weighted_number_of_events")->Fill(1,mcWeight*tracking_weight*electron_eff*muon_eff*Pileup_eff);
 if (num_track_in_window > tracks_max_n) {saveTree(); return StatusCode::SUCCESS;}
  passCut(cut_exclusive);
  ANA_MSG_VERBOSE("Pass cut_exclusive");
  hist("pass_2leps_OS_OF_sublep_leadlep_mll_ptll_0trk")->Fill(1,mcWeight*electron_eff*muon_eff);
  hist("pass_2leps_OS_OF_sublep_leadlep_mll_ptll_0trk_withPU")->Fill(1,Pileup_eff*mcWeight*electron_eff*muon_eff);
  //Plots after all selections
  hist("sr_dilep_pt")->Fill(m_dilep_pt/GeV);
  hist("delta_phi")->Fill(m_delta_phi);
  //hist("sr_dilep_pt_weights")->Fill(m_dilep_pt/GeV, tracking_weight*electron_eff*muon_eff);
  
  myfile.close();
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
