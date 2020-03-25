#ifndef LowPtAnalysis_TruthAnalysis_H
#define LowPtAnalysis_TruthAnalysis_H

#include <AnaAlgorithm/AnaAlgorithm.h>

#include <vector>

#include <TTree.h>
#include <TH1F.h>
#include<TProfile2D.h>
#include <TRandom3.h>


/**
 * Truth-level sensitivity calculation.
 */
class TruthAnalysis : public EL::AnaAlgorithm
{
public:
  // this is a standard algorithm constructor
  TruthAnalysis (const std::string& name, ISvcLocator* pSvcLocator);

  // these are the functions inherited from Algorithm
  virtual StatusCode initialize () override;
  virtual StatusCode execute () override;
  virtual StatusCode finalize () override;

  ~TruthAnalysis () override;

  enum cuts {
    cut_nocut = 0,
    cut_trigger,
    cut_lep_ocof, //two opposite-charged, opposite-flavor leptons
    cut_lep_minpt,
    cut_m_ll, 
    cut_pt_ll, 
    cut_exclusive,
    ncuts
  };

  const std::vector<std::string> cuts_labels = {
    "All",
    "Trigger",
    "OCOF",
    "lep_pT",
    "m(ll)",
    "pT(ll)",
    "Excl."
  };

  const float MeV=1.0; //default units: MeV
  const float GeV=1e3*MeV; 

private:
  // selections
  float lep1_min_pt;
  float lep2_min_pt;
  float lep_max_eta;
  float dilep_min_mass;
  float dilep_min_pt;
  float tracks_min_pt; 
  float tracks_max_eta; 
  float tracks_max_n;
  std::string input_trk_eff_file;
  std::string input_trk_eff_pt_eta_file;
  bool filter_by_selections;
  unsigned int random_seed;

  // efficiency
  TProfile2D *h_trk_eff_pt_eta;
  TProfile2D *h_electron_eff;
  TProfile2D *h_muon_eff;
  //TH1F *h_trk_eff_pt;

  // output variables for the current event
  float m_dilep_pt;
  float m_dilep_m;

  //Leptons
  std::vector<float> *m_lep_pt = nullptr;
  std::vector<float> *m_lep_eta = nullptr;
  std::vector<float> *m_lep_phi = nullptr;
  std::vector<int> *m_lep_charge = nullptr;
  std::vector<int> *m_lep_pdgid = nullptr;
  float electron_eff;
  float muon_eff;

  //Fiducial "Tracks"  
  std::vector<float> *m_trk_pt = nullptr;
  std::vector<float> *m_trk_eta = nullptr;
  std::vector<float> *m_trk_phi = nullptr;
  std::vector<int> *m_trk_charge = nullptr;
  std::vector<int> *m_trk_pdgid = nullptr;
  std::vector<float> *m_weights = nullptr;

  //Event-level selections
  std::vector<char> *m_pass_sel;

  //Helper to save tree if necessary
  void saveTree();
  //Helper for cutflow
  void passCut(int cut);

  //iternal random generator
  TRandom3 *m_rnd;

};

#endif
