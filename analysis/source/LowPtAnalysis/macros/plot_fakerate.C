void plot_fakerate() {

  //TFile *f_mu0 = TFile::Open("fakes-inclWW-lowpt-01/hist-Inclusive_withflag_AODs.root");  
  TFile *f = TFile::Open("fakes-ttbar-lowpt-01/hist-ttbar_pileup_withflag_AODs.root");

  TProfile *h_num_mu_0 = (TProfile*)f->Get("Frac_reco_track_with_lowmatchprob_vs_track_pt_mu_0_10");  
  TProfile *h_num_mu_low = (TProfile*)f->Get("Frac_reco_track_with_lowmatchprob_vs_track_pt_mu_10_25");
  TProfile *h_num_mu_high = (TProfile*)f->Get("Frac_reco_track_with_lowmatchprob_vs_track_pt_mu_40_60");

  gStyle->SetOptStat(0);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetOptTitle(0);
  gStyle->SetTitleXOffset(1.4);
  gStyle->SetTitleYOffset(1.4);
  gStyle->SetTextSize(0.05);
  gStyle->SetLabelSize(0.05, "x");
  gStyle->SetTitleSize(0.05, "x");  
  gStyle->SetLabelSize(0.05, "y");
  gStyle->SetTitleSize(0.05, "y");
  gStyle->SetEndErrorSize(0.);
  TCanvas *c = new TCanvas();
  TLegend *l = new TLegend (0.7, 0.75, 0.9, 0.95);

  h_num_mu_high->SetXTitle("p_{T} (MeV)");
  h_num_mu_high->SetYTitle("Fraction of fake tracks");  
  h_num_mu_high->SetLineWidth(3);
  h_num_mu_high->SetLineColor(kRed+2);
  h_num_mu_high->SetLineStyle(0);
  h_num_mu_high->SetLineStyle(0);    

  h_num_mu_low->SetLineWidth(3);
  h_num_mu_low->SetLineColor(kGreen+8);
  h_num_mu_low->SetLineStyle(1);

  h_num_mu_0->SetLineWidth(3);
  h_num_mu_0->SetLineColor(kBlack);
  h_num_mu_0->SetLineStyle(2);
  

  h_num_mu_high->Draw("PE");
  h_num_mu_high->GetXaxis()->SetRangeUser(100., 500.); //focus on low-pT
  h_num_mu_high->GetYaxis()->SetRangeUser(0.0, 0.35); //focus on low-pT  
  h_num_mu_low->Draw("PE SAME");
  h_num_mu_0->Draw("PE SAME");  

  l->AddEntry(h_num_mu_0, "#mu = 0");
  l->AddEntry(h_num_mu_low, "10 < #mu < 25");  
  l->AddEntry(h_num_mu_high, "40 < #mu < 60");
  l->Draw();

  c->SaveAs("fakerate.png");
}
