#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

void PhaseStability(){

  gStyle->SetOptStat(0);
  gStyle->SetPalette(1) ; // for better color output

  // Canvas
  int W = 1200;
  int H = 600;
  int H_ref = 600;
  int W_ref = 800;
  float T = 0.08*H_ref;
  float B = 0.12*H_ref;
  float L = 0.12*W_ref;
  float R = 0.08*W_ref;
  TCanvas *canvas = new TCanvas("canvas","canvas",10,10,W,H);
  canvas->SetFillColor(0);
  canvas->SetBorderMode(0);
  canvas->SetFrameFillStyle(0);
  canvas->SetFrameBorderMode(0);


  // files
  char filenames[500];
  vector<string> filesVec,dayVec,monthVec,runVec;

  ofstream outfile;
  outfile.open ("Phases.txt");

  string filelist_ = "macros/HCAL/InFile_PhaseStudy/PhaseStudy_.txt";
  ifstream fin_(filelist_.c_str());
  string line;
  while (getline(fin_, line))
  {
      if(line.substr(0,1)=="#")continue;
      istringstream ss(line);

      string var1, var2, var3, var4;

      ss >> var1 >> var2 >> var3 >> var4;
      
      filesVec.push_back(var1);
      dayVec.push_back(var2);
      monthVec.push_back(var3);
      runVec.push_back(var4);
  }




  int NumFile = filesVec.size();
  int binperfile = 24 + 2 + 1; // 24 channels + 2 pure QIE10 channels + one bin for average
  TH1D Hphase = TH1D("Hphase","",NumFile*binperfile,1.,NumFile*(double)binperfile+1.);

  Hphase.SetMinimum(12.);
  Hphase.SetMaximum(18.);
  Hphase.GetXaxis()->SetTitle("Date");
  Hphase.GetXaxis()->SetTitleOffset(1.4);
  Hphase.GetYaxis()->SetTitle("QIE10 Phase (ns)");
  //Hphase.SetLineColor(0);



  char tempname[200];

  for(int ifile=0; ifile<filesVec.size();ifile++){

    sprintf(tempname,"%s/%s",dayVec[ifile].c_str(),monthVec[ifile].c_str());
    Hphase.GetXaxis()->SetBinLabel( ifile*binperfile+25  ,   tempname );
  }

  Hphase.DrawCopy();

  TH1D * temphist;

  int iEta[]={29,30,31,32,33,34,35,36,37,38,39,41};
  int nTS = 4;
  int soi = 2;

  for(int ifile=0; ifile<filesVec.size();ifile++){

    sprintf(tempname,"%s",filesVec[ifile].c_str());
    TFile * file = new TFile(tempname,"R");

    //cout << "########\nfile: " << tempname << endl;
    //printf("date: %s/%s\n",dayVec[ifile].c_str(),monthVec[ifile].c_str());
    sprintf(tempname,"########\nfile: %s \ndate: %s/%s\n",filesVec[ifile].c_str(),dayVec[ifile].c_str(),monthVec[ifile].c_str());
    outfile << tempname;
   
    double SumOfPhase=0.;
    int NumberOfSamples=0; 
    for(int idep=1;idep<=2; idep++){
      for(int ii=0;ii<sizeof(iEta)/sizeof(iEta[0]); ii++){ 
        for(int its=2;its<=2;its++){

          NumberOfSamples++;
          sprintf(tempname,"individual_chips/ieta%d_depth%d/ieta%d_depth%d_ts%d/QIE10TDC_ieta%d_depth%d_ts%d"
                                                          ,iEta[ii],idep,iEta[ii],idep,its,iEta[ii],idep,its);
          temphist = (TH1D*) file->Get(tempname)->Clone();
          temphist->GetXaxis()->SetRangeUser(0.,60.);
          double phase = temphist->GetMean(1)/2.;
          double phase_Error = temphist->GetMeanError(1)/2.;          
          Hphase.SetBinContent( ifile*binperfile+NumberOfSamples  ,   phase  );
          Hphase.SetBinError( ifile*binperfile+NumberOfSamples  ,   phase_Error  );
  
          //printf("bin#: %d binContent: %g \n",ifile*binperfile+NumberOfSamples  ,   phase); 
          //printf("phase: %g eta: %d depth: %d\n",phase,iEta[ii],idep);
          sprintf(tempname,"phase: %g eta: %d depth: %d\n",phase,iEta[ii],idep);
          outfile << tempname;
   
          SumOfPhase+=phase;

        }
      }
    }

    // add the two QIE10 channels at depth 2 of iEta30 and iEta34
    NumberOfSamples++;
    sprintf(tempname,"DeadChannels/individual_chips/ieta%d_depth%d/ieta%d_depth%d_ts%d/QIE10TDC_ieta%d_depth%d_ts%d"
                                                    ,30,2,30,2,2,30,2,2);
    temphist = (TH1D*) file->Get(tempname)->Clone();
    temphist->GetXaxis()->SetRangeUser(0.,60.);
    double phase = temphist->GetMean(1)/2.;
    double phase_Error = temphist->GetMeanError(1)/2.;
    Hphase.SetBinContent( ifile*binperfile+NumberOfSamples  ,   phase  );
    Hphase.SetBinError( ifile*binperfile+NumberOfSamples  ,   phase_Error  );

    //printf("bin#: %d binContent: %g \n",ifile*binperfile+NumberOfSamples  ,   phase);
    //printf("phase: %g eta: %d depth: %d\n",phase,iEta[ii],idep);
    sprintf(tempname,"phase: %g eta: %d depth: %d\n",phase,30,2);
    outfile << tempname;

    SumOfPhase+=phase;
    //
    NumberOfSamples++;
    sprintf(tempname,"DeadChannels/individual_chips/ieta%d_depth%d/ieta%d_depth%d_ts%d/QIE10TDC_ieta%d_depth%d_ts%d"
                                                    ,34,2,34,2,2,34,2,2);
    temphist = (TH1D*) file->Get(tempname)->Clone();
    temphist->GetXaxis()->SetRangeUser(0.,60.);
    double phase = temphist->GetMean(1)/2.;
    double phase_Error = temphist->GetMeanError(1)/2.;
    Hphase.SetBinContent( ifile*binperfile+NumberOfSamples  ,   phase  );
    Hphase.SetBinError( ifile*binperfile+NumberOfSamples  ,   phase_Error  );

    //printf("bin#: %d binContent: %g \n",ifile*binperfile+NumberOfSamples  ,   phase);
    //printf("phase: %g eta: %d depth: %d\n",phase,iEta[ii],idep);
    sprintf(tempname,"phase: %g eta: %d depth: %d\n",phase,34,2);
    outfile << tempname;

    SumOfPhase+=phase;
    // end of two QIE10s






    Hphase.SetBinContent( ifile*binperfile+NumberOfSamples +1  ,   SumOfPhase/NumberOfSamples  );
    //printf("bin#: %d binContent(Ave): %g \n",ifile*binperfile+NumberOfSamples +1  ,   SumOfPhase/NumberOfSamples  );
    //printf("Average: %g \n", SumOfPhase/NumberOfSamples  );
    sprintf(tempname,"Average: %g \n", SumOfPhase/NumberOfSamples  );
    outfile << tempname;

    Hphase.GetXaxis()->SetRange( ifile*binperfile+1,ifile*binperfile+NumberOfSamples  );
    Hphase.SetMarkerStyle(7);
    Hphase.SetMarkerColor(1);
    Hphase.DrawCopy("same,p");

    Hphase.GetXaxis()->SetRange( ifile*binperfile+NumberOfSamples+1,ifile*binperfile+NumberOfSamples+1  );
    Hphase.SetMarkerStyle(20);
    Hphase.SetMarkerColor(2);
    Hphase.DrawCopy("same,p");  

    TLine *tl = new TLine();
    tl->SetLineColor(2);
    double tl_xmin=ifile*binperfile+1;
    double tl_xmax=ifile*binperfile+NumberOfSamples +1;
    double tl_ymin=Hphase.GetBinContent(ifile*binperfile+NumberOfSamples +1);
    double tl_ymax=Hphase.GetBinContent(ifile*binperfile+NumberOfSamples +1);
    tl->DrawLine(tl_xmin,tl_ymin,tl_xmax,tl_ymax);

    TLine * tlVer = new TLine();
    tlVer->SetLineStyle(4);
    tlVer->DrawLine(tl_xmax,Hphase.GetMinimum(),tl_xmax,Hphase.GetMaximum());


  }

  outfile.close();
  canvas->Print("QIE10PhaseCheck.png");



  for(int idep=1;idep<=2; idep++){
    for(int ii=0;ii<sizeof(iEta)/sizeof(iEta[0]); ii++){ 
      for(int its=2;its<=2;its++){

        canvas->Clear();
        TH1D * tempHist = new TH1D("tempHist","",NumFile,1.,NumFile+1.);
        int NumberOfSamples=0;

        for(int ifile=0; ifile<filesVec.size();ifile++){

          sprintf(tempname,"%s",filesVec[ifile].c_str());
          TFile * file = new TFile(tempname,"R");

          sprintf(tempname,"individual_chips/ieta%d_depth%d/ieta%d_depth%d_ts%d/QIE10TDC_ieta%d_depth%d_ts%d"
                                                          ,iEta[ii],idep,iEta[ii],idep,its,iEta[ii],idep,its);
          temphist = (TH1D*) file->Get(tempname)->Clone();
          temphist->GetXaxis()->SetRangeUser(0.,60.);
          double phase = temphist->GetMean(1)/2.;

    
          sprintf(tempname,"%s/%s",dayVec[ifile].c_str(),monthVec[ifile].c_str());
          tempHist->GetXaxis()->SetBinLabel( ifile+1  ,   tempname );
          tempHist->SetBinContent( ifile+1  ,   phase  );
          

        }
        
        tempHist->DrawCopy();
//        canvas->Print();


        delete tempHist;
      }
    }
  }
  


  //canvas->Clear();
  for(int idep=1;idep<=2; idep++){
    for(int ii=0;ii<sizeof(iEta)/sizeof(iEta[0]); ii++){
      for(int its=2;its<=2;its++){
        for(int ifile=0; ifile<filesVec.size();ifile++){
          sprintf(tempname,"%s",filesVec[ifile].c_str());
          TFile * file = new TFile(tempname,"R");

          NumberOfSamples++;
          sprintf(tempname,"individual_chips/ieta%d_depth%d/ieta%d_depth%d_ts%d/QIE10TDC_ieta%d_depth%d_ts%d"
                                                          ,iEta[ii],idep,iEta[ii],idep,its,iEta[ii],idep,its);
          temphist = (TH1D*) file->Get(tempname)->Clone();
          temphist->SetLineColor(ifile+1);
          sprintf(tempname,"run %s",runVec[ifile].c_str());
          temphist->GetXaxis()->SetRangeUser(0.,60.);
          temphist->SetTitle(tempname);
          temphist->SetTitleOffset(0.9);
          temphist->GetXaxis()->SetTitle("TDC");
          temphist->GetXaxis()->SetTitleOffset(1.3);
          temphist->GetYaxis()->SetTitle("Number of Events");
          temphist->Scale(10000./temphist->GetEntries());
          temphist->Draw();
          canvas->SetLogy();
          sprintf(tempname,"TDC_depth%d_iEta%d_run%s.png",idep,iEta[ii],runVec[ifile].c_str());
          if(idep==1&&iEta[ii]==30)canvas->Print(tempname);
           
        }
      }
    }
  }





}
