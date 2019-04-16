/** Plotting routine for truth-analysis for low-pT tracking study
 * Input histogram files for exclusive WW and inclusive WW.
 * Macro does:
 * - scale for x-section, lumi
 * - correct for SD/DD contribution of excl_WW
 * - produce plots in signal-region (cuts already applied in input files)
 * - print table of expected yields and S/B numbers
 */

//Settings
std::string plotName = "sr_dilep_pt"; ///< name of histogram to plot
float lumi = 150e3; //pb^-1
float exclWW_xsec = 8.5e-3*0.319; //pb; fixed x-sec, sample gives 1.49e-2 x-sec, wrong!
float inclWW_xsec = 10.636; //pb
float exclWW_SD_DD_corr = 3.39; ///< correction factor for exclusive WW SD/DD contributions
std::string label="Min. track p_{T} = 100 MeV";

void plot_TruthAnalysis_SBStudy_Hist(TH1F *h_excl, TH1F *h_incl,TH1F *h_excl_cutflow, TH1F *h_incl_cutflow);


// Plot pT(e-mu) for single configuration
void plot_TruthAnalysis_SBStudy(std::string p_f_exclWW, std::string p_f_inclWW)
{
 
  TFile *f_incl = TFile::Open(p_f_inclWW.c_str());
  TFile *f_excl = TFile::Open(p_f_exclWW.c_str());

  TH1F *h_incl = (TH1F*) f_incl->Get("sr_dilep_pt");
  TH1F *h_excl = (TH1F*) f_excl->Get("sr_dilep_pt");

  TH1F *h_incl_cutflow = (TH1F*) f_incl->Get("cutflow");
  TH1F *h_excl_cutflow = (TH1F*) f_excl->Get("cutflow");

  plot_TruthAnalysis_SBStudy_Hist(h_excl, h_incl, h_excl_cutflow, h_incl_cutflow);

};

// Plot pT(e-mu) mixing up two configurations
void plot_TruthAnalysis_Run2(std::string p_f_exclWW_500, std::string p_f_inclWW_500,
			     std::string p_f_exclWW_400, std::string p_f_inclWW_400)
{
  label = "Run 2 min. track p_{T}";
  
  TFile *f_incl_500 = TFile::Open(p_f_inclWW_500.c_str());
  TFile *f_excl_500 = TFile::Open(p_f_exclWW_500.c_str());

  TH1F *h_incl_500 = (TH1F*) f_incl_500->Get("sr_dilep_pt");
  TH1F *h_excl_500 = (TH1F*) f_excl_500->Get("sr_dilep_pt");

  TH1F *h_incl_cutflow_500 = (TH1F*) f_incl_500->Get("cutflow");
  TH1F *h_excl_cutflow_500 = (TH1F*) f_excl_500->Get("cutflow");

  TFile *f_incl_400 = TFile::Open(p_f_inclWW_400.c_str());
  TFile *f_excl_400 = TFile::Open(p_f_exclWW_400.c_str());

  TH1F *h_incl_400 = (TH1F*) f_incl_400->Get("sr_dilep_pt");
  TH1F *h_excl_400 = (TH1F*) f_excl_400->Get("sr_dilep_pt");

  TH1F *h_incl_cutflow_400 = (TH1F*) f_incl_400->Get("cutflow");
  TH1F *h_excl_cutflow_400 = (TH1F*) f_excl_400->Get("cutflow");

  const float mix_weight = 1.0; //ratio of luminosities

  TH1F *h_incl = (TH1F*)h_incl_500->Clone();
  h_incl->Add(h_incl_400, mix_weight); //same weight
  TH1F *h_excl = (TH1F*)h_excl_500->Clone();
  h_excl->Add(h_excl_400, mix_weight); //same weight

  TH1F *h_incl_cutflow = (TH1F*)h_incl_cutflow_500->Clone();
  h_incl_cutflow->Add(h_incl_cutflow_400, mix_weight); //same weight
  TH1F *h_excl_cutflow = (TH1F*)h_excl_cutflow_500->Clone();
  h_excl_cutflow->Add(h_excl_cutflow_400, mix_weight); //same weight

  plot_TruthAnalysis_SBStudy_Hist(h_excl, h_incl, h_excl_cutflow, h_incl_cutflow);
}

