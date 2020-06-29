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

void  plot_Same_Flavor_comp(std::string InclWW_Ratio_File, std::string InclWW_Error_File, std::string Zmumu_Ratio_File,std::string Zmumu_Error_File, std::string Ztautau_Ratio_File, std::string Ztautau_Error_File, std::string DYmumu_Ratio_File,std::string DYmumu_Error_File, std::string hInclWW_Ratio_File, std::string hInclWW_Error_File, std::string hZmumu_Ratio_File,std::string hZmumu_Error_File, std::string hZtautau_Ratio_File, std::string hZtautau_Error_File, std::string hDYmumu_Ratio_File,std::string hDYmumu_Error_File)
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

//0.5mm Window

  Double_t hincl_yield[n], hexcl_yield[n], hDY_yield[n], hZtautau_yield[n], hZmumu_yield[n];
  Double_t hincl_error[n], hexcl_error[n], hDY_error[n], hZtautau_error[n], hZmumu_error[n];
  Double_t hincl_sys_error[n], hexcl_sys_error[n], hDY_sys_error[n], hZtautau_sys_error[n], hZmumu_sys_error[n];
  Double_t hRatio[n], hsqrt_Ratio[n], hRatio_e[n], hsqrt_Ratio_e[n], hRatio_sys_e[n], hsqrt_Ratio_sys_e[n];
  Double_t hplot_sys_e[n], hplot_sqrt_sys_e[n];

  //inclusive
  std::ifstream hincl_rat_file;
  hincl_rat_file.open( hInclWW_Ratio_File );
  double hi;

  while( hincl_rat_file >> hi)
    {
      counter++;
      if ( counter == 1)
	{
	 hincl_yield[array_index] = hi;
	}
      else if (counter == 3)
	{
	  hexcl_yield[array_index] = hi;
	  array_index++;
	  counter = 0;
	}
    }
  hincl_rat_file.close();
  array_index = 0;
  counter = 0;

  std::ifstream hincl_err_file;
  hincl_err_file.open(hInclWW_Error_File);
  double hie;

  while(hincl_err_file >> hie)
    {
      counter++;
      if ( counter == 1)
	{
	  hincl_error[array_index] = hie;
	}
      else if (counter == 2)
	{
	  hincl_sys_error[array_index] = hie;
	}
      else if (counter == 3)
	{
	  hexcl_error[array_index]= hie;
	}
      else if (counter == 4 )
	{
	  hexcl_sys_error[array_index] = hie;
	  array_index++;
	  counter = 0;
	}
    }
  hincl_err_file.close();
  array_index =0;
  counter = 0;

  //Zmumu
  std::ifstream hzmumu_rat_file;
  hzmumu_rat_file.open( hZmumu_Ratio_File );
  double hzm;

  while( hzmumu_rat_file >> hzm)
    {
      counter++;
      if ( counter == 1)
	{
	  hZmumu_yield[array_index] = hzm;
	}
      else if (counter == 3)
	{
	  array_index++;
	  counter = 0;
	}
    }
  hzmumu_rat_file.close();
  array_index = 0;
  counter = 0;

  std::ifstream hzmumu_err_file;
  hzmumu_err_file.open(hZmumu_Error_File);
  double hzme;

  while(hzmumu_err_file >> hzme)
    {
      counter++;
      if ( counter == 1)
	{
	  hZmumu_error[array_index] = hzme;
	}
      else if (counter == 2)
	{
	  hZmumu_sys_error[array_index]= hzme;
	  array_index++;
	}
      else if (counter == 4 )
	{
	  counter = 0;
	}
    }
  hzmumu_err_file.close();
  array_index =0;
  counter = 0;

  //Ztautau
  std::ifstream hztautau_rat_file;
  hztautau_rat_file.open( hZtautau_Ratio_File );
  double hzt;

  while( hztautau_rat_file >> hzt)
    {
      counter++;
      if ( counter == 1)
	{
	  hZtautau_yield[array_index] = hzt;
	}
      else if (counter == 3)
	{
	  array_index++;
	  counter = 0;
	}
    }
  hztautau_rat_file.close();
  array_index = 0;
  counter = 0;

  std::ifstream hztautau_err_file;
  hztautau_err_file.open(hZtautau_Error_File);
  double hzte;

  while(hzmumu_err_file >> hzte)
    {
      counter++;
      if ( counter == 1)
	{
	  hZtautau_error[array_index] = hzte;
	}
      else if (counter == 2)
	{
	  hZtautau_sys_error[array_index]=hzte;
	  array_index++;
	}
      else if (counter == 4 )
	{
	  counter = 0;
	}
    }
  hztautau_err_file.close();
  array_index =0;
  counter = 0;

  //DYmumu
  std::ifstream hdymumu_rat_file;
  hdymumu_rat_file.open( hDYmumu_Ratio_File );
  double hd;

  while( hdymumu_rat_file >> hd)
    {
      counter++;
      if ( counter == 1)
	{
	  hDY_yield[array_index] = hd;
	}
      else if (counter == 3)
	{
	  array_index++;
	  counter = 0;
	}
    }
  hdymumu_rat_file.close();
  array_index = 0;
  counter = 0;

  std::ifstream hdymumu_err_file;
  hdymumu_err_file.open(hDYmumu_Error_File);
  double hde;

  while(hdymumu_err_file >> hde)
    {
      counter++;
      if ( counter == 1)
	{
	  hDY_error[array_index] = hde;
	}
      else if (counter == 2)
	{
	  hDY_sys_error[array_index]= hde;
	  array_index++;
	}
      else if (counter == 4 )
	{
	  counter = 0;
	}
    }
  hdymumu_err_file.close();
  array_index =0;
  counter = 0;

  //Ratios

  Double_t prefactor[n],excl_comp[n],excl_sys_comp[n],back_error[n],back_sys_error[n];
  Double_t hprefactor[n],hexcl_comp[n],hexcl_sys_comp[n],hback_error[n],hback_sys_error[n];


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

      //std::cout << Ratio_e[k] << " " << sqrt_Ratio_e[k] << " " << Ratio_sys_e[k] << " " << sqrt_Ratio_sys_e[k] <<"\n";
    }

  for (int hk = 0; hk<n; hk++)
    {
      hprefactor[hk] = 1/pow(  hincl_yield[hk]+hDY_yield[hk]+hZtautau_yield[hk]+hZmumu_yield[hk] , 2 );

      hback_error[hk] = pow( hincl_error[hk],2 ) + pow( hDY_error[hk], 2) + pow(hZmumu_error[hk], 2);
      hback_sys_error[hk] = pow( hincl_sys_error[hk],2 ) + pow( hDY_sys_error[hk], 2) + pow( hZtautau_sys_error[hk],2) + pow(hZmumu_sys_error[hk], 2);

      hexcl_comp[hk] = pow( hexcl_error[hk]/ excl_yield[hk], 2);
      hexcl_sys_comp[hk] = pow( hexcl_sys_error[hk]/ hexcl_yield[hk], 2);

      hRatio[hk] = hexcl_yield[hk]/(  hincl_yield[hk]+hDY_yield[hk]+hZtautau_yield[hk]+hZmumu_yield[hk]  );

      hRatio_e[hk] = hRatio[hk]*sqrt( hexcl_comp[hk]+hprefactor[hk]*hback_error[hk]);
      hRatio_sys_e[hk] = hRatio[hk]*sqrt( hexcl_sys_comp[hk]+hprefactor[hk]*hback_sys_error[hk]);

      //std::cout << hRatio_e[hk]-Ratio_e[hk]<< " ";

      hsqrt_Ratio[hk] = hexcl_yield[hk]/sqrt(  hincl_yield[hk]+hDY_yield[hk]+hZtautau_yield[hk]+hZmumu_yield[hk]  );

      hsqrt_Ratio_e[hk] = hsqrt_Ratio[hk]*sqrt(hexcl_comp[hk]+hprefactor[hk]*hback_error[hk]/4);
      hsqrt_Ratio_sys_e[hk] = hsqrt_Ratio[hk]*sqrt(hexcl_sys_comp[hk]+hprefactor[hk]*hback_sys_error[hk]/4);

      hplot_sys_e[hk] = hRatio_e[hk]+hRatio_sys_e[hk];
      hplot_sqrt_sys_e[hk]=hsqrt_Ratio_e[hk]+hsqrt_Ratio_sys_e[hk];

      //std::cout << Ratio_e[k] << " " << sqrt_Ratio_e[k] << " " << Ratio_sys_e[k] << " " << sqrt_Ratio_sys_e[k] <<"\n";
    }


  auto cl = new TCanvas("cl","cl");
  auto ratio_plot = new TGraphErrors(n,min_pT,Ratio,0,Ratio_e);
  auto hratio_plot = new TGraphErrors(n, min_pT, hRatio, 0 , hRatio_e);
  TLegend* legend = new TLegend();

  auto ratio_plot_sys = new TGraphErrors(n,min_pT,Ratio,0,plot_sys_e);
  auto hratio_plot_sys = new TGraphErrors(n,min_pT,hRatio,0,hplot_sys_e);

  legend->AddEntry(ratio_plot,"1mm Window");
  legend->AddEntry(hratio_plot,"0.5mm Window");

  hratio_plot->SetLineColor(kRed);
  hratio_plot_sys->SetLineColor(kRed);
  hratio_plot->SetMarkerSize(2);
  hratio_plot->SetMarkerColor(kRed);
  hratio_plot->SetMarkerStyle(6);  
  ratio_plot->SetMarkerSize(2);
  ratio_plot->SetMarkerStyle(6);  

  ratio_plot->GetXaxis()->SetTitle("Min Track p_{T} [MeV]");
  ratio_plot->GetYaxis()->SetTitle("Ratio of Signal to Background");
  ratio_plot->SetTitle("Same Flavor S/B for #mu#mu");
  ratio_plot->GetYaxis()->SetRangeUser(0,8.5);



  ratio_plot->Draw("APE");
  hratio_plot->Draw("PE same");
  ratio_plot_sys->Draw("[] same");
  hratio_plot_sys->Draw("[] same");
  legend->Draw();



  auto cl2 = new TCanvas("cl2","cl2");
  TLegend* l2 = new TLegend();
  auto sqrt_ratio_plot = new TGraphErrors(n,min_pT,sqrt_Ratio,0,sqrt_Ratio_e);
  auto sqrt_ratio_plot_sys = new TGraphErrors(n,min_pT,sqrt_Ratio,0,plot_sqrt_sys_e);
  auto hsqrt_ratio_plot = new TGraphErrors(n,min_pT, hsqrt_Ratio,0, hsqrt_Ratio_e);
  auto hsqrt_ratio_plot_sys = new TGraphErrors(n,min_pT, hsqrt_Ratio,0, hplot_sqrt_sys_e);

  hsqrt_ratio_plot_sys->SetLineColor(kRed);
  hsqrt_ratio_plot->SetLineColor(kRed);
  sqrt_ratio_plot->SetMarkerSize(2);
  hsqrt_ratio_plot->SetMarkerColor(kRed);
  sqrt_ratio_plot->SetMarkerStyle(6);

  hsqrt_ratio_plot->SetMarkerSize(2);
  hsqrt_ratio_plot->SetMarkerStyle(6);

  sqrt_ratio_plot->GetXaxis()->SetTitle("Min Track p_{T} [MeV]");
  sqrt_ratio_plot->GetYaxis()->SetTitle("Ratio of Signal to Root Background");
  sqrt_ratio_plot->SetTitle("Same Flavor S/#sqrt{B} for #mu#mu");
  sqrt_ratio_plot->GetYaxis()->SetRangeUser(0,14);


  sqrt_ratio_plot->SetMarkerStyle(6);
  sqrt_ratio_plot->SetMarkerSize(2);
  l2->AddEntry(sqrt_ratio_plot, "1mm Window");
  l2->AddEntry(hsqrt_ratio_plot, "0.5mm Window");

  
  sqrt_ratio_plot->Draw("APE");
  l2->Draw();
  hsqrt_ratio_plot->Draw("PE same");
  sqrt_ratio_plot_sys->Draw("[] same");
  hsqrt_ratio_plot_sys->Draw("[] same");
  
  
  
}
