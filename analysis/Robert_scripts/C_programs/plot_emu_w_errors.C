//Plots Cumlative Ratio of Exclusive to Inclusive (emu) with 5% efficiency offset
//Input Ratio files in specific order. Nominal,Added,Lowered

#include <iostream>
#include <fstream>
#include <math.h>

void  plot_emu_w_errors(std::string Previous_Ratio_File,std::string Previous_Error_File)
{
  int n = 9;
  Double_t min_pT[n], prev_incl_yield[n], prev_excl_yield[n], prev_incl_error[n], prev_excl_error[n], prev_Ratio[n],prev_Ratio_error[n],sqrt_R[n],sqrt_R_error[n];

  float value;
  int counter = 0;
  int array_index = 0;
  
  //Previous Ratio
  std::ifstream prev_rat_file;
  prev_rat_file.open( Previous_Ratio_File );
  double u;

  while(prev_rat_file >> u)
    {
      counter++;
      if ( counter == 2)
	{
	  prev_Ratio[array_index] = u;
	}
      else if (counter == 3)
	{
	  prev_excl_yield[array_index] = u;
	  array_index++;
	  counter = 0;	  
	}
      else
	{
	  prev_incl_yield[array_index] = u;
	}
    }
  prev_rat_file.close();
  array_index = 0;
  counter = 0;

  //Previous error
  std::ifstream prev_error_file;
  prev_error_file.open( Previous_Error_File );
  double j;

  while(prev_error_file >> j)
    {
      counter++;
      if ( counter == 2)
	{
	  prev_excl_error[array_index] = j;
	  array_index++;
	}
      else if (counter ==1)
	{
	  prev_incl_error[array_index] = j;
	}
      else if ( counter == 4 )
	{
	  counter = 0;
	}
    }
  prev_error_file.close();
  array_index = 0;
  counter = 0;


  for (int i = 0; i<n; i++)
    {
      min_pT[i] = 50*(i+2);
      prev_Ratio_error[i] = prev_Ratio[i] * sqrt( pow ( prev_excl_error[i] / prev_excl_yield[i] , 2 ) + pow ( prev_incl_error[i] / prev_incl_yield[i] , 2 ) );
      sqrt_R[i] = prev_excl_yield[i] / sqrt(prev_incl_yield[i]);
      sqrt_R_error[i] = sqrt_R[i]*sqrt(  pow ( prev_excl_error[i] / prev_excl_yield[i] , 2 ) + pow ( prev_incl_error[i] / prev_incl_yield[i]/2 , 2));
      std::cout << prev_Ratio_error[i] << " " << sqrt_R_error[i] << "\n";
    }

  auto cl = new TCanvas("cl","cl");
  auto prev_case = new TGraphErrors(n,min_pT, prev_Ratio, 0, prev_Ratio_error);


  prev_case->SetTitle("S/B with trk reco (emu)");
  prev_case->GetXaxis()->SetTitle("Min p_{T}(MeV)");
  prev_case->GetYaxis()->SetTitle("Ratio of Excl to Inclusive yields");
  prev_case->GetYaxis()->SetRangeUser(0,14);

  prev_case->Draw("AL");

  auto cl2 = new TCanvas("cl2","cl2");
  auto sqrt_ratio = new TGraphErrors(n,min_pT,sqrt_R,0,sqrt_R_error);

  sqrt_ratio->SetTitle("S/#sqrt{B} with trk reco (emu)");
  sqrt_ratio->GetXaxis()->SetTitle("Min p_{T}(MeV)");
  sqrt_ratio->GetYaxis()->SetTitle("Ratio of Exclusive to Square Root Inclusive yields");
  sqrt_ratio->GetYaxis()->SetRangeUser(0,80);

  sqrt_ratio->Draw("AL");

}
