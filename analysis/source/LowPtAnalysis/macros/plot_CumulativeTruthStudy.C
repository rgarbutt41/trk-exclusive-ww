/** Plotting routine for truth-analysis for low-pT tracking study
 * Input histogram files for exclusive WW and background.
 * Macro does:
 * - scale for x-section, lumi
 * - correct for SD/DD contribution of excl_WW
 * - produce plots in signal-region (cuts already applied in input files)
 * - print table of expected yields and S/B numbers
 */

//Settings
std::string plotName = "sr_dilep_pt"; ///< name of histogram to plot
float lumi = 150e3; //pb^-1
const char* pT="Min. track p_{T} = 200 MeV";

float exclWW_xsec = 8.5e-3*0.319; //pb; fixed x-sec, sample gives 1.49e-2 x-sec, wrong!
float exclWW_SD_DD_corr = 3.39; ///< correction factor for exclusive WW SD/DD contributions

float inclWW_filter_eff = 2.0132e-2; 
float inclWW_xsec = 10.636; //pb

float DYmumu_filter_eff = 0.0484;
float DYmumu_xsec = 32.11; //pb

float DYee_filter_eff = 0.043;
float DYee_xsec = 7.921; //pb

float Zmumu_filter_eff = 0.04903;
float Zmumu_xsec = 1901; //pb

float Ztautau_filter_eff = 0.000319;
float Ztautau_xsec = 1901; //pb

float LowMassDY_filter_eff = 0.000811;
float LowMassDY_xsec = 531379.2; //pb


void plot_TruthAnalysis_SBStudy_Hist(TH1F *h_excl, TH1F *h_incl,TH1F *h_excl_cutflow, TH1F *h_incl_cutflow, TH1F *h_Ztautau, TH1F *h_Ztautau_cutflow, TH1F *h_DYmumu, TH1F *h_DYmumu_cutflow );


// Plot pT(e-mu) for single configuration
void plot_CumulativeTruthStudy(std::string p_f_exclWW, std::string p_f_inclWW, std::string p_f_Ztautau, std::string p_f_DYmumu) // For simplicity keeping p_f_inclWW as the name, so I don't have to rename everything. Let Zmumu be inclWW.
{
 
  TFile *f_incl = TFile::Open(p_f_inclWW.c_str());
  TFile *f_excl = TFile::Open(p_f_exclWW.c_str());
  TFile *f_Ztautau = TFile::Open(p_f_Ztautau.c_str());
  TFile *f_DYmumu = TFile::Open(p_f_DYmumu.c_str());

  TH1F *h_incl = (TH1F*) f_incl->Get("sr_dilep_pt");
  TH1F *h_excl = (TH1F*) f_excl->Get("sr_dilep_pt");
  TH1F *h_Ztautau = (TH1F*) f_Ztautau->Get("sr_dilep_pt");
  TH1F *h_DYmumu = (TH1F*) f_DYmumu->Get("sr_dilep_pt");

  TH1F *h_incl_cutflow = (TH1F*) f_incl->Get("cutflow");
  TH1F *h_excl_cutflow = (TH1F*) f_excl->Get("cutflow");
  TH1F *h_Ztautau_cutflow = (TH1F*) f_Ztautau->Get("cutflow");
  TH1F *h_DYmumu_cutflow = (TH1F*) f_DYmumu->Get("cutflow");

  plot_TruthAnalysis_SBStudy_Hist(h_excl, h_incl, h_excl_cutflow, h_incl_cutflow, h_Ztautau, h_Ztautau_cutflow, h_DYmumu, h_DYmumu_cutflow );

};

