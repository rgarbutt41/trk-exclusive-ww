#include <AsgTools/MessageCheck.h>
#include <LowPtAnalysis/BasicPerf.h>

#include <xAODEventInfo/EventInfo.h>

#include <xAODTruth/TruthParticleContainer.h>
#include <xAODTruth/xAODTruthHelpers.h>
#include <xAODTracking/TrackParticleContainer.h>

BasicPerf :: BasicPerf (const std::string& name,
                                  ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm (name, pSvcLocator)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  This is also where you
  // declare all properties for your algorithm.  Note that things like
  // resetting statistics variables or booking histograms should
  // rather go into the initialize() function.
}



StatusCode BasicPerf :: initialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  ANA_MSG_INFO ("in initialize");

  ANA_CHECK (book (TTree ("analysis", "My analysis ntuple")));
  TTree* mytree = tree ("analysis");

  //Event-level
  mytree->Branch ("RunNumber", &m_runNumber);
  mytree->Branch ("EventNumber", &m_eventNumber);

  //Truth particles
  m_truthEta = new std::vector<float>();
  mytree->Branch ("TruthEta", &m_truthEta);
  m_truthPhi = new std::vector<float>();
  mytree->Branch ("TruthPhi", &m_truthPhi);
  m_truthPt = new std::vector<float>();
  mytree->Branch ("TruthPt", &m_truthPt);
  m_truthE = new std::vector<float>();
  mytree->Branch ("TruthE", &m_truthE);
  m_truthQoverP = new std::vector<float>();
  mytree->Branch ("TruthQoverP", &m_truthQoverP);
  m_truthPDGID = new std::vector<int>();
  mytree->Branch ("TruthPDGID", &m_truthPDGID);
  m_truthRecoIndex = new std::vector<int>();
  mytree->Branch ("TruthRecoIndex", &m_truthRecoIndex);

  //Reco tracks (see https://gitlab.cern.ch/atlas/athena/blob/21.2/Event/xAOD/xAODTracking/xAODTracking/TrackingPrimitives.h)
  m_trackPt = new std::vector<float>();
  mytree->Branch ("TrackPt", &m_trackPt);
  m_trackTruthIndex = new std::vector<int>();
  mytree->Branch ("TrackTruthIndex", &m_trackTruthIndex);
  m_TruthMatchProb = new std::vector<float>();
  mytree->Branch ("TruthMatchProb", &m_TruthMatchProb);
  m_trackProperties = new std::vector<int>();
  mytree->Branch ("TrackProperties", &m_trackProperties);
  m_trackPatternRecoInfo = new std::vector<int>();
  mytree->Branch ("TrackPatternRecoInfo", &m_trackPatternRecoInfo);
  m_trackParticleHypothesis = new std::vector<int>();
  mytree->Branch ("TrackParticleHypothesis", &m_trackParticleHypothesis);
  m_trackz0 = new std::vector<float>();
  mytree->Branch ("Trackz0", &m_trackz0);
  m_trackd0 = new std::vector<float>();
  mytree->Branch ("Trackd0", &m_trackd0);
  m_trackNSiHits = new std::vector<int>();
  mytree->Branch ("TrackNSiHits", &m_trackNSiHits);
  m_trackMatchID = new std::vector<int>();
  mytree->Branch ("TrackMatchID", &m_trackMatchID);



  ANA_CHECK (book ( TProfile ("Reco_eff_vs_track_pt", "Reco_eff_vs_track_pt", 200, 0, 10) ));
  ANA_CHECK (book ( TH2F ("TruthRecoIndex_and_TruthPt", "TruthRecoIndex_and_TruthPt", 50, -10, 90, 40, 0, 2000) ) );
  ANA_CHECK (book ( TH1F("num_matched_truth_particles_vs_truth_pt","num_matched_truth_particles_vs_truth_pt",40, 0, 2000) ) );
  ANA_CHECK (book ( TH1F("num_matched_track_particles_vs_track_pt","num_matched_track_particles_vs_track_pt",40, 0, 2000) ) );
  ANA_CHECK (book ( TH1F("num_unmatched_track_particles_vs_track_pt","num_unmatched_track_particles_vs_track_pt",40, 0, 2000) ) );
  ANA_CHECK (book ( TH1F("num_unmatched_truth_particles_vs_truth_pt","num_unmatched_truth_particles_vs_truth_pt",40, 0, 2000) ) );
  //ANA_CHECK (book ( TH1F("num_reco_tracks_vs_actualints","num_reco_tracks_vs_actualints",100, 0, 100) ) );
  //ANA_CHECK (book ( TH1F("num_reco_tracks_vs_avgints","num_reco_tracks_vs_avgints",100, 0, 100) ) );
  ANA_CHECK (book ( TProfile("num_reco_tracks_vs_actualints","num_reco_tracks_vs_actualints",100, 0, 100) ) );
  ANA_CHECK (book ( TProfile("num_reco_tracks_vs_avgints","num_reco_tracks_vs_avgints",100, 0, 100) ) ); 
  ANA_CHECK (book ( TProfile("num_reco_tracks_vs_num_truth_parts","num_reco_tracks_vs_num_truth_parts",50, 0, 5000) ) );

  ANA_CHECK (book ( TProfile("Frac_reco_track_with_lowmatchprob_vs_track_pt","Frac_reco_track_with_lowmatchprob_vs_track_pt",40, 0, 2000) ) );
  ANA_CHECK (book ( TProfile("Frac_reco_track_with_goodprob_and_missingtruth_vs_track_pt","Frac_reco_track_with_goodprob_and_missingtruth_vs_track_pt",40, 0, 2000) ) );
  ANA_CHECK (book ( TH1F("Num_reco_track_with_lowmatchprob_vs_track_pt","Num_reco_track_with_lowmatchprob_vs_track_pt",40, 0, 2000) ) );
  ANA_CHECK (book ( TH1F("Num_reco_track_with_goodprob_vs_track_pt","Num_reco_track_with_goodprob_vs_track_pt",40, 0, 2000) ) );

  ANA_CHECK (book ( TH1F("Num_reco_track_with_goodprob_and_missingtruth_vs_track_pt","Num_reco_track_with_goodprob_and_missingtruth_vs_track_pt",40, 0, 2000) ) );
  ANA_CHECK (book ( TH1F("Num_reco_track_with_goodprob_and_missingtruth_vs_abs_d0","Num_reco_track_with_goodprob_and_missingtruth_vs_abs_d0",100, 0, 20) ) );
  ANA_CHECK (book ( TH1F("Num_reco_track_with_goodprob_and_missingtruth_vs_abs_d0_zoom","Num_reco_track_with_goodprob_and_missingtruth_vs_abs_d0_zoom",100, 0, 2) ) );
  ANA_CHECK (book ( TH1F("Num_reco_track_with_goodprob_and_missingtruth_vs_abs_z0","Num_reco_track_with_goodprob_and_missingtruth_vs_abs_z0",300, 0, 300) ) );
  ANA_CHECK (book ( TH1F("Num_reco_track_with_goodprob_and_missingtruth_vs_numSiHits","Num_reco_track_with_goodprob_and_missingtruth_vs_numSiHits",20, 0, 20) ) );

  ANA_CHECK (book ( TH1F("Num_reco_track_with_goodprob_and_foundtruth_vs_track_pt","Num_reco_track_with_goodprob_and_foundtruth_vs_track_pt",40, 0, 2000) ) );
  ANA_CHECK (book ( TH1F("Num_reco_track_with_goodprob_and_foundtruth_vs_abs_d0","Num_reco_track_with_goodprob_and_foundtruth_vs_abs_d0",100, 0, 20) ) );
  ANA_CHECK (book ( TH1F("Num_reco_track_with_goodprob_and_foundtruth_vs_abs_d0_zoom","Num_reco_track_with_goodprob_and_foundtruth_vs_abs_d0_zoom",100, 0, 2) ) );
  ANA_CHECK (book ( TH1F("Num_reco_track_with_goodprob_and_foundtruth_vs_abs_z0","Num_reco_track_with_goodprob_and_foundtruth_vs_abs_z0",300, 0, 300) ) );
  ANA_CHECK (book ( TH1F("Num_reco_track_with_goodprob_and_foundtruth_vs_numSiHits","Num_reco_track_with_goodprob_and_foundtruth_vs_numSiHits",20, 0, 20) ) );

  //These two are filled as followed: if the particleHypothesis of a track is an electron (muon), the histogram will be filled.  If the track is truth-matched to an electron, the 1 bin is filled.  If the track is truth-matched to something else, or doesn't have a matched truth particle, the 0 bin is filled.  Charge matters.
  ANA_CHECK (book ( TH1F("Correctly_matched_electron_track","Correctly_matched_electron_track",2, 0, 2) ) );
  ANA_CHECK (book ( TH1F("Correctly_matched_muon_track","Correctly_matched_muon_track",2, 0, 2) ) );


  ANA_CHECK (book ( TH1F("Num_truth_electrons","Num_truth_electrons",30, 0, 30) ) );
  ANA_CHECK (book ( TH1F("Num_truth_muons","Num_truth_muons",30, 0, 30) ) );
  ANA_CHECK (book ( TH1F("Num_truth_e_and_mu","Num_truth_e_and_mu",30, 0, 30) ) );
  
  ANA_CHECK (book ( TH1F("Num_truth_electrons_withtracks","Num_truth_electrons_withtracks",30, 0, 30) ) );
  ANA_CHECK (book ( TH1F("Num_truth_muons_withtracks","Num_truth_muons_withtracks",30, 0, 30) ) );

  ANA_CHECK (book ( TH1F("Electron_muon_z0_diff","Electron_muon_z0_diff",50, 0, 5) ) ); //if there is one e and one mu in the event, and they have opposite charge
  ANA_CHECK (book ( TH1F("Num_tracks_in_exclusive_window","Num_tracks_in_exclusive_window",30, 0, 30) ) );


  return StatusCode::SUCCESS;
}

