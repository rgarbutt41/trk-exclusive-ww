#include "AtlasStyle.C"

void plot_basicPerf_AtlasStyle()
{
  // Set ATLAS style 
  SetAtlasStyle();

  gStyle->SetErrorX(0.5);

  gROOT->ForceStyle();

  TCanvas* canvas   = new TCanvas("c", "c",0,0,800,600);

  //TFile *data = TFile::Open("../../../data_LowPtRoI_ZFilterRoI_test1000_CorrBin_CorrLepPt_RoIOff_test0/hist-jo_RoIOff.root");
  //TFile *data = TFile::Open("../../../data_LowPtRoI_ZFilterRoITruth_test1000_CorrBin_CorrLepPt_test12/hist-LowPtRoI_SiSeed_ATLxk_ZFilterRoITruth_Events1000.root");
  //TFile *data = TFile::Open("../../../data_LowPtRoI_ZFilterRoITruth_test1000_CorrBin_CorrLepPt_TrackSel_Test6/hist-LowPtRoI_SiSeed_ATLxk_ZFilterRoITruth_Events1000_TrackSel_Test6.root");
  //TFile *data = TFile::Open("../../../data_LowPtRoI_ZFilterRoITruth_test1000_CorrBin_CorrLepPt_MuBin:40_60/hist-LowPtRoI_SiSeed_ATLxk_ZFilterRoITruth_Events1000.root");
  //TFile *data = TFile::Open("../../../data_LowPtRoI_ZFilterRoITruth_test1000_inclusiveWW/hist-LowPtRoI_SiSeed_ATLxk_ZFilterRoITruth_Events1000_Test0.root");
  TFile *data = TFile::Open("../../../data_LowPtRoI_ZFilterRoITruth_test1000_ttbar/hist-LowPtRoI_SiSeed_ATLxk_ZFilterRoITruth_Events1000.root");

  TProfile *histoE = (TProfile*)data->Get("Reco_eff_vs_track_pt");
  histoE->GetXaxis()->SetTitle("Truth Particle p_{T} [MeV]");
  histoE->GetYaxis()->SetTitle("Reco Eff");
  histoE->GetXaxis()->SetRangeUser(0,1000);
  histoE->GetYaxis()->SetRangeUser(0,1);
  histoE->Draw("P");

  canvas->SaveAs("RecoEff_ttbar_ZFilterRoITruth.pdf");

  TProfile *histoF = (TProfile*)data->Get("Frac_reco_track_with_lowmatchprob_vs_track_pt");
  histoF->GetXaxis()->SetTitle("Track Particle p_{T} [MeV]");
  histoF->GetYaxis()->SetTitle("Fake rate");
  histoF->GetXaxis()->SetRangeUser(0,1000);
  histoF->GetYaxis()->SetNdivisions(510, kTRUE);
  histoF->Draw("P");

  canvas->SaveAs("FakeRate_ttbar_ZFilterRoITruth.pdf");

  TH1F *histo1 = (TH1F*)data->Get("AverageN_reco_tracks_lowmatchprob_vs_track_pt");
  histo1->GetXaxis()->SetTitle("Track Particle p_{T} [MeV]");
  histo1->GetYaxis()->SetTitle("Average Number of Tracks in z_{0} window");
  histo1->GetXaxis()->SetRangeUser(0,1000);
  histo1->GetYaxis()->SetNdivisions(510, kTRUE);
  histo1->Draw("P");

  canvas->SaveAs("AverageTracks_Fake.pdf");

  TH1F *histo2 = (TH1F*)data->Get("AverageN_reco_tracks_goodprob_vs_track_pt");
  histo2->GetXaxis()->SetTitle("Track Particle p_{T} [MeV]");
  histo2->GetYaxis()->SetTitle("Average Number of Tracks in z_{0} window");
  histo2->GetXaxis()->SetRangeUser(0,1000);
  histo2->GetYaxis()->SetNdivisions(510, kTRUE);
  histo2->Draw("P");

  canvas->SaveAs("AverageTracks_Good.pdf");


//Num_reco_track_with_goodprob_and_foundtruth_vs_abs_d0_zoom
//Num_reco_track_with_lowmatchprob_vs_abs_d0_zoom
//Num_reco_track_with_goodprob_and_foundtruth_vs_numSiHits
//Num_reco_track_with_lowmatchprob_vs_numSiHits

  TH1F *histo3 = (TH1F*)data->Get("Num_reco_track_with_goodmatchprob_vs_abs_d0_zoom");//("Num_reco_track_with_goodprob_and_foundtruth_vs_abs_d0_zoom");
  histo3->GetXaxis()->SetTitle("d_{0} [mm]");
  histo3->GetXaxis()->SetRangeUser(0,20);
  histo3->SetLineColor(kBlue);
  histo3->SetMarkerColor(kBlue);
  histo3->Draw("P");
  TH1F *histo4 = (TH1F*)data->Get("Num_reco_track_with_lowmatchprob_vs_abs_d0_zoom");
  histo4->GetXaxis()->SetTitle("d_{0} [mm]");
  histo4->GetXaxis()->SetRangeUser(0,20);
  histo4->SetLineColor(kRed);
  histo4->SetMarkerColor(kRed);
  histo4->Draw("P sames");

  canvas->SaveAs("Distribution_d0_Separation.pdf");


  TH1F *histo5 = (TH1F*)data->Get("Num_reco_track_with_goodmatchprob_vs_numSiHits");//("Num_reco_track_with_goodprob_and_foundtruth_vs_numSiHits");
  histo5->GetXaxis()->SetTitle("Number of Si Hits");
  histo5->GetXaxis()->SetRangeUser(0,20);
  histo5->SetLineColor(kBlue);
  histo5->SetMarkerColor(kBlue);
  histo5->Draw("hist");
  TH1F *histo6 = (TH1F*)data->Get("Num_reco_track_with_lowmatchprob_vs_numSiHits");
  histo6->GetXaxis()->SetTitle("Number of Si Hits");
  histo6->GetXaxis()->SetRangeUser(0,20);
  histo6->SetLineColor(kRed);
  histo6->SetMarkerColor(kRed);
  histo6->Draw("hist sames");

  canvas->SaveAs("Distribution_SiHits_Separation.pdf");



}



