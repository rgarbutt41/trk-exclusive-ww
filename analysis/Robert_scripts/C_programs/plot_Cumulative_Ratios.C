//Plots Cumlative Ratio of Exclusive to Background for mumu/ee without 5% efficiency offset
//Input Ratio files in specific order. InclWW, Zmumu, Ztautau, DYmumu

#include <iostream>
#include <fstream>
#include <math.h>
//Scaling factors.
float exclWW_xsec = 8.5e-3*0.319; //pb; fixed x-sec, sample gives 1.49e-2 x-sec, wrong!
float exclWW_SD_DD_corr = 3.39; ///< correction factor for exclusive WW SD/DD contributions

float inclWW_filter_eff = 0.026; 
float inclWW_xsec = 10.612; //pb

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


void  plot_Cumulative_Ratios(std::string InclWW_Ratio_File, std::string Zmumu_Ratio_File, std::string Ztautau_Ratio_File, std::string DYmumu_Ratio_File) // std::string LowMassDY_Ratio_File)
{
  double exclusive_yield = 393.831;
  int n = 9;
  Double_t ey[n],ex[n], Cumulative[n],min_pT[n];
  int counter = 0;
  Double_t Zmumu_Raw[n], DYmumu_Raw[n], Ztautau_Raw[n], InclWW_Raw[n],LowMassDY_Raw[n],ExclWW_Raw[n], Ratio[n];
  Double_t rho_ExclWW[n], rho_InclWW[n], rho_DYmumu[n],rho_Ztautau[n],rho_LowMassDY[n], rho_Zmumu[n];

  Double_t Zmumu_Ratios[n], DYmumu_Ratios[n], Ztautau_Ratios[n], InclWW_Ratios[n],LowMassDY_Ratios[n],ExclWW_Ratios[n], Background_sygma[n], sum[n];

  int ratio_index = 0;
  int raw_index = 0;
  int exclusive_index = 0;
  
  //InclWW Raw
  std::ifstream InclWWfile;
  InclWWfile.open( InclWW_Ratio_File );
  double u;

  while(InclWWfile >> u)
    {
      counter++;
      if ( counter %2 == 0)
	{
	  InclWW_Ratios[ratio_index] = u;
	  ratio_index++;
	}
      else if (counter %3 == 0)
	{
	  counter = 0;
	  ExclWW_Raw[exclusive_index] = u;
	  exclusive_index++;
	}
      else
	{
	  InclWW_Raw[raw_index] = u;
	  raw_index++;
	}
    }
  InclWWfile.close();
  raw_index = 0;
  ratio_index = 0;
  
  //Zmumu Ratios
  std::ifstream Zmumufile;
  Zmumufile.open( Zmumu_Ratio_File );
  double w;

  while(Zmumufile >> w)
    {
      counter++;
      if ( counter %2 == 0)
	{
	  Zmumu_Ratios[ratio_index] = w;
	  ratio_index++;
	}
      else if (counter %3 == 0)
	{
	  counter = 0;
	}
      else
	{
	  Zmumu_Raw[raw_index] = w;
	  raw_index++;
	}
    }
  Zmumufile.close();
  raw_index = 0;
  ratio_index = 0;

 //DYmumu Ratios
  std::ifstream DYmumufile;
  DYmumufile.open( DYmumu_Ratio_File );
  double o;

  while(DYmumufile >> o)
    {
      counter++;
      if ( counter %2 == 0)
	{
	  DYmumu_Ratios[ratio_index] = o;
	  ratio_index++;
	}
      else if (counter %3 == 0)
	{
	  counter = 0;
	}
      else
	{
	  DYmumu_Raw[raw_index] = o;
	  raw_index++;
	}
    }
  DYmumufile.close();

  raw_index = 0;
  ratio_index = 0;

 //Ztautau Ratios
  std::ifstream Ztautaufile;
  Ztautaufile.open( Ztautau_Ratio_File );
  double p;
  while(Ztautaufile >> p )
    {
      counter++;
      if ( counter %2 == 0)
	{
	  Ztautau_Ratios[ratio_index] = p;
	  ratio_index++;
	}
      else if (counter %3 == 0)
	{
	  counter = 0;
	}
      else
	{
	   Ztautau_Raw[raw_index] = p;
	   raw_index++;
	}
    }
  Ztautaufile.close();

  raw_index = 0;
  ratio_index = 0;

  /*LMDY Ratios
  std::ifstream LMDYfile;
  LMDYfile.open( LowMassDY_Ratio_File );
  double e;

  while(LMDYfile >> e)
    {
      counter++;
      if ( counter %2 == 0)
	{
	  LowMassDY_Ratios[ratio_index] = e;
	  ratio_index++;
	}
      else if (counter %3 == 0)
	{
	  counter = 0;
	}
      else
	{
	  LMDY_Raw[raw_index] = e;
	  raw_index++;
	}
    }
  LMDYfile.close();
  
  raw_index = 0;
  ratio_index = 0;
  */ 

  //Scale factors for the ratios.
  for (int i = 0; i<n; i++)
    {
      min_pT[i] = 50*(i+2);
      
      rho_ExclWW[i] = exclusive_yield/ExclWW_Raw[i] ;
      //rho_LowMassDY[i] = exclusive_yield/LowMassDY_Ratios[i]/LowMassDY_Raw[i];
      rho_InclWW[i] = exclusive_yield/InclWW_Ratios[i]/InclWW_Raw[i];
      rho_DYmumu[i] = exclusive_yield/DYmumu_Ratios[i]/DYmumu_Raw[i];
      rho_Ztautau[i] = exclusive_yield/Ztautau_Ratios[i]/Ztautau_Raw[i];
      rho_Zmumu[i] = exclusive_yield/Zmumu_Ratios[i]/Zmumu_Raw[i];


      sum[i] = rho_InclWW[i]*InclWW_Raw[i]+rho_DYmumu[i]*DYmumu_Raw[i]+rho_Zmumu[i]*Zmumu_Raw[i]+rho_Ztautau[i]*Ztautau_Raw[i]; //+rho_LowMassDY[i]*LowMassDY_Raw[i];

      Ratio[i] = exclusive_yield/sum[i];
      
      Background_sygma[i] =  pow( rho_InclWW[i], 2 )*InclWW_Raw[i]+pow( rho_DYmumu[i], 2 )*DYmumu_Raw[i]+pow( rho_Zmumu[i], 2 )*Zmumu_Raw[i]+pow( rho_Ztautau[i], 2 )*Ztautau_Raw[i];
      //+pow( rho_LowMassDY[i], 2 )*LowMassDY_Raw[i] 

      ey[i] = Ratio[i]*sqrt( 1/pow( sum[i] , 2 )*Background_sygma[i]+ 1/ExclWW_Raw[i] );
      ex[i] = 0;
    }

  auto cl = new TCanvas("cl","cl");
  auto gr = new TGraphErrors(n,min_pT,Ratio,ex,ey);
  gr->SetTitle("Same Flavor S/B with 2D track eff");
  gr->GetXaxis()->SetTitle("Min_pT(MeV)");
  gr->GetYaxis()->SetTitle("Ratios of Excl to Background");
  gr->GetYaxis()->SetRangeUser(0,5.2);
  gr->Draw("ALP");

  std::cout << "For Track min_pT of 100 MeV"<< endl;
  std::cout << "DYmumu Raw Yield:" << DYmumu_Raw[1] <<" " << "DYmumu Scale:" << rho_DYmumu[1] << endl;
  std::cout << "Zmumu Raw Yield:" << Zmumu_Raw[1] <<" " << "Zmumu Scale:" << rho_Zmumu[1] << endl;
  std::cout << "Ztautau Raw Yield:" << Ztautau_Raw[1] <<" " << "Ztautau Scale:" << rho_Ztautau[1] << endl;
  std::cout << "InclWW Raw Yield:" << InclWW_Raw[1] <<" " << "InclWW Scale:" << rho_InclWW[1] << endl;
 
  
}
