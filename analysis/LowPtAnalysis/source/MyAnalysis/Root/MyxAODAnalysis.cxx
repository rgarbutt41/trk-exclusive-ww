#include <AsgTools/MessageCheck.h>
#include <MyAnalysis/MyxAODAnalysis.h>

#include <xAODEventInfo/EventInfo.h>

#include <xAODTau/TauJetContainer.h>


MyxAODAnalysis :: MyxAODAnalysis (const std::string& name,
                                  ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm (name, pSvcLocator)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  This is also where you
  // declare all properties for your algorithm.  Note that things like
  // resetting statistics variables or booking histograms should
  // rather go into the initialize() function.
}



StatusCode MyxAODAnalysis :: initialize ()
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
  m_taujetEta = new std::vector<float>();
  mytree->Branch ("TauJetEta", &m_taujetEta);
  m_taujetPhi = new std::vector<float>();
  mytree->Branch ("TauJetPhi", &m_taujetPhi);
  m_taujetPt = new std::vector<float>();
  mytree->Branch ("TauJetPt", &m_taujetPt);
  m_taujetE = new std::vector<float>();
  mytree->Branch ("TauJetE", &m_taujetE);

  return StatusCode::SUCCESS;
}



StatusCode MyxAODAnalysis :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  // Read/fill the EventInfo variables:
  const xAOD::EventInfo* ei = nullptr;
  ANA_CHECK (evtStore()->retrieve (ei, "EventInfo"));
  m_runNumber = ei->runNumber ();
  m_eventNumber = ei->eventNumber ();


  // get tau jet container of interest
  const xAOD::TauJetContainer* jets = 0;
  ANA_CHECK (evtStore()->retrieve( jets, "TauJets"));
  ANA_MSG_INFO ("execute(): number of jets = " << jets->size());

  m_taujetEta->clear();
  m_taujetPhi->clear();
  m_taujetPt->clear();
  m_taujetE->clear();

  // loop over the jets in the container
  for (const xAOD::TauJet *jet : *jets) {

    m_taujetEta->push_back (jet->eta ());
    m_taujetPhi->push_back (jet->phi ());
    m_taujetPt-> push_back (jet->pt ());
    m_taujetE->  push_back (jet->e ());

    //ANA_MSG_INFO ("execute(): jet pt = " << (jet->pt() * 0.001) << " GeV"); // just to print out something
  } // end for loop over jets

  // Fill the event into the tree:
  tree ("analysis")->Fill ();


  return StatusCode::SUCCESS;
}



StatusCode MyxAODAnalysis :: finalize ()
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


MyxAODAnalysis :: ~MyxAODAnalysis () {
  delete m_taujetEta;
  delete m_taujetPhi;
  delete m_taujetPt;
  delete m_taujetE;
}