// Work-horse function to make the plot
void plot_TruthAnalysis_SBStudy_Hist(TH1F *h_excl, TH1F *h_incl,
				     TH1F *h_excl_cutflow, TH1F *h_incl_cutflow)
{   
  float y_raw_incl = h_incl->GetEntries();
  float y_raw_excl = h_excl->GetEntries();

  float y_ngen_incl = h_incl_cutflow->GetBinContent(1);
  float y_ngen_excl = h_excl_cutflow->GetBinContent(1);

  float eff_sel_incl = h_incl_cutflow->GetBinContent(6) / y_ngen_incl;
  float eff_sel_excl = h_excl_cutflow->GetBinContent(6) / y_ngen_excl;  
  
  float eff_exclusivity_incl = h_incl_cutflow->GetBinContent(7) / h_incl_cutflow->GetBinContent(6);
  float eff_exclusivity_excl = h_excl_cutflow->GetBinContent(7) / h_excl_cutflow->GetBinContent(6);  

  //apply scalings
  h_incl->Sumw2();
  h_incl->Scale( inclWW_xsec*lumi / y_ngen_incl );
  h_excl->Sumw2();
  h_excl->Scale( exclWW_xsec*lumi / y_ngen_excl );
  h_excl->Scale(exclWW_SD_DD_corr);

  //calculate yields
  float y_incl = h_incl->Integral(0, h_incl->GetNbinsX()+1);
  float y_excl = h_excl->Integral(0, h_excl->GetNbinsX()+1);

  //apply style
  gStyle->SetPaperSize(20,26);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadBottomMargin(0.16);
  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetTitleXOffset(1.4);
  gStyle->SetTitleYOffset(1.4);  
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
  gROOT->ForceStyle();

  int color_incl = kGreen+8;
  int color_excl = kBlue+2;

  h_incl->SetFillColor(color_incl);
  h_incl->SetFillStyle(1001);
  h_incl->SetLineColor(color_incl);
  h_incl->SetLineWidth(2);

  h_excl->SetFillColor(color_excl);
  h_excl->SetFillStyle(3244);
  h_excl->SetLineColor(color_excl);
  h_excl->SetLineWidth(2);  

  //draw plot canvas
  TCanvas *c = new TCanvas();
  TLegend *l = new TLegend (0.55, 0.7, 0.80, 0.94);
  l->SetBorderSize(0);
  THStack *hs = new THStack("sr_pt_emu", "");
  hs->Add(h_incl);
  l->AddEntry(h_incl, "Inclusive WW", "f");
  hs->Add(h_excl);
  l->AddEntry(h_excl, "Exclusive WW", "f");
  hs->Draw("HIST");
  hs->GetXaxis()->SetTitle("p_{T}(e#mu) [GeV]");
  hs->GetXaxis()->SetRangeUser(30.0, 150.0);
  hs->GetYaxis()->SetTitle("Events / 5 GeV");  
  TLatex tex;  
  tex.DrawLatexNDC(0.55, 0.65, "Truth-level study");
  tex.DrawLatexNDC(0.55, 0.59, "#sqrt{s} = 13 TeV, L = 150 fb^{-1}");      
  tex.DrawLatexNDC(0.55, 0.52, label.c_str());
  l->Draw();
  c->Modified();
  c->SaveAs("sr_pt_emu.png");

  //print table with yields
  std::cout << "Raw yields:" << std::endl;
  std::cout << "Excl WW: " << y_raw_excl << std::endl;
  std::cout << "Incl WW: " << y_raw_incl << std::endl;  
  std::cout << "Yields for L = " << lumi/1000. << " / fb" <<  std::endl;
  std::cout << "|          | Excl. WW | Incl. WW | Ratio |" << std::endl;
  std::cout << "|----------|----------|----------|-----|" << std::endl; 
  std::cout << "| Yield | " << y_excl << " | " << y_incl << " | " << y_excl / y_incl << " | " << std::endl;
  std::cout << "| Eff   | " << y_raw_excl / y_ngen_excl *100 << "% | " << y_raw_incl / y_ngen_incl << "% | " << std::endl;
  std::cout << "| Eff-sel | " << eff_sel_excl << " | " << eff_sel_incl << " | " << eff_sel_excl / eff_sel_incl << "| "  << std::endl;
  std::cout << "| Eff-exclus. | " << eff_exclusivity_excl << " | " << eff_exclusivity_incl << " | " << eff_exclusivity_excl / eff_exclusivity_incl << "| "  << std::endl;  
  std::cout << std::endl;										      

}
