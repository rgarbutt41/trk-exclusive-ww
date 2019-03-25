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
  mytree->Branch ("RunNumber", &m_runNumber);
  mytree->Branch ("EventNumber", &m_eventNumber);
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


  m_trackPt = new std::vector<float>();
  mytree->Branch ("TrackPt", &m_trackPt);

  ANA_CHECK (book ( TProfile ("Reco_eff_vs_track_pt", "Reco_eff_vs_track_pt", 100, 0, 1000) ));
  //p_Reco_eff_vs_track_pt = new TProfile("Reco_eff_vs_track_pt", "", 100, 0, 1000);

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
  ANA_MSG_INFO ("execute(): number of truth particles = " << truthParts->size());

  // get truth particle container of interest
  const xAOD::TrackParticleContainer* trackParts = 0;
  ANA_CHECK (evtStore()->retrieve( trackParts, "InDetTrackParticles"));
  ANA_MSG_INFO ("execute(): number of track particles = " << trackParts->size());

  m_truthEta->clear();
  m_truthPhi->clear();
  m_truthPt->clear();
  m_truthE->clear();
  m_truthQoverP->clear();
  m_truthPDGID->clear();
  m_trackPt->clear();

  int particle_is_reconstructed;

  // loop over the particles in the container
  for (const xAOD::TruthParticle *part : *truthParts) {

    particle_is_reconstructed = 0;

    //select fiducial truth particles
    if (part->auxdata<int>("status") != 1) continue;
    if (part->auxdata<int>("barcode") > 200000) continue;
    if (part->charge() == 0) continue;

    //store basic kinematic and particle info
    m_truthEta->push_back (part->eta ());
    m_truthPhi->push_back (part->phi ());
    m_truthPt-> push_back (part->pt ());
    m_truthE->  push_back (part->e ());
    m_truthPDGID->push_back (part->auxdata<int>("pdgId"));

    //retrieve reco track matched to this particle (first one considered, TODO: improve!)
    ElementLink< xAOD::TruthParticleContainer > truthLink;
    for (const xAOD::TrackParticle *trpart : *trackParts) {
      float probMatch = trpart->auxdataConst<float>("truthMatchProbability");
      ANA_MSG_DEBUG("Truth particle with match prob=" << probMatch);
      if (probMatch < 0.5) continue; //not a good match
      ANA_MSG_DEBUG( "Truth matched particle. Finding truth link, if it exists." );
      if ((trpart)->isAvailable< ElementLink< xAOD::TruthParticleContainer > > ("truthParticleLink")){
	truthLink = (trpart)->auxdata< ElementLink< xAOD::TruthParticleContainer>  >("truthParticleLink");
	if(truthLink.isValid()){
	  ANA_MSG_DEBUG( "Matching particle found. Barcode: " << (*truthLink)->barcode() );
	  particle_is_reconstructed = 1;
	}
      }
    }

    hist("Reco_eff_vs_track_pt")->Fill(part->pt()/1000., particle_is_reconstructed);

  } // end for loop over truth particles

  for (const xAOD::TrackParticle *track_part : *trackParts) {

    m_trackPt-> push_back (track_part->pt ());

  } // end for loop over track particles

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

  delete m_trackPt;
}
