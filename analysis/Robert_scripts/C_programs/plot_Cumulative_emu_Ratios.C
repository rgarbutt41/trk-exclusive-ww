//Plots Cumlative Ratio of Exclusive to Background for e/mu

#include <iostream>
#include <fstream>
#include <math.h>
float exclWW_xsec = 8.5e-3*0.319; //pb; fixed x-sec, sample gives 1.49e-2 x-sec, wrong!
float exclWW_SD_DD_corr = 3.39; ///< correction factor for exclusive WW SD/DD contributions

float inclWW_filter_eff = 0.026; 
float inclWW_xsec = 10.612; //pb

float Ztautau_filter_eff = 0.000319;
float Ztautau_xsec = 1901; //pb

void plot_Cumulative_emu_Ratios(std::string InclWW_Ratio_File)// std::string Ztautau_Ratio_File)
{
  double exclusive_yield = 393.831;
  int n = 10;
  Double_t ey[n], ex[n], Cumulative[n], min_pT[n];
  Double_t Ztautau_Raw[n], InclWW_Raw[n], ExclWW_Raw[n];
  Double_t rho_ExclWW[n], rho_InclWW[n], rho_Ztautau[n];
  Double_t Ztautau_Ratios[n], InclWW_Ratios[n], ExclWW_Ratios[n];
  Double_t Background_sygma[n], sum[n],Ratio[n];

  int counter = 0;

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

  /*Ztautau Ratios
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
  */
  for (int i = 0; i<n; i++)
    {
      min_pT[i] = 50*(i+1);
      
      rho_ExclWW[i] = exclusive_yield/ExclWW_Raw[i] ;
      rho_InclWW[i] = exclusive_yield/InclWW_Ratios[i]/InclWW_Raw[i];
      //  rho_Ztautau[i] = exclusive_yield/Ztautau_Ratios[i]/Ztautau_Raw[i];


      sum[i] = rho_InclWW[i]*InclWW_Raw[i]; //+rho_Ztautau[i]*Ztautau_Raw[i]; 

      Ratio[i] = exclusive_yield/sum[i];
      
      Background_sygma[i] =  pow( rho_InclWW[i], 2 )*InclWW_Raw[i]; //+pow( rho_Ztautau[i], 2 )*Ztautau_Raw[i];

      ey[i] = Ratio[i]*sqrt( 1/pow( sum[i] , 2 )*Background_sygma[i]+ 1/ExclWW_Raw[i] );
      ex[i] = 0;
    }

 auto cl = new TCanvas("cl","cl");
  auto gr = new TGraphErrors(n,min_pT,Ratio,ex,ey);
  gr->SetTitle("Exclusive to Background Yield Ratios (emu)");
  gr->GetXaxis()->SetTitle("Min_pT(MeV)");
  gr->GetYaxis()->SetTitle("Ratios of Excl to Background");
  gr->Draw("ALP");

  std::cout << "For Track min_pT of 100 MeV"<< endl;
  //  std::cout << "Ztautau Raw Yield:" << Ztautau_Raw[1] <<" " << "Ztautau Scale:" << rho_Ztautau[1] << endl;
  std::cout << "InclWW Raw Yield:" << InclWW_Raw[1] <<" " << "InclWW Scale:" << rho_InclWW[1] << endl;






}
