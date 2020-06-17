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


void get_opp_flavor_expected_yield(std::string p_f_exclWW, std::string p_f_inclWW, std::string Sample)
{
  float xsec;
  float filter_eff;

  //Getting the proper histograms.
  TFile *f_incl = TFile::Open(p_f_inclWW.c_str());
  TFile *f_excl = TFile::Open(p_f_exclWW.c_str());

  TH1F *h_incl_cutflow =(TH1F*)  f_incl->Get("hCutFlow_Sum");
  TH1F *h_excl_cutflow = (TH1F*)f_excl->Get("hCutFlow_Sum");

  float y_ngen_incl = h_incl_cutflow->GetBinContent(2);
  float y_ngen_excl = h_excl_cutflow->GetBinContent(2);

  TH1F *h_incl_base =(TH1F*) f_incl->Get("Base_Case");
  TH1F *h_excl_base = (TH1F*)f_excl->Get("Base_Case");

  TH1D *error_incl_base = (TH1D*) f_incl->Get("error_Base_Case");
  TH1D *error_excl_base = (TH1D*) f_excl->Get("error_Base_Case");

  TH1F *h_incl_tracks =(TH1F*) f_incl->Get("Tracks");
  TH1F *h_excl_tracks = (TH1F*)f_excl->Get("Tracks");

  TH1D *error_incl_tracks = (TH1D*) f_incl->Get("error_Tracks");
  TH1D *error_excl_tracks = (TH1D*) f_excl->Get("error_Tracks");

  TH1F *h_incl_lepton =(TH1F*) f_incl->Get("Lepton_Reco");
  TH1F *h_excl_lepton = (TH1F*)f_excl->Get("Lepton_Reco");

  TH1D *error_incl_lepton = (TH1D*) f_incl->Get("error_Lepton_Reco");
  TH1D *error_excl_lepton = (TH1D*) f_excl->Get("error_Lepton_Reco");

  TH1F *h_incl_pileup =(TH1F*) f_incl->Get("Pileup");
  TH1F *h_excl_pileup = (TH1F*)f_excl->Get("Pileup");

  TH1D *error_incl_pileup = (TH1D*) f_incl->Get("error_Pileup");
  TH1D *error_excl_pileup = (TH1D*) f_excl->Get("error_Pileup");

  TH1F *h_incl_fakes =(TH1F*) f_incl->Get("Fakes");
  TH1F *h_excl_fakes = (TH1F*)f_excl->Get("Fakes");

  TH1D *error_incl_fakes = (TH1D*) f_incl->Get("error_Fakes");
  TH1D *error_excl_fakes = (TH1D*) f_excl->Get("error_Fakes");

  TH1F *h_incl_halfmm_pileup =(TH1F*) f_incl->Get("halfmm_Pileup");
  TH1F *h_excl_halfmm_pileup = (TH1F*)f_excl->Get("halfmm_Pileup");

  TH1D *error_incl_halfmm_pileup = (TH1D*) f_incl->Get("error_halfmm_Pileup");
  TH1D *error_excl_halfmm_pileup = (TH1D*) f_excl->Get("error_halfmm_Pileup");

  TH1F *h_incl_halfmm_fakes =(TH1F*) f_incl->Get("halfmm_Fakes");
  TH1F *h_excl_halfmm_fakes = (TH1F*)f_excl->Get("halfmm_Fakes");

  TH1D *error_incl_halfmm_fakes = (TH1D*) f_incl->Get("error_halfmm_Fakes");
  TH1D *error_excl_halfmm_fakes = (TH1D*) f_excl->Get("error_halfmm_Fakes");


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

  float incl_scale = xsec*lumi / y_ngen_incl * filter_eff;
  float excl_scale =  exclWW_xsec*lumi / y_ngen_excl * exclWW_filter_eff*exclWW_SD_DD_corr;

  float y_incl_base = h_incl_base->GetBinContent(1)*incl_scale;
  float y_excl_base = h_excl_base->GetBinContent(1)*excl_scale;

  float y_incl_tracks = h_incl_tracks->GetBinContent(1)*incl_scale;
  float y_excl_tracks = h_excl_tracks->GetBinContent(1)*excl_scale;

  float y_incl_lepton = h_incl_lepton->GetBinContent(1)*incl_scale;
  float y_excl_lepton = h_excl_lepton->GetBinContent(1)*excl_scale;

  float y_incl_pileup = h_incl_pileup->GetBinContent(1)*incl_scale;
  float y_excl_pileup = h_excl_pileup->GetBinContent(1)*excl_scale;

  float y_incl_fakes = h_incl_fakes->GetBinContent(1)*incl_scale;
  float y_excl_fakes = h_excl_fakes->GetBinContent(1)*excl_scale;

  float y_incl_halfmm_pileup = h_incl_halfmm_pileup->GetBinContent(1)*incl_scale;
  float y_excl_halfmm_pileup = h_excl_halfmm_pileup->GetBinContent(1)*excl_scale;

  float y_incl_halfmm_fakes = h_incl_halfmm_fakes->GetBinContent(1)*incl_scale;
  float y_excl_halfmm_fakes = h_excl_halfmm_fakes->GetBinContent(1)*excl_scale;


  ofstream myfile;
  myfile.open ("Ratios.txt", ios::app );
  myfile << y_incl_base  << " " << y_incl_tracks  << " " << y_incl_lepton  << " " << y_incl_pileup  << " " << y_incl_fakes  << " " << y_incl_halfmm_pileup  << " " << y_incl_halfmm_fakes  << " " <<y_excl_base  << " " << y_excl_tracks  << " " << y_excl_lepton  << " " << y_excl_pileup  << " " << y_excl_fakes  << " " << y_excl_halfmm_pileup  << " " << y_excl_halfmm_fakes  <<"\n";
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
  

  float e_incl_base = sqrt(error_incl_base->GetBinContent(1))*incl_scale;
  float e_excl_base = sqrt(error_excl_base->GetBinContent(1))*excl_scale;

  float e_incl_tracks = sqrt(error_incl_tracks->GetBinContent(1))*incl_scale;
  float e_excl_tracks = sqrt(error_excl_tracks->GetBinContent(1))*excl_scale;

  float e_incl_lepton = sqrt(error_incl_lepton->GetBinContent(1))*incl_scale;
  float e_excl_lepton = sqrt(error_excl_lepton->GetBinContent(1))*excl_scale;

  float e_incl_pileup = sqrt(error_incl_pileup->GetBinContent(1))*incl_scale;
  float e_excl_pileup = sqrt(error_excl_pileup->GetBinContent(1))*excl_scale;

  float e_incl_fakes = sqrt(error_incl_fakes->GetBinContent(1))*incl_scale;
  float e_excl_fakes = sqrt(error_excl_fakes->GetBinContent(1))*excl_scale;

  float e_incl_halfmm_pileup = sqrt(error_incl_halfmm_pileup->GetBinContent(1))*incl_scale;
  float e_excl_halfmm_pileup = sqrt(error_excl_halfmm_pileup->GetBinContent(1))*excl_scale;

  float e_incl_halfmm_fakes = sqrt(error_incl_halfmm_fakes->GetBinContent(1))*incl_scale;
  float e_excl_halfmm_fakes = sqrt(error_excl_halfmm_fakes->GetBinContent(1))*excl_scale;

  ofstream errorfile;
  errorfile.open ("Errors.txt", ios::app );
  errorfile << e_incl_base  << " " << e_incl_tracks  << " " << e_incl_lepton  << " " << e_incl_pileup  << " " << e_incl_fakes  << " " << e_incl_halfmm_pileup  << " " << e_incl_halfmm_fakes  << " " << incl_sys_error << " " << e_excl_base  << " " << e_excl_tracks  << " " << e_excl_lepton  << " " << e_excl_pileup  << " " << e_excl_fakes  << " " << e_excl_halfmm_pileup  << " " << e_excl_halfmm_fakes  <<" " << excl_sys_error << "\n";
  errorfile.close();
}

  
