#ifndef LowPtAnalysis_BasicPerf_H
#define LowPtAnalysis_BasicPerf_H

#include <AnaAlgorithm/AnaAlgorithm.h>

#include <TTree.h>
#include <TH1.h>
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
  //float m_cutValue;
  //TTree *m_myTree;
  //TH1 *m_myHist;

  /// output variables for the current event
  /// \{
  unsigned int m_runNumber = 0; ///< Run number
  unsigned long long m_eventNumber = 0; ///< Event number
  /// Jet 4-momentum variables

  std::vector<float> *m_truthEta = nullptr;
  std::vector<float> *m_truthPhi = nullptr;
  std::vector<float> *m_truthPt = nullptr;
  std::vector<float> *m_truthE = nullptr;
  std::vector<float> *m_truthQoverP = nullptr;
  std::vector<int> *m_truthPDGID = nullptr;


  std::vector<float> *m_trackPt = nullptr;

  TProfile* p_Reco_eff_vs_track_pt; //!

};

#endif
