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

  //Reco tracks
  m_trackPt = new std::vector<float>();
  mytree->Branch ("TrackPt", &m_trackPt);
  m_trackTruthIndex = new std::vector<int>();
  mytree->Branch ("TrackTruthIndex", &m_trackTruthIndex);
  m_TruthMatchProb = new std::vector<float>();
  mytree->Branch ("TruthMatchProb", &m_TruthMatchProb);

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

  std::vector< const xAOD::TruthParticle* > vec_of_truth_pointers;

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

  } // end for loop over truth particles


  std::vector<int> vec_of_matched_truth_indices(vec_of_truth_pointers.size(), 0); //vector of 0s and 1s: one for each truth particle in the event.  If 0, the truth particle is unmatched; if 1, it is matched
  
  int truthMatchIndex=-2; //-2 = not matched; -1 = matched, no particle found; >=0 link to position in truth particle branches  
  for (const xAOD::TrackParticle *track_part : *trackParts) {
    m_trackPt-> push_back (track_part->pt ());

    //hist("num_reco_tracks_vs_actualints")->Fill(ei->actualInteractionsPerCrossing());
    //hist("num_reco_tracks_vs_actualints")->Fill(ei->actualInteractionsPerCrossing());

    //check truth link    
    float probMatch = track_part->auxdataConst<float>("truthMatchProbability");
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
	    }
	  }
	} else {
	  ANA_MSG_VERBOSE( "Matching track with no truth information found." );
	}      
      } //check truth link
    } //truth-matched track
    else{
      hist("num_unmatched_track_particles_vs_track_pt")->Fill( track_part->pt());
    } //unmatched track
    m_trackTruthIndex->push_back(truthMatchIndex);

  } // end for loop over track particles


  std::vector<const xAOD::TruthParticle*>::iterator partitr;
  for(partitr = vec_of_truth_pointers.begin(); partitr < vec_of_truth_pointers.end(); partitr++){
    if( vec_of_matched_truth_indices.at( partitr - vec_of_truth_pointers.begin() ) == 0 ){ //this truth particle is unmatched
      hist("num_unmatched_truth_particles_vs_truth_pt")->Fill( (*partitr)->pt());
    }
  }
  

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
}
