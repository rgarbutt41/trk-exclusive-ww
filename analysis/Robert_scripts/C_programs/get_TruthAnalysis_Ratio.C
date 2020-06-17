//Input histogram files of respective sample type, prints ratios and adds ratios to a file called "Ratios.txt"
//Must input one of the following sample types: InclWW, DYmumu, Ztautau, Zmumu
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


void get_TruthAnalysis_Ratio(std::string p_f_exclWW, std::string p_f_inclWW, int min_pt, std::string Sample)
{
  float xsec;
  float filter_eff;

  //Getting the proper histograms.
  TFile *f_incl = TFile::Open(p_f_inclWW.c_str());
  TFile *f_excl = TFile::Open(p_f_exclWW.c_str());
  TH1F *h_incl =(TH1F*) f_incl->Get("Fakes");
  TH1F *h_excl = (TH1F*)f_excl->Get("Fakes");

  TH1F *h_incl_cutflow =(TH1F*)  f_incl->Get("hCutFlow_Sum");
  TH1F *h_excl_cutflow = (TH1F*)f_excl->Get("hCutFlow_Sum");

  TH1D *error_incl = (TH1D*) f_incl->Get("error_Fakes");
  TH1D *error_excl = (TH1D*) f_excl->Get("error_Fakes");

  float y_ngen_incl = h_incl_cutflow->GetBinContent(2);
  float y_ngen_excl = h_excl_cutflow->GetBinContent(2);

  float raw_incl = h_incl->GetBinContent(1);
  float raw_excl = h_excl->GetBinContent(1);

   //Retrieval of correct scaling factor for respective sample type
  if ( Sample ==  "DYmumu")
    {
      xsec = DYmumu_xsec;
      filter_eff = DYmumu_filter_eff;
    }
  else if ( Sample ==  "InclWW")
    {
      xsec = inclWW_xsec*inclWW_kfactor;
      filter_eff = inclWW_filter_eff; 
    }
  else if ( Sample == "Ztautau" )
    {
      xsec = Ztautau_xsec;
      filter_eff = Ztautau_filter_eff;
    }
  else if ( Sample == "Zmumu" )
    {
      xsec = Zmumu_xsec;
      filter_eff = Zmumu_filter_eff;
    }
  else if ( Sample == "LowMassDY" )
    {
      xsec = LowMassDY_xsec;
      filter_eff = LowMassDY_filter_eff;
    }

  float incl_scale = xsec*lumi / y_ngen_incl * filter_eff*0.447;
  float excl_scale =  exclWW_xsec*lumi / y_ngen_excl * exclWW_filter_eff*exclWW_SD_DD_corr;

  //apply scalings
  h_incl->Sumw2();
  h_incl->Scale( xsec*lumi / y_ngen_incl * filter_eff*inclWW_kfactor*0.447);
  h_excl->Sumw2();
  h_excl->Scale( exclWW_xsec*lumi / y_ngen_excl * exclWW_filter_eff );
  h_excl->Scale(exclWW_SD_DD_corr);

  float y_incl = h_incl->GetBinContent(1);
  float y_excl = h_excl->GetBinContent(1);

  //std::cout<<"Exclusive Raw Yield:"<<y_raw_excl <<std::endl ; 
  //std::cout<<"Background Raw Yield:"<< y_raw_incl <<std::endl;
  //std::cout<<"Ratio of Yields:"<< y_excl / y_incl <<std::endl;
  //std::cout<<"Inclusive Yield:" << y_incl << std::endl;
  //std::cout << "Exclusive Yield:" <<y_excl << std::endl;
  ofstream myfile;
  myfile.open ("Ratios.txt", ios::app );
  myfile << y_incl  << " " << y_excl / y_incl << " " << y_excl <<"\n";
  //myfile << y_incl << "\n";
  //myfile << y_excl << "\n";
  myfile.close();


  //Calculating Errors

  TH1F *excl_track_hist = (TH1F*)f_excl->Get("num_fiducial_tracks");
  TH1F *incl_track_hist = (TH1F*)f_incl->Get("num_fiducial_tracks");
  float incl_tracks = 0;
  float excl_tracks = 0;
  float P_average = 0.71427198;
  
  for(int i=1;(i < 51);i++)
    {
      incl_tracks += incl_track_hist->GetBinContent(i)*pow(1-P_average,2*i);
      excl_tracks += excl_track_hist->GetBinContent(i)*pow(1-P_average,2*i);
    }

  float incl_sys_error = sqrt(incl_tracks)*incl_scale;
  float excl_sys_error = sqrt(excl_tracks)*excl_scale;
  float incl_error = sqrt(error_incl->GetBinContent(1))*incl_scale;
  float excl_error = sqrt(error_excl->GetBinContent(1))*excl_scale;
  std::cout << incl_scale << "\n";
  ofstream errorfile;
  errorfile.open ("Errors.txt", ios::app );
  errorfile << incl_error << " " << incl_sys_error  << " " << excl_error << " " << excl_sys_error << "\n";
  errorfile.close();
}

  
