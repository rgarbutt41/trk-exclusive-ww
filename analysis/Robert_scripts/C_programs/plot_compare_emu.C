//Plots Cumlative Ratio of Exclusive to Inclusive (emu) with 5% efficiency offset
//Input Ratio files in specific order. Nominal,Added,Lowered

#include <iostream>
#include <fstream>
#include <math.h>

void  plot_compare_emu(std::string Previous_Ratio_File,std::string Previous_Error_File, std::string New_Ratio_File, std::string New_Error_File)
{
  int n = 9;
  Double_t min_pT[n], prev_incl_yield[n], prev_excl_yield[n], prev_incl_error[n], prev_excl_error[n], new_incl_yield[n], new_excl_yield[n], new_excl_error[n], new_incl_error[n], prev_Ratio[n],prev_Ratio_error[n],new_Ratio[n],new_Ratio_error[n];

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

  //New Ratio
  std::ifstream new_ratio_file;
  new_ratio_file.open( New_Ratio_File );
  double k;

  while(new_ratio_file >> k)
    {
      counter++;
      if ( counter  == 2)
	{
	  new_Ratio[array_index] = k;
	}
      else if (counter  == 3)
	{
	  new_excl_yield[array_index] = k;
	  array_index++;
	  counter = 0;	  
	}
      else
	{
	  new_incl_yield[array_index] = k;
	}
    }
  new_ratio_file.close();
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

 //New error
  std::ifstream new_error_file;
  new_error_file.open( New_Error_File );
  double l;

  while(new_error_file >> l)
    {
      counter++;
      value = l;
      if ( counter == 2)
	{
	  new_excl_error[array_index] = value;
	  array_index++;
	}
      else if ( counter == 1 )
	{
	  new_incl_error[array_index] = l;
	}
      else if ( counter == 4 )
	{
	  counter = 0;
	}
    }
  new_error_file.close();
  array_index = 0;
  counter = 0;

  for (int i = 0; i<n; i++)
    {
      min_pT[i] = 50*(i+2);
      new_Ratio_error[i] = new_Ratio[i] * sqrt( pow ( new_excl_error[i] / new_excl_yield[i]  , 2 ) + pow ( new_incl_error[i] / new_incl_yield[i], 2 )  );
      prev_Ratio_error[i] = prev_Ratio[i] * sqrt( pow ( prev_excl_error[i] / prev_excl_yield[i] , 2 ) + pow ( prev_incl_error[i] / prev_incl_yield[i] , 2 ) );
      std::cout << new_Ratio[i] << " " << prev_Ratio[i] << "\n";
      std::cout << new_Ratio_error[i] << " " << prev_Ratio_error[i] << "\n";
    }

  auto cl = new TCanvas("cl","cl");
  auto prev_case = new TGraphErrors(n,min_pT, prev_Ratio, 0, prev_Ratio_error);
  auto new_case = new TGraphErrors(n,min_pT, new_Ratio, 0, new_Ratio_error);

  TLegend *len = new TLegend (0.55,0.65,0.8,0.8);
  len->AddEntry(prev_case,"Previous excl samp","l");
  len->AddEntry(new_case,"New excl samp","l");

  prev_case->SetTitle("S/B Yield Ratios for 1mm PU, Exclusive Sample Comparison (emu)");
  prev_case->GetXaxis()->SetTitle("Min_pT(MeV)");
  prev_case->GetYaxis()->SetTitle("Ratios of Excl to Inclusive");
  prev_case->GetYaxis()->SetRangeUser(0,23);

  new_case->SetLineColor(4);
  new_case->SetLineColor(2);

  new_case->SetMarkerSize(.5);
  prev_case->SetMarkerSize(.5);

  prev_case->Draw("AL*");
  new_case->Draw("L*");

  len->Draw();
  len->SetBorderSize(0);
  
}
