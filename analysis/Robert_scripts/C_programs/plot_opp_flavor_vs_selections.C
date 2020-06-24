//Input histogram files of respective sample type, prints ratios and adds ratios to a file called "Ratios.txt"
//Must input one of the following sample types: InclWW, DYmumu, Ztautau, Zmumu
#include <iostream>
#include <fstream>
#include <math.h>


void plot_opp_flavor_vs_selections(std::string Ratio_File, std::string Error_File)
{
  //Declare arrays
  Double_t Ratio_array[126];
  Double_t Error_array[144];

  float value;
  int counter = 0;
  int array_index = 0;
  
  //Ratio
  std::ifstream rat_file;
  rat_file.open( Ratio_File );

  double u;

  while(rat_file >> u)
    {
      counter++;
      Ratio_array[array_index]= u;
      array_index++;
    }
  rat_file.close();

  array_index = 0;
  counter = 0;

  //Error
  std::ifstream error_file;
  error_file.open( Error_File );

  double k;

  while(error_file >> k)
    {
      counter++;
      Error_array[array_index]= k;
      array_index++;
    }
  error_file.close();
  int n =8;
  Double_t r_100[n],e_100[n],s_100[n],se_100[n];
  Double_t r_sys_100[n],s_sys_100[n];
  Double_t labels[n];
  Double_t r_500[n],e_500[n],s_500[n],se_500[n];
  Double_t r_sys_500[n],s_sys_500[n];
  Double_t r_300[n],e_300[n],s_300[n],se_300[n];
  Double_t r_sys_300[n],s_sys_300[n];
  

  array_index= 0;

  int j = 0;
  //Put yields into respective MeV array
  for (int i=0; i<6; i++)
    {
      labels[i]=i+1;
      r_100[i]= Ratio_array[i+8]/Ratio_array[i+1];

      e_100[i]= r_100[i]*sqrt( Error_array[i+9]*Error_array[i+9]/Ratio_array[i+8]/Ratio_array[i+8]+Error_array[i+1]*Error_array[i+1]/Ratio_array[i+1]/Ratio_array[i+1] );    

      r_sys_100[i]=r_100[i]*sqrt( Error_array[15]*Error_array[15]/Ratio_array[i+8]/Ratio_array[i+8]+Error_array[7]*Error_array[7]/Ratio_array[i+1]/Ratio_array[i+1])+ e_100[i];    

      s_100[i]= Ratio_array[i+8]/sqrt(Ratio_array[i+1]);
      
      se_100[i]= s_100[i]*sqrt( Error_array[i+9]*Error_array[i+9]/Ratio_array[i+8]/Ratio_array[i+8]+Error_array[i+1]*Error_array[i+1]/Ratio_array[i+1]/Ratio_array[i+1]/4 );    

      s_sys_100[i]= s_100[i]*sqrt( Error_array[15]*Error_array[15]/Ratio_array[i+8]/Ratio_array[i+8]+Error_array[7]*Error_array[7]/Ratio_array[i+1]/Ratio_array[i+1]/4 )+se_100[i];    

      //300

      r_300[i]= Ratio_array[i+8+112-56]/Ratio_array[i+1+112/2];

      e_300[i]= r_300[i]*sqrt( Error_array[i+9+128/2]*Error_array[i+9+128/2]/Ratio_array[i+8+112/2]/Ratio_array[i+8+112/2]+Error_array[i+1+128/2]*Error_array[i+1+128/2]/Ratio_array[i+1+112/2]/Ratio_array[i+1+112/2] );    

      r_sys_300[i]=r_300[i]*sqrt( Error_array[15+128/2]*Error_array[15+128/2]/Ratio_array[i+8+112/2]/Ratio_array[i+8+112/2]+Error_array[7+128/2]*Error_array[7+128/2]/Ratio_array[i+1+112/2]/Ratio_array[i+1+112/2])+ e_300[i];    

      s_300[i]= Ratio_array[i+8+112/2]/sqrt(Ratio_array[i+1+112/2]);
     
    se_300[i]= s_300[i]*sqrt( Error_array[i+9+128/2]*Error_array[i+9+128/2]/Ratio_array[i+8+112/2]/Ratio_array[i+8+112/2]+Error_array[i+1+128/2]*Error_array[i+1+128/2]/Ratio_array[i+1+112/2]/Ratio_array[i+1+112/2] /4);    

      s_sys_300[i]= s_300[i]*sqrt( Error_array[15+128/2]*Error_array[15+128/2]/Ratio_array[i+8+112/2]/Ratio_array[i+8+112/2]+Error_array[7+128/2]*Error_array[7+128/2]/Ratio_array[i+1+112/2]/Ratio_array[i+1+112/2]/4 )+se_300[i];    


      //500

      r_500[i]= Ratio_array[i+8+112]/Ratio_array[i+1+112];

      e_500[i]= r_500[i]*sqrt( Error_array[i+9+128]*Error_array[i+9+128]/Ratio_array[i+8+112]/Ratio_array[i+8+112]+Error_array[i+1+128]*Error_array[i+1+128]/Ratio_array[i+1+112]/Ratio_array[i+1+112] );    

      r_sys_500[i]=r_500[i]*sqrt( Error_array[15+128]*Error_array[15+128]/Ratio_array[i+8+112]/Ratio_array[i+8+112]+Error_array[7+128]*Error_array[7+128]/Ratio_array[i+1+112]/Ratio_array[i+1+112])+ e_500[i];    

      s_500[i]= Ratio_array[i+8+112]/sqrt(Ratio_array[i+1+112]);
     
    se_500[i]= s_500[i]*sqrt( Error_array[i+9+128]*Error_array[i+9+128]/Ratio_array[i+8+112]/Ratio_array[i+8+112]+Error_array[i+1+128]*Error_array[i+1+128]/Ratio_array[i+1+112]/Ratio_array[i+1+112] /4);    

      s_sys_500[i]= s_500[i]*sqrt( Error_array[15+128]*Error_array[15+128]/Ratio_array[i+8+112]/Ratio_array[i+8+112]+Error_array[7+128]*Error_array[7+128]/Ratio_array[i+1+112]/Ratio_array[i+1+112]/4 )+se_500[i];    


      //std::cout << Error_array[i+129] << " " << Error_array[7+128] << " ";
      
    }
  //std::cout << Error_array[128] << " " << Ratio_array[112];
  
  labels[6]=7;
  labels[7]=8;
  r_100[6]= r_100[5]/0.447;
  r_100[5]= r_100[4]/0.447;
  r_100[4]=1/0.447*r_100[3];
  r_100[3]=r_100[2]/0.447;
  r_100[2]=1/0.447*r_100[1];

  r_300[6]= r_300[5]/0.447;
  r_300[5]= r_300[4]/0.447;
  r_300[4]= r_300[3]/0.447;
  r_300[3]= r_300[2]/0.447;
  r_300[2]= r_300[1]/0.447;

  r_500[6]= r_500[5]/0.447;
  r_500[5]= r_500[4]/0.447;
  r_500[4]= r_500[3]/0.447;
  r_500[3]= r_500[2]/0.447;
  r_500[2]= r_500[1]/0.447;

  e_100[6]= e_100[5];
  e_100[5]= e_100[4];
  e_100[4]= e_100[3];
  e_100[3]= e_100[2];
  e_100[2]= e_100[1];

  e_300[6]= e_300[5];
  e_300[5]= e_300[4];
  e_300[4]= e_300[3];
  e_300[3]= e_300[2];
  e_300[2]= e_300[1];

  e_500[6]= e_500[5];
  e_500[5]= e_500[4];
  e_500[4]= e_500[3];
  e_500[3]= e_500[2];
  e_500[2]= e_500[1];

  r_sys_100[6]=r_sys_100[5];
  r_sys_100[5]=r_sys_100[4];
  r_sys_100[4]=r_sys_100[3];
  r_sys_100[3]=r_sys_100[2];
  r_sys_100[2]=r_sys_100[1];

  r_sys_300[6]=r_sys_300[5];
  r_sys_300[5]=r_sys_300[4];
  r_sys_300[4]=r_sys_300[3];
  r_sys_300[3]=r_sys_300[2];
  r_sys_300[2]=r_sys_300[1];

  r_sys_500[6]=r_sys_500[5];
  r_sys_500[5]=r_sys_500[4];
  r_sys_500[4]=r_sys_500[3];
  r_sys_500[3]=r_sys_500[2];
  r_sys_500[2]=r_sys_500[1];

  s_100[6]=s_100[5]/sqrt(0.447);
  s_100[5]=s_100[4]/sqrt(0.447);
  s_100[4]=s_100[3]/sqrt(0.447);
  s_100[3]=s_100[2]/sqrt(0.447);
  s_100[2]=s_100[1]/sqrt(0.447);

  s_300[6]=s_300[5]/sqrt(0.447);
  s_300[5]=s_300[4]/sqrt(0.447);
  s_300[4]=s_300[3]/sqrt(0.447);
  s_300[3]=s_300[2]/sqrt(0.447);
  s_300[2]=s_300[1]/sqrt(0.447);

  s_500[6]=s_500[5]/sqrt(0.447);
  s_500[5]=s_500[4]/sqrt(0.447);
  s_500[4]=s_500[3]/sqrt(0.447);
  s_500[3]=s_500[2]/sqrt(0.447);
  s_500[2]=s_500[1]/sqrt(0.447);

  se_100[6]=se_100[5];
  se_100[5]= se_100[4];
  se_100[4]=se_100[3];
  se_100[3]=se_100[2];
  se_100[2]=se_100[1];

  se_300[6]=se_300[5];
  se_300[5]=se_300[4];
  se_300[4]=se_300[3];
  se_300[3]=se_300[2];
  se_300[2]=se_300[1];

  se_500[6]=se_500[5];
  se_500[5]=se_500[4];
  se_500[4]=se_500[3];
  se_500[3]=se_500[2];
  se_500[2]=se_500[1];

  s_sys_100[6]=s_sys_100[5];
  s_sys_100[5]=s_sys_100[4];
  s_sys_100[4]=s_sys_100[3];
  s_sys_100[3]=s_sys_100[2];
  s_sys_100[2]=s_sys_100[1];

  s_sys_300[6]=s_sys_300[5];
  s_sys_300[5]=s_sys_300[4];
  s_sys_300[4]=s_sys_300[3];
  s_sys_300[3]=s_sys_300[2];
  s_sys_300[2]=s_sys_300[1];

  s_sys_500[6]=s_sys_500[5];
  s_sys_500[5]=s_sys_500[4];
  s_sys_500[4]=s_sys_500[3];
  s_sys_500[3]=s_sys_500[2];
  s_sys_500[2]=s_sys_500[1];

  std::cout << r_100[7];

  auto cl = new TCanvas("cl","cl");
  auto ratio_plot = new TGraphErrors(7,labels,r_100,0,e_100);
  auto plot_r_500 = new TGraphErrors(7,labels,r_500,0,e_500);
  auto plot_r_300 = new TGraphErrors(7,labels,r_300,0,e_300);
  TLegend* legend = new TLegend();

  ratio_plot->SetTitle("Opposite Flavor S/B vs Selections for Several min Track p_{T}");
  ratio_plot->GetXaxis()->SetTitle("Selections");
  ratio_plot->GetYaxis()->SetTitle("Ratio of Excl to Inclusive yields");
  TAxis *ax = ratio_plot->GetHistogram()->GetXaxis();
  Double_t x1 = ax->GetBinLowEdge(1);
  Double_t x2 = ax->GetBinUpEdge(ax->GetNbins());
  ratio_plot->GetHistogram()->GetXaxis()->Set(7,x1,x2);
  std::string names[8] = {"2D Track","Lepton","nCh","1mm PU","1mm Fakes","0.5mm PU","0.5mm Fakes"};
  
  for (int k=0;k<8;k++)
    {
      ratio_plot->GetHistogram()->GetXaxis()->SetBinLabel(k+1,names[k].c_str());
    }

  ratio_plot->SetMarkerStyle(6);
  ratio_plot->SetMarkerSize(2);
  ratio_plot->SetMarkerColor(kRed);
  ratio_plot->SetLineColor(kRed);
  auto ratio_plot_sys = new TGraphErrors(7,labels,r_100,0,r_sys_100);
  auto r_500_sys = new TGraphErrors(7,labels,r_500,0,r_sys_500);
  auto r_300_sys = new TGraphErrors(7,labels,r_300,0,r_sys_300);


  
   //ratio_plot_sys->SetBinLabel(1,"hi");

  legend->AddEntry(ratio_plot,"100 MeV");
  legend->AddEntry(plot_r_300,"300 MeV");
  legend->AddEntry(plot_r_500, "500 MeV");
  legend->SetHeader("min Track p_{T}","C");

  ratio_plot_sys->SetLineColor(kRed);
  ratio_plot_sys->SetMarkerColor(kRed);
  ratio_plot->GetYaxis()->SetRangeUser(0,34);

  r_500_sys->SetMarkerColor(kBlue);
  r_500_sys->SetLineColor(kBlue);


  plot_r_500->SetMarkerStyle(6);
  plot_r_500->SetMarkerSize(2);
  plot_r_500->SetMarkerColor(kBlue);
  
  ratio_plot->Draw("APE");
  ratio_plot_sys->Draw("[] same");
  plot_r_500->Draw("PE same");
  plot_r_300->Draw("PE same");
  r_300_sys->Draw("[] same");
  r_500_sys->Draw("[] same");

 
   legend->Draw();


  auto cl2 = new TCanvas("cl2","cl2");
  auto s_plot = new TGraphErrors(7,labels,s_100,0,se_100);
  auto plot_s_500 = new TGraphErrors(7,labels,s_500,0,se_500);
  auto plot_s_300 = new TGraphErrors(7,labels,s_300,0,se_300);
  TLegend* legend2 = new TLegend();


  s_plot->SetTitle("Opposite Flavor S/#sqrt{B} for Several min Track p_{T}");
  s_plot->GetXaxis()->SetTitle("Selections");
  s_plot->GetYaxis()->SetTitle("Ratio of Excl to #sqrt{B} yields");

  TAxis *ax1 = s_plot->GetHistogram()->GetXaxis();
  Double_t sx1 = ax1->GetBinLowEdge(1);
  Double_t sx2 = ax1->GetBinUpEdge(ax1->GetNbins());
  s_plot->GetHistogram()->GetXaxis()->Set(7,sx1,sx2);
  for (int k=0;k<8;k++)
    {
      s_plot->GetHistogram()->GetXaxis()->SetBinLabel(k+1,names[k].c_str());
    }

  legend2->AddEntry(s_plot,"100 MeV");
  legend2->AddEntry(plot_s_300, "300 MeV");
  legend2->AddEntry(plot_s_500,"500 MeV");
  legend2->SetHeader("min Track p_{T}","C");

  s_plot->SetMarkerStyle(6);
  s_plot->SetMarkerSize(2);
  s_plot->SetMarkerColor(kRed);

  plot_s_500->SetMarkerStyle(6);
  plot_s_500->SetMarkerSize(2);
  plot_s_500->SetMarkerColor(kBlue);
  

  s_plot->SetLineColor(kRed);
  auto s_plot_sys = new TGraphErrors(7,labels,s_100,0,s_sys_100);
  auto s_500_sys = new TGraphErrors(7,labels,s_500,0,s_sys_500);
  auto s_300_sys = new TGraphErrors(7,labels,s_300,0,s_sys_300);

  s_plot_sys->SetLineColor(kRed);
  s_plot_sys->SetMarkerColor(kRed);
  
  s_500_sys->SetMarkerColor(kBlue);
  s_500_sys->SetLineColor(kBlue);


  s_plot->GetYaxis()->SetRangeUser(0,100);
  s_plot->Draw("APE");
  plot_s_500->Draw("PE same");
  s_500_sys->Draw("[] same");
  s_plot_sys->Draw("[] same");
  s_300_sys->Draw("[] same");
  plot_s_300->Draw("PE same");
  legend2->Draw();
  
}

  