StatusCode BasicPerf :: execute ()
{
  // Read/fill the EventInfo variables:
  const xAOD::EventInfo* ei = nullptr;
  ANA_CHECK (evtStore()->retrieve (ei, "EventInfo"));
  m_runNumber = ei->runNumber ();
  m_eventNumber = ei->eventNumber ();

  // get truth particle container of interest
  const xAOD::TruthParticleContainer* truthParts = 0;
  ANA_CHECK (evtStore()->retrieve( truthParts, "TruthParticles"));
  ANA_MSG_DEBUG ("execute(): number of truth particles = " << truthParts->size());

  // get truth particle container of interest
  const xAOD::TrackParticleContainer* trackParts = 0;
  ANA_CHECK (evtStore()->retrieve( trackParts, "InDetTrackParticles"));
  ANA_MSG_DEBUG ("execute(): number of track particles = " << trackParts->size());

  hist("num_reco_tracks_vs_actualints")->Fill(ei->actualInteractionsPerCrossing(), trackParts->size() );
  hist("num_reco_tracks_vs_actualints")->Fill(ei->averageInteractionsPerCrossing(), trackParts->size() );
  hist("num_reco_tracks_vs_num_truth_parts")->Fill(truthParts->size(), trackParts->size() );

  m_truthEta->clear();
  m_truthPhi->clear();
  m_truthPt->clear();
  m_truthE->clear();
  m_truthQoverP->clear();
  m_truthPDGID->clear();
  m_truthRecoIndex->clear();
  m_trackPt->clear();
  m_trackTruthIndex->clear();
  m_TruthMatchProb->clear();
  m_trackProperties->clear();
  m_trackPatternRecoInfo->clear();
  m_trackParticleHypothesis->clear();
  m_trackz0->clear();
  m_trackd0->clear();
  m_trackNSiHits->clear();
  m_trackMatchID->clear();

  int num_electrons = 0;
  int num_muons = 0;
  //int num_electrons_withtrack = 0;
  //int num_muons_withtrack = 0;


  std::vector< const xAOD::TruthParticle* > vec_of_truth_pointers;
  //std::vector< const xAOD::TruthParticle* > vec_of_electron_pointers;
  //std::vector< const xAOD::TruthParticle* > vec_of_muon_pointers;

  // loop over the particles in the container
  for (const xAOD::TruthParticle *part : *truthParts) {    

    //select fiducial truth particles
    if (part->pt() < 100.0) continue;
    if (part->abseta() > 2.5 ) continue;
    if (part->charge() == 0) continue;
    if (part->auxdata<int>("status") != 1) continue;
    if (part->auxdata<int>("barcode") > 200000) continue;

    //store basic kinematic and particle info
    m_truthEta->push_back (part->eta ());
    m_truthPhi->push_back (part->phi ());
    m_truthPt-> push_back (part->pt ());
    m_truthE->  push_back (part->e ());
    m_truthPDGID->push_back (part->auxdata<int>("pdgId"));

    vec_of_truth_pointers.push_back( (part) );
    //if( std::abs(part->auxdata<int>("pdgId")) == 11 ) vec_of_electron_pointers.push_back( (part) );
    //if( std::abs(part->auxdata<int>("pdgId")) == 13 ) vec_of_muon_pointers.push_back( (part) );

    if( std::abs(part->auxdata<int>("pdgId")) == 11 ) num_electrons++;
    if( std::abs(part->auxdata<int>("pdgId")) == 13 ) num_muons++;

    /*
    //retrieve reco track matched to this particle (first one considered, TODO: improve!)
    ElementLink< xAOD::TruthParticleContainer > truthLink;
    int recoTrackIndex=-1;
    int trackIndex=-1;
    for (const xAOD::TrackParticle *trpart : *trackParts) {
      trackIndex++;
      float probMatch = trpart->auxdataConst<float>("truthMatchProbability");
      if (probMatch < 0.5) continue; //not a good match
      if ((trpart)->isAvailable< ElementLink< xAOD::TruthParticleContainer > > ("truthParticleLink")){
	truthLink = (trpart)->auxdata< ElementLink< xAOD::TruthParticleContainer>  >("truthParticleLink");
	if(truthLink.isValid()) {
	  if ( (*truthLink) == part ) {
	    ANA_MSG_VERBOSE( "Matching particle found. Barcode: " << (*truthLink)->barcode() << ", reco,truth pT (MeV) = " <<  trpart->pt() << ", " << (*truthLink)->pt());
	    recoTrackIndex = trackIndex;
	  }
	}
      }
    }

    m_truthRecoIndex->push_back(recoTrackIndex);
    hist("Reco_eff_vs_track_pt")->Fill(part->pt()/1000., recoTrackIndex >=0 ? 1.0 : 0.0);

    hist("TruthRecoIndex_and_TruthPt")->Fill(recoTrackIndex, part->pt());
    */
    //Edit: Now I loop over good truth particles in the track particle. Kept in the above loop in case I want to restore it later

  } // end for loop over truth particles


  std::vector< const xAOD::TrackParticle* > vec_of_electrontrack_pointers;
  std::vector< const xAOD::TrackParticle* > vec_of_muontrack_pointers;

  std::vector<float> vec_of_track_z0s; //all z0's of tracks

  std::vector<int> vec_of_matched_truth_indices(vec_of_truth_pointers.size(), 0); //vector of 0s and 1s: one for each truth particle in the event.  If 0, the truth particle is unmatched; if 1, it is matched
  
  int truthMatchIndex=-2; //-2 = not matched; -1 = matched, no particle found; >=0 link to position in truth particle branches  

  //start loop over track particles
  for (const xAOD::TrackParticle *track_part : *trackParts) {

    int track_is_matched = 0;
    int track_matched_id = 0;

    int no_truth_link_available = 0;
    int truth_link_not_valid = 0;
    int truth_barcode_is_zero = 0;

    int trackhypo = track_part->auxdataConst<unsigned char>("particleHypothesis");//1 = electron, 2 = muon
    if(track_part->charge() < 0) trackhypo = -1*trackhypo;
    int trackhypo_isright = 0; //becomes 1 if the track is matched to the right charge/type of lepton at truth level


    m_trackPt-> push_back (track_part->pt ());
    m_trackProperties->push_back( track_part->auxdataConst<unsigned char>("trackProperties") );
    m_trackPatternRecoInfo->push_back( track_part->auxdataConst<unsigned long>("patternRecoInfo") );
    m_trackParticleHypothesis->push_back( track_part->auxdataConst<unsigned char>("particleHypothesis") ); //1 = electron, 2 = muon
    m_trackz0-> push_back( track_part->auxdataConst<float>("z0") );    
    m_trackd0-> push_back( track_part->auxdataConst<float>("d0") );    
    m_trackNSiHits-> push_back( track_part->auxdataConst<unsigned char>("numberOfPixelHits") + track_part->auxdataConst<unsigned char>("numberOfSCTHits") );

    vec_of_track_z0s.push_back(track_part->auxdataConst<float>("z0"));

    //check truth link    
    float probMatch = track_part->auxdataConst<float>("truthMatchProbability");

    hist("Frac_reco_track_with_lowmatchprob_vs_track_pt")->Fill( track_part->pt() , probMatch < 0.5 ? 1.0 : 0.0);
    if(probMatch < 0.5){
      hist("Num_reco_track_with_lowmatchprob_vs_track_pt")->Fill( track_part->pt() );
    }

    m_TruthMatchProb->push_back(probMatch);
    if (probMatch >= 0.5) {
      truthMatchIndex=-1;
      if ((track_part)->isAvailable< ElementLink< xAOD::TruthParticleContainer > > ("truthParticleLink")){
	ElementLink< xAOD::TruthParticleContainer > truthLink = (track_part)->auxdata< ElementLink< xAOD::TruthParticleContainer>  >("truthParticleLink");
	if(truthLink.isValid()) {
	  //Find index of stored truth particle
	  //for (truthP: m_truthParticles) //...	
	  truthMatchIndex = 0; //for now just put 0 always, to flag it's matched with a link. @TODO: fix with proper indexing if needed
	  std::vector<const xAOD::TruthParticle*>::iterator partitr;
	  for(partitr = vec_of_truth_pointers.begin(); partitr < vec_of_truth_pointers.end(); partitr++){
	    if((*truthLink) == (*partitr)){
	      hist("num_matched_truth_particles_vs_truth_pt")->Fill( (*partitr)->pt());
	      hist("num_matched_track_particles_vs_track_pt")->Fill( track_part->pt());
	      vec_of_matched_truth_indices.at( partitr - vec_of_truth_pointers.begin() ) = 1; //this partitr is matched!

	      hist("Num_reco_track_with_goodprob_and_foundtruth_vs_track_pt")->Fill( track_part->pt() );
	      hist("Num_reco_track_with_goodprob_and_foundtruth_vs_abs_d0")->Fill( std::abs( track_part->auxdataConst<float>("d0") ) );
	      hist("Num_reco_track_with_goodprob_and_foundtruth_vs_abs_d0_zoom")->Fill( std::abs( track_part->auxdataConst<float>("d0") ) );
	      hist("Num_reco_track_with_goodprob_and_foundtruth_vs_abs_z0")->Fill( std::abs( track_part->auxdataConst<float>("z0") ) );
	      hist("Num_reco_track_with_goodprob_and_foundtruth_vs_numSiHits")->Fill( track_part->auxdataConst<unsigned char>("numberOfPixelHits") + track_part->auxdataConst<unsigned char>("numberOfSCTHits") );

	      if(trackhypo == 1 && (*partitr)->auxdata<int>("pdgId") == 11) trackhypo_isright = 1;
	      if(trackhypo == -1 && (*partitr)->auxdata<int>("pdgId") == -11) trackhypo_isright = 1;
	      if(trackhypo == 2 && (*partitr)->auxdata<int>("pdgId") == 13) trackhypo_isright = 1;
	      if(trackhypo == -2 && (*partitr)->auxdata<int>("pdgId") == -13) trackhypo_isright = 1;
	      
	      if( std::abs( (*partitr)->auxdata<int>("pdgId") ) == 11 ){
		vec_of_electrontrack_pointers.push_back( (track_part) );
	      }
	      if( std::abs( (*partitr)->auxdata<int>("pdgId") ) == 13 ){
		vec_of_muontrack_pointers.push_back( (track_part) );
	      }

	      track_is_matched = 1;
	      track_matched_id = (*partitr)->auxdata<int>("pdgId");

	      if((*truthLink)->barcode() == 0){
		truth_barcode_is_zero++;
	      }
	    }
	  }
	} else {
	  ANA_MSG_VERBOSE( "Matching track with no truth information found." );
	  truth_link_not_valid++;
	} //truthlink.isvalid check
      } else{
	no_truth_link_available++;
      } //check truth link
    } //truth-matched track
    else{
      hist("num_unmatched_track_particles_vs_track_pt")->Fill( track_part->pt());
    } //unmatched track
    m_trackTruthIndex->push_back(truthMatchIndex);

    if(probMatch >= 0.5){
      hist("Num_reco_track_with_goodprob_vs_track_pt")->Fill( track_part->pt() );	
      if(truth_barcode_is_zero > 0.1 || truth_link_not_valid > 0.1 || no_truth_link_available > 0.1){
	hist("Frac_reco_track_with_goodprob_and_missingtruth_vs_track_pt")->Fill( track_part->pt() , 1.0);
	hist("Num_reco_track_with_goodprob_and_missingtruth_vs_track_pt")->Fill( track_part->pt() );
	hist("Num_reco_track_with_goodprob_and_missingtruth_vs_abs_d0")->Fill( std::abs( track_part->auxdataConst<float>("d0") ) );
	hist("Num_reco_track_with_goodprob_and_missingtruth_vs_abs_d0_zoom")->Fill( std::abs( track_part->auxdataConst<float>("d0") ) );
	hist("Num_reco_track_with_goodprob_and_missingtruth_vs_abs_z0")->Fill( std::abs( track_part->auxdataConst<float>("z0") ) );
	hist("Num_reco_track_with_goodprob_and_missingtruth_vs_numSiHits")->Fill( track_part->auxdataConst<unsigned char>("numberOfPixelHits") + track_part->auxdataConst<unsigned char>("numberOfSCTHits") );
      } else{
	hist("Frac_reco_track_with_goodprob_and_missingtruth_vs_track_pt")->Fill( track_part->pt() , 0.0);	
      }
    }


    if(std::abs(trackhypo) == 1) hist("Correctly_matched_electron_track")->Fill( trackhypo_isright);
    if(std::abs(trackhypo) == 2) hist("Correctly_matched_electron_track")->Fill( trackhypo_isright);

    if(track_is_matched == 1){
      m_trackMatchID->push_back (track_matched_id);
    } else{
      m_trackMatchID->push_back (0);      
    }


  } // end for loop over track particles


  std::vector<const xAOD::TruthParticle*>::iterator partitr;
  for(partitr = vec_of_truth_pointers.begin(); partitr < vec_of_truth_pointers.end(); partitr++){

    hist("Reco_eff_vs_track_pt")->Fill((*partitr)->pt()/1000., vec_of_matched_truth_indices.at( partitr - vec_of_truth_pointers.begin() ) > 0 ? 1.0 : 0.0);

    if( vec_of_matched_truth_indices.at( partitr - vec_of_truth_pointers.begin() ) == 0 ){ //this truth particle is unmatched
      hist("num_unmatched_truth_particles_vs_truth_pt")->Fill( (*partitr)->pt());
    }
  }
  
  hist("Num_truth_electrons")->Fill( num_electrons );
  hist("Num_truth_muons")->Fill( num_muons );
  hist("Num_truth_e_and_mu")->Fill( num_electrons + num_muons);
 
  hist("Num_truth_electrons_withtracks")->Fill( vec_of_electrontrack_pointers.size() );
  hist("Num_truth_muons_withtracks")->Fill( vec_of_muontrack_pointers.size() );


  int tracks_in_window = 0;

  std::vector<const xAOD::TrackParticle*>::iterator eitr;
  std::vector<const xAOD::TrackParticle*>::iterator muitr;
  if( vec_of_electrontrack_pointers.size() == 1 && vec_of_muontrack_pointers.size() == 1 ){
    for(eitr = vec_of_electrontrack_pointers.begin(); eitr < vec_of_electrontrack_pointers.end(); eitr++){
      for(muitr = vec_of_muontrack_pointers.begin(); muitr < vec_of_muontrack_pointers.end(); muitr++){
	if( (*eitr)->charge() == (-1)*(*muitr)->charge() ){
	  hist("Electron_muon_z0_diff")->Fill( std::abs( (*eitr)->auxdataConst<float>("z0") - (*muitr)->auxdataConst<float>("z0") ) );

	  float lepvtxz0 = 0.5*( (*eitr)->auxdataConst<float>("z0") + (*muitr)->auxdataConst<float>("z0") );
	  for(unsigned int zz = 0; zz < vec_of_track_z0s.size(); zz++){
	    if( std::abs( lepvtxz0 - vec_of_track_z0s.at(zz) ) < 0.5 ){
	      tracks_in_window++;
	    }
	  }
	}
      }
    }
  }
  if(tracks_in_window > 0) hist("Num_tracks_in_exclusive_window")->Fill( tracks_in_window );



  // Fill the event into the tree:
  tree ("analysis")->Fill ();

  return StatusCode::SUCCESS;
}



StatusCode BasicPerf :: finalize ()
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


BasicPerf :: ~BasicPerf () {
  delete m_truthEta;
  delete m_truthPhi;
  delete m_truthPt;
  delete m_truthE;
  delete m_truthQoverP;
  delete m_truthPDGID;
  delete m_truthRecoIndex;

  delete m_trackPt;
  delete m_TruthMatchProb;
  delete m_trackTruthIndex;
  delete m_trackProperties;
  delete m_trackPatternRecoInfo;
  delete m_trackParticleHypothesis;
  delete m_trackz0;
  delete m_trackd0;
  delete m_trackNSiHits;
  delete m_trackMatchID;
}
