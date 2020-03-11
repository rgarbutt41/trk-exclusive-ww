//Plots the e/mu signl to background ratios with the 5% efficiency file changes.
//Inputs are the three Nom/Add/Low ratio files in that exact order

#include <iostream>
#include <fstream>
#include <math.h>

float Ratio_return( Double_t Ratio_Array[40], int index)
{
  float Ratio;
  Ratio = 1/(1/Ratio_Array[index]+1/Ratio_Array[index+10]+1/Ratio_Array[index+20]+1/Ratio_Array[index+30]);
  return Ratio;
}

void  plot_mumu_analysis(std::string Nominal_Ratio_File, std::string Added_Ratio_File, std::string Lowered_Ratio_File)
{
  int n = 40;
  int x = 10;
  Double_t Nominal_Ratios[n], Added_Ratios[n],Lowered_Ratios[n];
  Double_t min_pT[x], nom_sum_ratios[x], add_sum_ratios[x], low_sum_ratios[x];
  int counter = 0;



  std::ifstream Nominalfile;
  Nominalfile.open( Nominal_Ratio_File );
  double u;

  while(Nominalfile >> u)
    {
      Nominal_Ratios[counter] = u;
      counter++;
    }
  counter = 0;
  Nominalfile.close();

 std::ifstream Addedfile;
  Addedfile.open( Added_Ratio_File );
  double k;

  while(Addedfile >> k)
    {
      Added_Ratios[counter] = k;
      counter++;
    }
  counter = 0;
  Addedfile.close();

 std::ifstream Loweredfile;
  Loweredfile.open( Lowered_Ratio_File );
  double j;

  while(Loweredfile >> j)
    {
      Lowered_Ratios[counter] = j;
      counter++;
    }
  counter = 0;
  Loweredfile.close();

  for (int i = 0; i<x; i++)
    {
      min_pT[i] = 50*(i+1);
      add_sum_ratios[i] = Ratio_return(Added_Ratios, i);
      nom_sum_ratios[i] = Ratio_return(Nominal_Ratios, i);
      low_sum_ratios[i] = Ratio_return(Lowered_Ratios, i);
      //std::cout << "count: "<< min_pT[i] << std::endl;
      //std::cout << "add: "<< add_sum_ratios[i] << std::endl;
      //std::cout << "nom: "<< nom_sum_ratios[i] << std::endl;
      //std::cout << "low: " << low_sum_ratios[i] << std::endl;
    }

  


  auto cl = new TCanvas("cl","cl");
  auto Add = new TGraph(x,min_pT,add_sum_ratios);
  auto Nom = new TGraph(x,min_pT, nom_sum_ratios);
  auto Low= new TGraph(x,min_pT, low_sum_ratios);

  TLegend *l = new TLegend (0.55,0.65,0.8,0.8);
  l->AddEntry(Add,"5% Added Efficiency","l");
  l->AddEntry(Nom,"Nominal","l");
  l->AddEntry(Low,"5% Lowered Efficiency","l");

  Nom->SetTitle("Exclusive to Background Yield Ratios (mumu)");
  Nom->GetXaxis()->SetTitle("Min_pT(MeV)");
  Nom->GetYaxis()->SetTitle("Ratios of Excl to Background");
  Nom->GetYaxis()->SetRangeUser(0,6);

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
