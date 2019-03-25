#include <AsgTools/MessageCheck.h>
#include <LowPtAnalysis/BasicPerf.h>

#include <xAODEventInfo/EventInfo.h>

#include <xAODTruth/TruthParticleContainer.h>
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
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  std::cout<<"start execute"<<std::endl;

  // Read/fill the EventInfo variables:
  const xAOD::EventInfo* ei = nullptr;
  std::cout<<"1"<<std::endl;
  ANA_CHECK (evtStore()->retrieve (ei, "EventInfo"));
  std::cout<<"2"<<std::endl;
  m_runNumber = ei->runNumber ();
  std::cout<<"3"<<std::endl;
  m_eventNumber = ei->eventNumber ();
  std::cout<<"4"<<std::endl;

  // get truth particle container of interest
  const xAOD::TruthParticleContainer* truthParts = 0;
  std::cout<<"5"<<std::endl;
  ANA_CHECK (evtStore()->retrieve( truthParts, "TruthParticles"));
  //if ( !evtStore()->retrieve( truthParts, "TruthParticles").isSuccess() ){
  //Error("execute()", "Failed to retrieve Input Jet container. Exiting." );
  //return StatusCode::FAILURE;
  //}
  std::cout<<"6"<<std::endl;
  ANA_MSG_INFO ("execute(): number of truth particles = " << truthParts->size());
  std::cout<<"7"<<std::endl;

  // get truth particle container of interest
  const xAOD::TrackParticleContainer* trackParts = 0;
  std::cout<<"8"<<std::endl;
  ANA_CHECK (evtStore()->retrieve( trackParts, "InDetTrackParticles"));
  std::cout<<"9"<<std::endl;
  ANA_MSG_INFO ("execute(): number of track particles = " << trackParts->size());

  std::cout<<"clearing"<<std::endl;

  m_truthEta->clear();
  m_truthPhi->clear();
  m_truthPt->clear();
  m_truthE->clear();
  m_truthQoverP->clear();
  m_truthPDGID->clear();

  m_trackPt->clear();

  int particle_is_reconstructed;

  std::cout<<"about to start truth loop"<<std::endl;

  // loop over the particles in the container
  for (const xAOD::TruthParticle *part : *truthParts) {

    particle_is_reconstructed = 0;

    if( part->auxdata<int>("status") == 1 && part->auxdata<int>("barcode") < 200000 && std::abs(part->charge()) > 0){
      std::cout<<part->auxdata<int>("pdgId")<<" "<<part->charge()<<std::endl;
      m_truthEta->push_back (part->eta ());
      m_truthPhi->push_back (part->phi ());
      m_truthPt-> push_back (part->pt ());
      m_truthE->  push_back (part->e ());
      //if(part->isAvailable< auxdata<float> >("qOverP")){
      //m_truthQoverP->push_back (part->auxdata<float>("qOverP")) ;
      //}
      m_truthPDGID->push_back (part->auxdata<int>("pdgId"));
      //if(1./part->auxdata<float>("qOverP") > 1){
      //std::cout<<1./part->auxdata<float>("qOverP")<<std::endl;
      //}

      std::cout<<"truth barcode "<<part->barcode()<<std::endl;
      
      /*
      ElementLink< xAOD::TruthParticleContainer > truthLink;
      for (const xAOD::TrackParticle *trpart : *trackParts) {
      	//ElementLink< xAOD::TruthParticleContainer > truthLink;
	if ((trpart)->isAvailable< ElementLink< xAOD::TruthParticleContainer > > ("truthParticleLink")){
	  //std::cout<<"it's available"<<std::endl;
	  truthLink = (trpart)->auxdata< ElementLink< xAOD::TruthParticleContainer>  >("truthParticleLink");
	  if(truthLink.isValid()){
	    std::cout<<(*truthLink)->barcode()<<std::endl;
	    ////if( truthLink ){
	    ////particle_is_reconstructed = 1;
	    ////}
	  }
	}
      }
*/      

      //p_Reco_eff_vs_track_pt->Fill(part->pt()/1000., particle_is_reconstructed);
      hist("Reco_eff_vs_track_pt")->Fill(part->pt()/1000., particle_is_reconstructed);

    }

    //ANA_MSG_INFO ("execute(): jet pt = " << (jet->pt() * 0.001) << " GeV"); // just to print out something
  } // end for loop over truth particles

  std::cout<<"end of truth part loop"<<std::endl;

  for (const xAOD::TrackParticle *track_part : *trackParts) {

    m_trackPt-> push_back (track_part->pt ());

  } // end for loop over track particles

  std::cout<<"end of track part loop"<<std::endl;

  // Fill the event into the tree:
  tree ("analysis")->Fill ();

  std::cout<<"filled tree"<<std::endl;

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
