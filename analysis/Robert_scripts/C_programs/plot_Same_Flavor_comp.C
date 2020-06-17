//Plots Cumlative Ratio of Exclusive to Background for mumu/ee without 5% efficiency offset
//Input Ratio files in specific order. InclWW, Zmumu, Ztautau, DYmumu

#include <iostream>
#include <fstream>
#include <math.h>
//Scaling factors.
float exclWW_xsec = 8.5434e-3; //pb; fixed x-sec, sample gives 1.49e-2 x-sec, wrong! Previous 8.5e-3*0.319
float exclWW_SD_DD_corr = 3.39; ///< correction factor for exclusive WW SD/DD contributions
float exclWW_filter_eff = 0.30838;

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


void  plot_Same_Flavor_w_error(std::string InclWW_Ratio_File, std::string InclWW_Error_File, std::string Zmumu_Ratio_File,std::string Zmumu_Error_File, std::string Ztautau_Ratio_File, std::string Ztautau_Error_File, std::string DYmumu_Ratio_File,std::string DYmumu_Error_File)
{
  int n = 9;
  Double_t min_pT[n], incl_yield[n], excl_yield[n], DY_yield[n], Ztautau_yield[n], Zmumu_yield[n];
  Double_t incl_error[n], excl_error[n],DY_error[n], Ztautau_error[n], Zmumu_error[n];
  Double_t incl_sys_error[n],excl_sys_error[n],DY_sys_error[n], Ztautau_sys_error[n],Zmumu_sys_error[n];
  Double_t Ratio[n],sqrt_Ratio[n],Ratio_e[n],sqrt_Ratio_e[n],Ratio_sys_e[n],sqrt_Ratio_sys_e[n];
  Double_t plot_sys_e[n], plot_sqrt_sys_e[n];
  float value;
  int counter = 0;
  int array_index = 0;

  //inclusive
  std::ifstream incl_rat_file;
  incl_rat_file.open( InclWW_Ratio_File );
  double i;

  while( incl_rat_file >> i)
    {
      counter++;
      if ( counter == 1)
	{
	  incl_yield[array_index] = i;
	}
      else if (counter == 3)
	{
	  excl_yield[array_index] = i;
	  array_index++;
	  counter = 0;
	}
    }
  incl_rat_file.close();
  array_index = 0;
  counter = 0;

  std::ifstream incl_err_file;
  incl_err_file.open(InclWW_Error_File);
  double ie;

  while(incl_err_file >> ie)
    {
      counter++;
      if ( counter == 1)
	{
	  incl_error[array_index] = ie;
	}
      else if (counter == 2)
	{
	  incl_sys_error[array_index] = ie;
	}
      else if (counter == 3)
	{
	  excl_error[array_index]= ie;
	}
      else if (counter == 4 )
	{
	  excl_sys_error[array_index] = ie;
	  array_index++;
	  counter = 0;
	}
    }
  incl_err_file.close();
  array_index =0;
  counter = 0;

  //Zmumu
  std::ifstream zmumu_rat_file;
  zmumu_rat_file.open( Zmumu_Ratio_File );
  double zm;

  while( zmumu_rat_file >> zm)
    {
      counter++;
      if ( counter == 1)
	{
	  Zmumu_yield[array_index] = zm;
	}
      else if (counter == 3)
	{
	  array_index++;
	  counter = 0;
	}
    }
  zmumu_rat_file.close();
  array_index = 0;
  counter = 0;

  std::ifstream zmumu_err_file;
  zmumu_err_file.open(Zmumu_Error_File);
  double zme;

  while(zmumu_err_file >> zme)
    {
      counter++;
      if ( counter == 1)
	{
	  Zmumu_error[array_index] = zme;
	}
      else if (counter == 2)
	{
	  Zmumu_sys_error[array_index]= zme;
	  array_index++;
	}
      else if (counter == 4 )
	{
	  counter = 0;
	}
    }
  zmumu_err_file.close();
  array_index =0;
  counter = 0;

  //Ztautau
  std::ifstream ztautau_rat_file;
  ztautau_rat_file.open( Ztautau_Ratio_File );
  double zt;

  while( ztautau_rat_file >> zt)
    {
      counter++;
      if ( counter == 1)
	{
	  Ztautau_yield[array_index] = zt;
	}
      else if (counter == 3)
	{
	  array_index++;
	  counter = 0;
	}
    }
  ztautau_rat_file.close();
  array_index = 0;
  counter = 0;

  std::ifstream ztautau_err_file;
  ztautau_err_file.open(Ztautau_Error_File);
  double zte;

  while(zmumu_err_file >> zte)
    {
      counter++;
      if ( counter == 1)
	{
	  Ztautau_error[array_index] = zte;
	}
      else if (counter == 2)
	{
	  Ztautau_sys_error[array_index]=zte;
	  array_index++;
	}
      else if (counter == 4 )
	{
	  counter = 0;
	}
    }
  ztautau_err_file.close();
  array_index =0;
  counter = 0;

  //DYmumu
  std::ifstream dymumu_rat_file;
  dymumu_rat_file.open( DYmumu_Ratio_File );
  double d;

  while( dymumu_rat_file >> d)
    {
      counter++;
      if ( counter == 1)
	{
	  DY_yield[array_index] = d;
	}
      else if (counter == 3)
	{
	  array_index++;
	  counter = 0;
	}
    }
  dymumu_rat_file.close();
  array_index = 0;
  counter = 0;

  std::ifstream dymumu_err_file;
  dymumu_err_file.open(DYmumu_Error_File);
  double de;

  while(dymumu_err_file >> de)
    {
      counter++;
      if ( counter == 1)
	{
	  DY_error[array_index] = de;
	}
      else if (counter == 2)
	{
	  DY_sys_error[array_index]= de;
	  array_index++;
	}
      else if (counter == 4 )
	{
	  counter = 0;
	}
    }
  dymumu_err_file.close();
  array_index =0;
  counter = 0;

  Double_t prefactor[n],excl_comp[n],excl_sys_comp[n],back_error[n],back_sys_error[n];

  for (int k = 0; k<n; k++)
    {
      prefactor[k] = 1/pow(  incl_yield[k]+DY_yield[k]+Ztautau_yield[k]+Zmumu_yield[k] , 2 );
      min_pT[k] = 50*(k+2);

      back_error[k] = pow( incl_error[k],2 ) + pow( DY_error[k], 2) + pow( Ztautau_error[k],2) + pow(Zmumu_error[k], 2);
      back_sys_error[k] = pow( incl_sys_error[k],2 ) + pow( DY_sys_error[k], 2) + pow( Ztautau_sys_error[k],2) + pow(Zmumu_sys_error[k], 2);

      excl_comp[k] = pow( excl_error[k]/ excl_yield[k], 2);
      excl_sys_comp[k] = pow( excl_sys_error[k]/ excl_yield[k], 2);

      Ratio[k] = excl_yield[k]/(  incl_yield[k]+DY_yield[k]+Ztautau_yield[k]+Zmumu_yield[k]  );

      Ratio_e[k] = Ratio[k]*sqrt( excl_comp[k]+prefactor[k]*back_error[k]);
      Ratio_sys_e[k] = Ratio[k]*sqrt( excl_sys_comp[k]+prefactor[k]*back_sys_error[k]);
      
      sqrt_Ratio[k] = excl_yield[k]/sqrt(  incl_yield[k]+DY_yield[k]+Ztautau_yield[k]+Zmumu_yield[k]  );

      sqrt_Ratio_e[k] = sqrt_Ratio[k]*sqrt(excl_comp[k]+prefactor[k]*back_error[k]/4);
      sqrt_Ratio_sys_e[k] = sqrt_Ratio[k]*sqrt(excl_sys_comp[k]+prefactor[k]*back_sys_error[k]/4);

      plot_sys_e[k] = Ratio_e[k]+Ratio_sys_e[k];
      plot_sqrt_sys_e[k]=sqrt_Ratio_e[k]+sqrt_Ratio_sys_e[k];

      std::cout << Ratio_e[k] << " " << sqrt_Ratio_e[k] << " " << Ratio_sys_e[k] << " " << sqrt_Ratio_sys_e[k] <<"\n";
    }


  auto cl = new TCanvas("cl","cl");
  auto mg = new TMultiGraph;
  auto ratio_plot = new TGraphErrors(n,min_pT,Ratio,0,Ratio_e);
  //TLegend* legend = new TLegend();

  mg->SetTitle("Same Flavor S/B 0.5mm PU window (ee)");
  mg->GetXaxis()->SetTitle("Min p_{T}(MeV)");
  mg->GetYaxis()->SetTitle("Ratio of Excl to Inclusive yields");
  //mg->GetYaxis()->SetRangeUser(0,9);

  ratio_plot->SetMarkerStyle(6);
  ratio_plot->SetMarkerSize(2);
  auto ratio_plot_sys = new TGraphErrors(n,min_pT,Ratio,0,plot_sys_e);

  mg->Add(ratio_plot_sys,"[]");
  mg->Add(ratio_plot,"AP");

  mg->GetXaxis()->SetLimits(50.,550.);
  mg->SetMinimum(0.);
  //mg->SetMaximum(9);

  //legend->AddEntry(ratio_plot,"Stat");
  //legend->AddEntry(ratio_plot_sys,"Sys");
  mg->Draw("apl");
  //legend->Draw();

  auto cl2 = new TCanvas("cl2","cl2");
  auto mg2 = new TMultiGraph;
  auto sqrt_ratio_plot = new TGraphErrors(n,min_pT,sqrt_Ratio,0,sqrt_Ratio_e);
  auto sqrt_ratio_plot_sys = new TGraphErrors(n,min_pT,sqrt_Ratio,0,plot_sqrt_sys_e);

  mg2->SetTitle("Same Flavor S/#sqrt{B} 0.5mm PU window (ee)");
  mg2->GetXaxis()->SetTitle("Min p_{T}(MeV)");
  mg2->GetYaxis()->SetTitle("Ratio of Excl to Square Root Inclusive yields");
  //mg2->GetYaxis()->SetRangeUser(0,14);

  sqrt_ratio_plot->SetMarkerStyle(6);
  sqrt_ratio_plot->SetMarkerSize(2);
  //sqrt_ratio_plot->Draw("AP");
  mg2->Add(sqrt_ratio_plot,"AP");
  mg2->Add(sqrt_ratio_plot_sys,"[]");


  mg2->GetXaxis()->SetLimits(50.,550.);
  mg2->SetMinimum(0.);
  //mg2->SetMaximum(35);

  mg2->Draw("apl");
  
}