// Work-horse function to make the plot
void plot_TruthAnalysis_SBStudy_Hist(TH1F *h_excl, TH1F *h_incl,TH1F *h_excl_cutflow, TH1F *h_incl_cutflow, TH1F *h_Ztautau, TH1F *h_Ztautau_cutflow, TH1F *h_DYmumu, TH1F *h_DYmumu_cutflow  )
{   
  
  float y_raw_incl = h_incl->GetEntries();
  float y_raw_excl = h_excl->GetEntries();
  float y_raw_Ztautau = h_Ztautau->GetEntries();
  float y_raw_DYmumu = h_DYmumu->GetEntries();

  float y_ngen_incl = h_incl_cutflow->GetBinContent(1);
  float y_ngen_excl = h_excl_cutflow->GetBinContent(1);
  float y_ngen_Ztautau = h_Ztautau_cutflow->GetBinContent(1);
  float y_ngen_DYmumu = h_DYmumu_cutflow->GetBinContent(1);

  float eff_sel_incl = h_incl_cutflow->GetBinContent(6) / y_ngen_incl;
  float eff_sel_excl = h_excl_cutflow->GetBinContent(6) / y_ngen_excl;  
  float eff_sel_Ztautau = h_Ztautau_cutflow->GetBinContent(6) / y_ngen_Ztautau;
  float eff_sel_DYmumu = h_DYmumu_cutflow->GetBinContent(6) / y_ngen_DYmumu;  
  
  float eff_exclusivity_incl = h_incl_cutflow->GetBinContent(7) / h_incl_cutflow->GetBinContent(6);
  float eff_exclusivity_excl = h_excl_cutflow->GetBinContent(7) / h_excl_cutflow->GetBinContent(6);  
  float eff_exclusivity_Ztautau = h_Ztautau_cutflow->GetBinContent(7) / h_Ztautau_cutflow->GetBinContent(6);
  float eff_exclusivity_DYmumu = h_DYmumu_cutflow->GetBinContent(7) / h_DYmumu_cutflow->GetBinContent(6);  

  //apply scalings
  h_incl->Sumw2();
  h_incl->Scale(Zmumu_xsec*lumi / y_ngen_incl * Zmumu_filter_eff);
  h_excl->Sumw2();
  h_excl->Scale( exclWW_xsec*lumi / y_ngen_excl );
  h_excl->Scale( exclWW_SD_DD_corr );
  h_Ztautau->Sumw2();
  h_Ztautau->Scale( Ztautau_xsec*lumi / y_ngen_Ztautau* Ztautau_filter_eff );
  h_DYmumu->Sumw2();
  h_DYmumu->Scale( DYmumu_xsec*lumi / y_ngen_DYmumu* DYmumu_filter_eff );


  //calculate yields
  float y_incl = h_incl->Integral(0, h_incl->GetNbinsX()+1);
  float y_excl = h_excl->Integral(0, h_excl->GetNbinsX()+1);
  float y_Ztautau = h_Ztautau->Integral(0,h_Ztautau->GetNbinsX()+1);
  float y_DYmumu = h_DYmumu->Integral(0,h_DYmumu->GetNbinsX()+1);

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
  int color_Ztautau = kRed+0;
  int color_DYmumu = kBlack;

  h_incl->SetFillColor(color_incl);
  h_incl->SetFillStyle(1001);
  h_incl->SetLineColor(color_incl);
  h_incl->SetLineWidth(2);

  h_excl->SetFillColor(color_excl);
  h_excl->SetFillStyle(3244);
  h_excl->SetLineColor(color_excl);
  h_excl->SetLineWidth(2);  

  h_Ztautau->SetFillColor(color_Ztautau);
  h_Ztautau->SetFillStyle(3490);
  h_Ztautau->SetLineColor(color_Ztautau);
  h_Ztautau->SetLineWidth(2);  

  h_DYmumu->SetFillColor(color_DYmumu);
  h_DYmumu->SetFillStyle(3353);
  h_DYmumu->SetLineColor(color_DYmumu);
  h_DYmumu->SetLineWidth(2);  

  //draw plot canvas
  TCanvas *c = new TCanvas();
  TLegend *l = new TLegend (0.55, 0.7, 0.80, 0.94);
  l->SetBorderSize(0);
  THStack *hs = new THStack("sr_pt", "");
  hs->Add(h_incl);
  l->AddEntry(h_incl, "Zmumu" , "f");
  hs->Add(h_DYmumu);
  l->AddEntry(h_DYmumu, "DYmumu", "f");
  hs->Add(h_Ztautau);
  l->AddEntry(h_Ztautau, "Ztautau", "f");  
  hs->Add(h_excl);
  l->AddEntry(h_excl, "Exclusive WW" , "f");
  hs->Draw("HIST");
  hs->GetXaxis()->SetTitle("p_{T} [GeV]");
  hs->GetXaxis()->SetRangeUser(30.0, 150.0);
  hs->GetYaxis()->SetTitle("Events / 5 GeV");  
  TLatex tex;  
  tex.DrawLatexNDC(0.55, 0.65, "Truth-level study");
  tex.DrawLatexNDC(0.55, 0.59, "#sqrt{s} = 13 TeV, L = 150 fb^{-1}");      
  tex.DrawLatexNDC(0.55, 0.52, pT);
  l->Draw();
  c->Modified();
  c->SaveAs("sr_pt.png");

  //print table with yields
  std::cout << "Raw yields:" << std::endl;
  std::cout << "Excl WW: " << y_raw_excl << std::endl;
  std::cout << "Background" << ": " << y_raw_incl + y_raw_DYmumu + y_raw_Ztautau << std::endl;  
  std::cout << "Yields for L = " << lumi/1000. << " / fb" <<  std::endl;
  std::cout << "|          | Excl. WW | Background  | Ratio |" << std::endl;
  std::cout << "|----------|----------|-----------|-----|" << std::endl; 
  std::cout << "| Yield | " << y_excl << " | " << y_incl + y_Ztautau + y_DYmumu << " | " << y_excl / (y_incl + y_Ztautau + y_DYmumu ) << " | " << std::endl;
  std::cout << "| Eff   | " << y_raw_excl / y_ngen_excl *100 << "% | " << (y_raw_incl+y_raw_Ztautau + y_raw_DYmumu) /( y_ngen_incl+y_ngen_DYmumu + y_ngen_Ztautau ) << "% | " << std::endl;
  std::cout << "| Eff-sel | " << eff_sel_excl << " | " << eff_sel_incl + eff_sel_Ztautau + eff_sel_DYmumu << " | " << eff_sel_excl / (eff_sel_incl+ eff_sel_DYmumu + eff_sel_Ztautau) << "| "  << std::endl;
  std::cout << "| Eff-exclus. | " << eff_exclusivity_excl << " | " << eff_exclusivity_incl + eff_exclusivity_DYmumu + eff_exclusivity_Ztautau << " | " << eff_exclusivity_excl /( eff_exclusivity_incl + eff_exclusivity_DYmumu + eff_exclusivity_Ztautau)<< "| "  << std::endl;  
  std::cout << std::endl;										      

}
