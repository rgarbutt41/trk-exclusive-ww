/** Plots the invariant masses of the DY samples.
 */
std::string plotName = "sr_dilep_m";
float lumi = 150e3;

float DYmumu_filter_eff = 0.0484;
float DYmumu_xsec = 32.11; //pb

float Zmumu_filter_eff = 0.04903;
float Zmumu_xsec = 1901; //pb

float LowMassDY_filter_eff = 0.000811;
float LowMassDY_xsec = 531379.2; //pb

void plot_DY_Mll( std::string p_DYmumu, std::string p_Zmumu, std::string p_LMDY )
{
  TFile *f_DYmumu = TFile::Open(p_DYmumu.c_str());
  TFile *f_Zmumu = TFile::Open(p_Zmumu.c_str());
  TFile *f_LMDY = TFile::Open(p_LMDY.c_str());

  TH1F *h_DYmumu = (TH1F*) f_DYmumu->Get("dilep_m");
  TH1F *h_Zmumu = (TH1F*) f_Zmumu->Get("dilep_m");
  TH1F *h_LMDY = (TH1F*) f_LMDY->Get("dilep_m");

  TH1F *h_DYmumu_cutflow = (TH1F*) f_DYmumu->Get("cutflow");
  TH1F *h_Zmumu_cutflow = (TH1F*) f_Zmumu->Get("cutflow");
  TH1F *h_LMDY_cutflow = (TH1F*) f_LMDY->Get("cutflow");

  float y_ngen_DYmumu = h_DYmumu_cutflow->GetBinContent(1);
  float y_ngen_Zmumu = h_Zmumu_cutflow->GetBinContent(1);
  float y_ngen_LMDY = h_LMDY_cutflow->GetBinContent(1);

  h_DYmumu->Sumw2();
  h_Zmumu->Sumw2();
  h_LMDY->Sumw2();
  h_DYmumu->Scale( DYmumu_xsec*lumi *DYmumu_filter_eff/ y_ngen_DYmumu);
  h_Zmumu->Scale(Zmumu_xsec*lumi * Zmumu_filter_eff / y_ngen_Zmumu);
  h_LMDY->Scale(LowMassDY_xsec*lumi * LowMassDY_filter_eff / y_ngen_LMDY);


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

  int color_Zmumu = kRed+0;
  int color_DYmumu = kBlack;
  int color_LMDY = kGreen+8;

  h_DYmumu->SetFillColor(color_DYmumu);
  h_DYmumu->SetFillStyle(0);
  h_DYmumu->SetLineColor(color_DYmumu);
  h_DYmumu->SetLineWidth(2);

  h_Zmumu->SetFillColor(color_Zmumu);
  h_Zmumu->SetFillStyle(0);
  h_Zmumu->SetLineColor(color_Zmumu);
  h_Zmumu->SetLineWidth(2);

  h_LMDY->SetFillColor(color_LMDY);
  h_LMDY->SetFillStyle(0);
  h_LMDY->SetLineColor(color_LMDY);
  h_LMDY->SetLineWidth(2);

  TCanvas *c = new TCanvas();
  TLegend *l = new TLegend(0.55,0.7,0.8,0.94);
  l->SetBorderSize(0);
  c->SetLogy();
  THStack *hs = new THStack("mass","");
  hs->SetTitle("DY m(ll)");
  hs->Add(h_DYmumu);
  l->AddEntry(h_DYmumu, "DYmumu", "f");
  hs->Add(h_Zmumu);
  l->AddEntry(h_Zmumu, "Zmumu", "f"); 
  hs->Add(h_LMDY);
  l->AddEntry(h_LMDY, "Low Mass DY", "f");
  hs->Draw("Hist");
  hs->GetXaxis()->SetTitle("m(ll) [GeV]");
  hs->GetYaxis()->SetTitle("Events / 5 GeV");
  l->Draw();
  c->Modified();

}
