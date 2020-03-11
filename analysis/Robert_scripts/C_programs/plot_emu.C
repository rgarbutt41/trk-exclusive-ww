//Plots Cumlative Ratio of Exclusive to Inclusive (emu) with 5% efficiency offset
//Input Ratio files in specific order. Nominal,Added,Lowered

#include <iostream>
#include <fstream>
#include <math.h>

void  plot_emu(std::string Nominal_Ratio_File, std::string Added_Ratio_File, std::string Lowered_Ratio_File)
{
  double exclusive_yield = 393.831;
  int n = 10;
  Double_t min_pT[n],Nominal_Ratios[n], Added_Ratios[n],Lowered_Ratios[n];

  int counter = 0;
  int ratio_index = 0;
  int raw_index = 0;
  
  //Nominal
  std::ifstream Nominalfile;
  Nominalfile.open( Nominal_Ratio_File );
  double u;

  while(Nominalfile >> u)
    {
      counter++;
      if ( counter %2 == 0)
	{
	  Nominal_Ratios[ratio_index] = u;
	  ratio_index++;
	}
      else if (counter %3 == 0)
	{
	  counter = 0;
	}
      else
	{
	  raw_index++;
	}
    }
  Nominalfile.close();
  raw_index = 0;
  ratio_index = 0;
  
  //Added Ratios
  std::ifstream Addedfile;
  Addedfile.open( Added_Ratio_File );
  double w;

  while(Addedfile >> w)
    {
      counter++;
      if ( counter %2 == 0)
	{
	  Added_Ratios[ratio_index] = w;
	  ratio_index++;
	}
      else if (counter %3 == 0)
	{
	  counter = 0;
	}
      else
	{
	  raw_index++;
	}
    }
  Addedfile.close();
  raw_index = 0;
  ratio_index = 0;

 //Lowered Ratios
  std::ifstream Loweredfile;
  Loweredfile.open( Lowered_Ratio_File );
  double p;
  while(Loweredfile >> p )
    {
      counter++;
      if ( counter %2 == 0)
	{
	  Lowered_Ratios[ratio_index] = p;
	  ratio_index++;
	}
      else if (counter %3 == 0)
	{
	  counter = 0;
	}
      else
	{
	   raw_index++;
	}
    }
  Loweredfile.close();


  for (int i = 0; i<n; i++)
    {
      min_pT[i] = 50*(i+1);
    }

  auto cl = new TCanvas("cl","cl");
  auto Add = new TGraph(n,min_pT,Added_Ratios);
  auto Nom = new TGraph(n,min_pT,Nominal_Ratios);
  auto Low= new TGraph(n,min_pT,Lowered_Ratios);

  TLegend *l = new TLegend (0.55,0.65,0.8,0.8);
  l->AddEntry(Add,"5% Added Efficiency","l");
  l->AddEntry(Nom,"Nominal","l");
  l->AddEntry(Low,"5% Lowered Efficiency","l");

  Nom->SetTitle("Exclusive to Inclusive Yield Ratios (emu)");
  Nom->GetXaxis()->SetTitle("Min_pT(MeV)");
  Nom->GetYaxis()->SetTitle("Ratios of Excl to Inclusive");
  Nom->GetYaxis()->SetRangeUser(0,23);

  Add->SetLineColor(4);
  Low->SetLineColor(2);

  Add->SetMarkerSize(.5);
  Nom->SetMarkerSize(.5);
  Low->SetMarkerSize(.5);

  Nom->Draw("AL*");
  Low->Draw("L*");
  Add->Draw("L*");

  l->Draw();
  l->SetBorderSize(0);
  
}
