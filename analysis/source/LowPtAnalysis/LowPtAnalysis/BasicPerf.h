#ifndef LowPtAnalysis_BasicPerf_H
#define LowPtAnalysis_BasicPerf_H

#include <AnaAlgorithm/AnaAlgorithm.h>

#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <vector>

class BasicPerf : public EL::AnaAlgorithm
{
public:
  // this is a standard algorithm constructor
  BasicPerf (const std::string& name, ISvcLocator* pSvcLocator);

  // these are the functions inherited from Algorithm
  virtual StatusCode initialize () override;
  virtual StatusCode execute () override;
  virtual StatusCode finalize () override;

  ~BasicPerf () override;

private:
  // Configuration, and any other types of variables go here.
  std::vector< std::pair<float,float> > m_muBinning;

  /** output variables for the current event
   * \@{ */
  unsigned int m_runNumber = 0; ///< Run number
  unsigned long long m_eventNumber = 0; ///< Event number
  unsigned int m_mu = 0; ///< actual average interactions per bunch crossing

  //Truth particles
  std::vector<float> *m_truthEta = nullptr;
  std::vector<float> *m_truthPhi = nullptr;
  std::vector<float> *m_truthPt = nullptr;
  std::vector<float> *m_truthPt_lep = nullptr;
  std::vector<float> *m_truthE = nullptr;
  std::vector<float> *m_truthQoverP = nullptr;
  std::vector<int> *m_truthPDGID = nullptr;
  std::vector<int> *m_truthRecoIndex = nullptr;

  //Reconstructed particles (see https://gitlab.cern.ch/atlas/athena/blob/21.2/Event/xAOD/xAODTracking/xAODTracking/TrackingPrimitives.h)
  std::vector<float> *m_trackPt = nullptr;
  std::vector<int> *m_trackTruthIndex = nullptr;
  std::vector<float> *m_TruthMatchProb = nullptr;
  std::vector<int> *m_trackProperties = nullptr;
  std::vector<int> *m_trackPatternRecoInfo = nullptr;
  std::vector<int> *m_trackParticleHypothesis = nullptr;
  std::vector<float> *m_trackz0 = nullptr;
  std::vector<float> *m_trackd0 = nullptr;
  std::vector<int> *m_trackNSiHits = nullptr;
  std::vector<int> *m_trackMatchID = nullptr;

  std::vector<float> *m_truthVertex = nullptr;

  //Number of Events
  int Nevent;

  /** \@} */

  //Helper functions
  std::string getStrMuRange(float low, float high);
  int getMuBin(float mu);

};

#endif
