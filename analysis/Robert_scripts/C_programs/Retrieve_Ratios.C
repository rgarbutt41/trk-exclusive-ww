/*
Input Ratio_file produced from get_TruthAnalysis_Ratio.C file will be named Ratios.txt
Output File should be of the form Add/Nom/Low_Ratios.txt
 The Ratio files contain columns with following content: raw background, Scaled Ratio, raw_signal
*/
#include <iostream>
#include <fstream>
#include <math.h>

void  Retrieve_Ratios(std::string Ratio_File, std::string Output_File)
{
  int counter = 0;
  int n = 9;
  Double_t Ratios[n];
  int ratio_index = 0;
  int raw_index = 0;

  std::ifstream Ratiofile;
  Ratiofile.open( Ratio_File );
  double u;

  //Loops over the Ratio column and inputs values into the Ratios array.
  while(Ratiofile >> u)
    {
      counter++;
      if ( counter %2 == 0)
	{
	  Ratios[ratio_index] = u;
	  ratio_index++;
	}
      else if ( counter %3 == 0)
	{
	  counter = 0;
	}
    }
  Ratiofile.close();

  //Opens the output file inputs ratios with new line separations.
  ofstream outfile;
  outfile.open(Output_File, ios::app);

  for ( int i= 0; i < n; i++ )
    {
      outfile << Ratios[i] << "\n";
    }
  outfile.close();
}
