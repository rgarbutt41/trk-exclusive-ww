//Input root files of the DY, Ztautau, and Zmumu samples in order
#include <iostream>
#include <fstream>
#include <math.h>


//List of relevant constants
float lumi = 150e3; //pb^-1

float exclWW_xsec = 8.5434e-3; //pb; fixed x-sec, sample gives 1.49e-2 x-sec, wrong!
float exclWW_SD_DD_corr = 3.39; ///< correction factor for exclusive WW SD/DD contributions
float exclWW_filter_eff = 0.30838;

float inclWW_filter_eff = 0.026; 
float inclWW_xsec = 10.612; //pb
float inclWW_kfactor = 1.15;

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


void plot_mll(std::string DY_Sample, std::string Ztautau_Sample, std::string Zmumu_Sample)
{

  //Getting the proper histograms.
  TFile *f_DY = TFile::Open(DY_Sample.c_str());
  TFile *f_Ztautau = TFile::Open(Ztautau_Sample.c_str());
  TFile *f_Zmumu = TFile::Open(Zmumu_Sample.c_str());
  TH1F *h_DY =(TH1F*) f_DY->Get("dilep_m");
  TH1F *h_Ztautau = (TH1F*)f_Ztautau->Get("dilep_m");
  TH1F *h_Zmumu = (TH1F*)f_Zmumu->Get("dilep_m");

  TH1F *h_DY_cutflow =(TH1F*)  f_DY->Get("hCutFlow_Sum");
  TH1F *h_Ztautau_cutflow = (TH1F*) f_Ztautau->Get("hCutFlow_Sum");
  TH1F *h_Zmumu_cutflow = (TH1F*) f_Zmumu->Get("hCutFlow_Sum");

  float y_ngen_DY = h_DY_cutflow->GetBinContent(2);
  float y_ngen_Ztautau = h_Ztautau_cutflow->GetBinContent(2);
  float y_ngen_Zmumu = h_Zmumu_cutflow->GetBinContent(2);

  float DY_scale = DYmumu_xsec*lumi / y_ngen_DY *DYmumu_filter_eff*0.447;
  float Ztautau_scale =  Ztautau_xsec*lumi / y_ngen_Ztautau * Ztautau_filter_eff*0.447;
  float Zmumu_scale =  Zmumu_xsec*lumi / y_ngen_Zmumu * Zmumu_filter_eff*0.447;

  //apply scalings
  h_DY->Scale(DY_scale );
  h_Ztautau->Scale(Ztautau_scale);
  h_Zmumu->Scale(Zmumu_scale);

  //Draw options
  h_DY->SetFillColor(kRed);
  h_Zmumu->SetFillColor(kBlue);
  h_Ztautau->SetFillColor(kGreen);

  //Plotting
  THStack *hs = new THStack("hs","Drell-Yan Sample m(ll);m(ll) [GeV];Number of Scaled Events");
  h_DY->SetFillColor(kRed);
  h_Zmumu->SetFillColor(kBlue);
  h_Ztautau->SetFillColor(kGreen);
  hs->Add(h_DY);
  hs->Add(h_Ztautau);
  hs->Add(h_Zmumu);

  TCanvas *cs = new TCanvas("cs","cs");
  auto legend = new TLegend(0.1,0.7,0.48,0.9);
  legend->AddEntry(h_DY,"DYmumu");
  legend->AddEntry(h_Zmumu, "Zmumu");
  legend->AddEntry(h_Ztautau, "Ztautau");
  
  hs->Draw("HIST");
  legend->Draw();

  



  
}

  
