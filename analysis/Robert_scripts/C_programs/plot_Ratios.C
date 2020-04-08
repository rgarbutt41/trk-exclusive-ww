//Plot the ratio of signal to background for one specfic background.

#include <iostream>
#include <fstream>
#include <math.h>

void  plot_Ratios(std::string Ratio_File, const char* Title)
{
  int counter = 0;
  std::ifstream myfile;
  myfile.open (Ratio_File);
  double i;
  int n = 9;
  Double_t b_raw_yield[n], Ratios[n], e_raw_yield[n], min_pT[n];
  int k = 0;
  int g = 0;
  int f = 0;
  Double_t ex[n],ey[n];
  while(myfile >>  i)
    {
      counter++;
      if (counter %2 ==  0)
	{
	  Ratios[g]= i;
	  g++;
	}
      else if (counter %3 ==  0)
	{
	  e_raw_yield[f]= i;
	  f++;
	  counter = 0;
	}
      else
	{
	  b_raw_yield[k] = i;
	  k++;
	}
    }
  myfile.close();
  for (int i = 0; i<n; i++)
    {
      min_pT[i] = 50*(i+2);
      ey[i] = Ratios[i]*sqrt(1/e_raw_yield[i]+1/b_raw_yield[i]);
      ex[i] = 0;
      //std::cout << Ratios[i] << endl;
      //std::cout << ey[i] << endl;
      //std::cout <<e_raw_yield[i] << endl;
      //std::cout << b_raw_yield[i] <<endl;
      
      }
  int p = 9;
  Double_t b_raw_yield_s[p], Ratios_s[p], e_raw_yield_s[p], min_pT_s[p],ey_s[p],ex_s[p];
  for (int j = 0; j<p; j++)
    {
      min_pT_s[j] = min_pT[j+1];
      Ratios_s[j] = Ratios[j+1];
      ex_s[j]=0;
      ey_s[j]=ey[j+1];
      //std::cout << Ratios_s[j];
      //std::cout << ey_s[j];
    }
  auto cl = new TCanvas("cl","cl");
  auto gr = new TGraph(n,min_pT,Ratios);
  gr->SetTitle(Title);
  gr->GetXaxis()->SetTitle("Min_pT(MeV)");
  gr->GetYaxis()->SetTitle("Ratios of Excl to Incl");
  gr->GetYaxis()->SetRangeUser(0, 20);
  gr->SetMarkerStyle(20);
  gr->Draw("ALP");
}
