//Input histogram files to print out exclusive/inclusive yield and their ratio.
#include <iostream>
#include <fstream>


float lumi = 150e3; //pb^-1
float exclWW_xsec = 8.5e-3*0.319; //pb; fixed x-sec, sample gives 1.49e-2 x-sec, wrong!
float inclWW_xsec = 10.636; //pb
float exclWW_SD_DD_corr = 3.39; // correction factor for exclusive ww SD/DD contributions
float inclWW_filter_eff = 2.0132e-2;

void get_TruthAnalysis_Ratio(std::string p_f_exclWW, std::string p_f_inclWW, int min_pt)
{
  TFile *f_incl = TFile::Open(p_f_inclWW.c_str());
  TFile *f_excl = TFile::Open(p_f_exclWW.c_str());

  TH1F *h_incl =(TH1F*) f_incl->Get("sr_dilep_pt");
  TH1F *h_excl = (TH1F*)f_excl->Get("sr_dilep_pt");

  TH1F *h_incl_cutflow =(TH1F*)  f_incl->Get("cutflow");
  TH1F *h_excl_cutflow = (TH1F*)f_excl->Get("cutflow");

  float y_ngen_incl = h_incl_cutflow->GetBinContent(1);
  float y_ngen_excl = h_excl_cutflow->GetBinContent(1);
  //apply scalings
  h_incl->Sumw2();
  h_incl->Scale( inclWW_xsec*lumi / y_ngen_incl * inclWW_filter_eff);
  h_excl->Sumw2();
  h_excl->Scale( exclWW_xsec*lumi / y_ngen_excl );
  h_excl->Scale(exclWW_SD_DD_corr);

  float y_incl = h_incl->Integral(0,h_incl->GetNbinsX()+1);
  float y_excl = h_excl->Integral(0,h_excl->GetNbinsX()+1);
  std::cout<<"Exclusive Yield:"<<y_excl <<std::endl ; 
  std::cout<<"Inclusive Yield:"<< y_incl <<std::endl;
  std::cout<<"Ratio of Yields:"<< y_excl / y_incl <<std::endl;
  ofstream myfile;
  myfile.open ("Ratios.txt", ios::app );
  myfile << min_pt << " " << y_excl / y_incl << "\n";
  myfile.close();
}

  
