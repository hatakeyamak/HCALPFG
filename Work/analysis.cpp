#include "Events.h"
#include "TTree.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "TChain.h"
#include <cmath>
#include <map>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include "TLorentzVector.h"
#include <stdio.h>
#include "TColor.h"
#include "TF1.h"
#include "TLegend.h"
#include "TVector3.h"
#include "TFile.h"
#include "TH1.h"
#include "TVector2.h"
#include "TCanvas.h"
#include "TH2.h"


using namespace std;

std::map <int,std::string> CapId_IntToString();
std::map <std::string,int> CapId_StringToInt();
int QIE10map(double eta, double phi, double depth);
int QIE8map(double eta, double phi, double depth);


int main(int argc, char *argv[]){


  /////////////////////////////////////
  if (argc != 6)
  {
  std::cout << "Please enter something like: ./run \"filelist_WJets_PU20bx25_100_200.txt\" \"WJets_PU20bx25_100_200\" \"Results\" \"00\" \"0\" " << std::endl;
  return EXIT_FAILURE;
  }
  //get the inputs from user
  const string InRootList = argv[1];
  const string subSampleKey = argv[2];
  const string Outdir = argv[3];
  const string inputnumber = argv[4];
  const string verbosity = argv[5];
  //////////////////////////////////////
  int verbose = atoi(verbosity.c_str());

  //some varaibles
  char filenames[500];
  vector<string> filesVec;
  ifstream fin(InRootList.c_str());
  TChain *sample_AUX = new TChain("hcalTupleTree/tree");

  char tempname[200];
  char wrongPattern[200];

  ///read the file names from the .txt files and load them to a vector.
  while(fin.getline(filenames, 500) ){filesVec.push_back(filenames);}
  cout<< "\nProcessing " << subSampleKey << " ... " << endl;



  for(unsigned int in=0; in<filesVec.size(); in++){ sample_AUX->Add(filesVec.at(in).c_str()); }

  // --- Analyse the events --------------------------------------------

  // Interface to the event content
  Events * evt = new Events(sample_AUX, subSampleKey,verbose);

  // a map between different CapId patterns and an int from 1 to 256
  map <int,std::string> capid_IntToString= CapId_IntToString();
  map <string,int>      capid_StringToInt= CapId_StringToInt();
  

  bool localRun = true;
  int NadcBin, NchargeBin;
  if(localRun==false){
    NadcBin=100;
    NchargeBin=100;
  }
  else{
    NadcBin=250;
    NchargeBin=1000;
  }


  // define some histograms
  // per channel per TS
  TH2D  QIE10vs8_charge               =  TH2D("QIE10vs8_charge","",NchargeBin,0.,5000.,NchargeBin,0.,5000.);
  TH2D  QIE10_charge_TDC              =  TH2D("QIE10_charge_TDC","",63,0.,63.,NchargeBin,0.,5000.);
  TH2D  QIE10_charge_ADC              =  TH2D("QIE10_charge_ADC","",NadcBin,0.,250.,NchargeBin,0.,5000.);  
  TH2D  QIE10_ADC_TDC                 =  TH2D("QIE10_ADC_TDC","",63,0.,63.,NadcBin,0.,250.);
  TH1D  QIE10_TDC                     =  TH1D("QIE10_TDC","",64,0.,64.);
  QIE10_TDC.Sumw2();
  TH1D  QIE10_ADC                     =  TH1D("QIE10_ADC","",NadcBin,0.,250.);
  TH1D  QIE10_charge                  =  TH1D("QIE10_charge","",100,0.,2500.);
  QIE10_charge.Sumw2();
  TH1D  QIE10_chargeAsym              =  TH1D("QIE10_chargeAsym","",100,-1.,1.);
  QIE10_chargeAsym.Sumw2();
  TH1D  QIE10_Noisecharge             =  TH1D("QIE10_Noisecharge","",100,0.,2500.);
  QIE10_Noisecharge.Sumw2();
  TH1D  QIE10_CapIdOk                 =  TH1D("QIE10_CapIdOk","",2,0.,2.);
  TH1D  QIE10_TS0CapId                =  TH1D("QIE10_TS0CapId","",4,0.,4.);

  // dual anode readout channel 
  // per channel
  TH1D  QIE10_CapIdPattern            =  TH1D("QIE10_CapIdPattern","",256,1.,256.+1.); // 256 ways to put 0..3 in 4 TSs
  TH2D  QIE10_CapIdPattern_2D         =  TH2D("QIE10_CapIdPattern_2D","",256,1.,256.+1.,256,1.,256.+1.);
  TH1D  QIE10_IsAligned               =  TH1D("QIE10_IsAligned","",4,0.,4.);

  //integrated histograms
  TH2D  QIE10vs8_charge_Integ_SOI               =  TH2D("QIE10vs8_charge_Integ_SOI","",NchargeBin,0.,5000.,NchargeBin,0.,5000.);
  TH2D  QIE10_charge_TDC_Integ_SOI              =  TH2D("QIE10_charge_TDC_Integ_SOI","",63,0.,63.,NchargeBin,0.,5000.);
  TH2D  QIE10_charge_ADC_Integ_SOI              =  TH2D("QIE10_charge_ADC_Integ_SOI","",NadcBin,0.,250.,NchargeBin,0.,5000.);
  TH2D  QIE10_ADC_TDC_Integ_SOI                 =  TH2D("QIE10_ADC_TDC_Integ_SOI","",63,0.,63.,50,0.,250.);
  TH2D  QIE10_CapIdOk_iEta_Integ_SOI            =  TH2D("QIE10_CapIdOk_iEta_Integ_SOI","",13,29,42,2,0.,2.);
  TH1D  QIE10_TDC_Integ_SOI                     =  TH1D("QIE10_TDC_Integ_SOI","",64,0.,64.);
  TH1D  QIE10_ADC_Integ_SOI                     =  TH1D("QIE10_ADC_Integ_SOI","",NadcBin,0.,250.);
  TH1D  QIE10_CapIdOk_Integ_SOI                 =  TH1D("QIE10_CapIdOk_Integ_SOI","",2,0.,2.);
  TH1D  QIE10_TS0CapId_Integ_SOI                =  TH1D("QIE10_TS0CapId_Integ_SOI","",4,0.,4.);  


  TH2D  QIE10vs8_charge_Integ_allTS               =  TH2D("QIE10vs8_charge_Integ_allTS","",NchargeBin,0.,5000.,NchargeBin,0.,5000.);
  TH2D  QIE10_charge_TDC_Integ_allTS              =  TH2D("QIE10_charge_TDC_Integ_allTS","",63,0.,63.,NchargeBin,0.,5000.);
  TH2D  QIE10_charge_ADC_Integ_allTS              =  TH2D("QIE10_charge_ADC_Integ_allTS","",NadcBin,0.,250.,NchargeBin,0.,5000.);
  TH2D  QIE10_ADC_TDC_Integ_allTS                 =  TH2D("QIE10_ADC_TDC_Integ_allTS","",63,0.,63.,50,0.,250.);
  TH1D  QIE10_TDC_Integ_allTS                     =  TH1D("QIE10_TDC_Integ_allTS","",64,0.,64.);
  TH1D  QIE10_ADC_Integ_allTS                     =  TH1D("QIE10_ADC_Integ_allTS","",NadcBin,0.,250.);
  TH1D  QIE10_CapIdOk_Integ_allTS                 =  TH1D("QIE10_CapIdOk_Integ_allTS","",2,0.,2.);
  TH1D  QIE10_TS0CapId_Integ_allTS                =  TH1D("QIE10_TS0CapId_Integ_allTS","",4,0.,4.);

  TH1D  QIE10_SOI                                 =  TH1D("QIE10_SOI","",4,0.,4.);

  // Number of time samples
  int nTS = 4;

  // one histogram for each time sample of each channel
  vector<TH2D> tsVec_QIE10vs8_charge(nTS,QIE10vs8_charge);
  vector<TH2D> tsVec_QIE10_charge_TDC(nTS,QIE10_charge_TDC);
  vector<TH2D> tsVec_QIE10_charge_ADC(nTS,QIE10_charge_ADC);
  vector<TH2D> tsVec_QIE10_ADC_TDC(nTS,QIE10_ADC_TDC);
  vector<TH1D> tsVec_QIE10_TDC(nTS,QIE10_TDC);
  vector<TH1D> tsVec_QIE10_ADC(nTS,QIE10_ADC);
  vector<TH1D> tsVec_QIE10_charge(nTS,QIE10_charge);
  vector<TH1D> tsVec_QIE10_chargeAsym(nTS,QIE10_chargeAsym);
  vector<TH1D> tsVec_QIE10_Noisecharge(nTS,QIE10_Noisecharge);
  vector<TH1D> tsVec_QIE10_CapIdOk(nTS,QIE10_CapIdOk);
  vector<TH1D> tsVec_QIE10_TS0CapId(nTS,QIE10_TS0CapId);

  // Number of split channels 
  int nChan = 24;

  // a vector of 4 histograms for each channel 
  vector<vector<TH2D>> chVec_QIE10vs8_charge(nChan,tsVec_QIE10vs8_charge); 
  vector<vector<TH2D>> chVec_QIE10_charge_TDC(nChan,tsVec_QIE10_charge_TDC);
  vector<vector<TH2D>> chVec_QIE10_charge_ADC(nChan,tsVec_QIE10_charge_ADC);
  vector<vector<TH2D>> chVec_QIE10_ADC_TDC(nChan,tsVec_QIE10_ADC_TDC);
  vector<vector<TH1D>> chVec_QIE10_TDC(nChan,tsVec_QIE10_TDC);
  vector<vector<TH1D>> chVec_QIE10_ADC(nChan,tsVec_QIE10_ADC);
  vector<vector<TH1D>> chVec_QIE10_charge(nChan,tsVec_QIE10_charge);
  vector<vector<TH1D>> chVec_QIE10_Noisecharge(nChan,tsVec_QIE10_Noisecharge);
  vector<vector<TH1D>> chVec_QIE10_CapIdOk(nChan,tsVec_QIE10_CapIdOk);
  vector<vector<TH1D>> chVec_QIE10_TS0CapId(nChan,tsVec_QIE10_TS0CapId);

  vector<TH1D>         chVec_QIE10_CapIdPattern(nChan,QIE10_CapIdPattern);
  vector<TH1D>         chVec_QIE10_IsAligned(nChan,QIE10_IsAligned);
  vector<TH2D>         chVec_QIE10_CapIdPattern_2D(nChan,QIE10_CapIdPattern_2D);

  // for dead HF channels
  TH2D  QIE10_TDC_TDC                                    =  TH2D("QIE10_TDC_TDC","",64,0.,64.,64,0.,64.);

  //integrated histograms
  TH2D  QIE10vs8_charge_Integ_DeadChan_SOI               =  TH2D("QIE10vs8_charge_Integ_DeadChan_SOI","",NchargeBin,0.,5000.,NchargeBin,0.,5000.);
  TH2D  QIE10_charge_TDC_Integ_DeadChan_SOI              =  TH2D("QIE10_charge_TDC_Integ_DeadChan_SOI","",63,0.,63.,NchargeBin,0.,5000.);
  TH2D  QIE10_charge_ADC_Integ_DeadChan_SOI              =  TH2D("QIE10_charge_ADC_Integ_DeadChan_SOI","",NadcBin,0.,250.,NchargeBin,0.,5000.);
  TH2D  QIE10_ADC_TDC_Integ_DeadChan_SOI                 =  TH2D("QIE10_ADC_TDC_Integ_DeadChan_SOI","",63,0.,63.,NadcBin,0.,250.);
  TH2D  QIE10_CapIdOk_iEta_Integ_DeadChan_SOI            =  TH2D("QIE10_CapIdOk_iEta_Integ_DeadChan_SOI","",13,29,42,2,0.,2.);
  TH1D  QIE10_TDC_Integ_DeadChan_SOI                     =  TH1D("QIE10_TDC_Integ_DeadChan_SOI","",63,0.,63.);
  TH1D  QIE10_ADC_Integ_DeadChan_SOI                     =  TH1D("QIE10_ADC_Integ_DeadChan_SOI","",NadcBin,0.,250.);
  TH1D  QIE10_CapIdOk_Integ_DeadChan_SOI                 =  TH1D("QIE10_CapIdOk_Integ_DeadChan_SOI","",2,0.,2.);
  TH1D  QIE10_TS0CapId_Integ_DeadChan_SOI                =  TH1D("QIE10_TS0CapId_Integ_DeadChan_SOI","",4,0.,4.);  


  TH2D  QIE10vs8_charge_Integ_DeadChan_allTS               =  TH2D("QIE10vs8_charge_Integ_DeadChan_allTS","",NchargeBin,0.,5000.,NchargeBin,0.,5000.);
  TH2D  QIE10_charge_TDC_Integ_DeadChan_allTS              =  TH2D("QIE10_charge_TDC_Integ_DeadChan_allTS","",63,0.,63.,NchargeBin,0.,5000.);
  TH2D  QIE10_charge_ADC_Integ_DeadChan_allTS              =  TH2D("QIE10_charge_ADC_Integ_DeadChan_allTS","",NadcBin,0.,250.,NchargeBin,0.,5000.);
  TH2D  QIE10_ADC_TDC_Integ_DeadChan_allTS                 =  TH2D("QIE10_ADC_TDC_Integ_DeadChan_allTS","",63,0.,63.,NadcBin,0.,250.);
  TH1D  QIE10_TDC_Integ_DeadChan_allTS                     =  TH1D("QIE10_TDC_Integ_DeadChan_allTS","",63,0.,63.);
  TH1D  QIE10_ADC_Integ_DeadChan_allTS                     =  TH1D("QIE10_ADC_Integ_DeadChan_allTS","",NadcBin,0.,250.);
  TH1D  QIE10_CapIdOk_Integ_DeadChan_allTS                 =  TH1D("QIE10_CapIdOk_Integ_DeadChan_allTS","",2,0.,2.);
  TH1D  QIE10_TS0CapId_Integ_DeadChan_allTS                =  TH1D("QIE10_TS0CapId_Integ_DeadChan_allTS","",4,0.,4.);

  vector<TH2D> tsVec_QIE10_TDC_TDC(nTS,QIE10_TDC_TDC);

  // for each channel
  vector<vector<TH2D>> chVec_DeadChan_QIE10vs8_charge(2,tsVec_QIE10vs8_charge);
  vector<vector<TH2D>> chVec_DeadChan_QIE10vs8_charge_noise(2,tsVec_QIE10vs8_charge);
  vector<vector<TH1D>> chVec_DeadChan_QIE10_chargeAsym(2,tsVec_QIE10_chargeAsym);
  vector<vector<TH2D>> chVec_DeadChan_QIE10_charge_TDC(2,tsVec_QIE10_charge_TDC);
  vector<vector<TH2D>> chVec_DeadChan_QIE10_charge_ADC(2,tsVec_QIE10_charge_ADC);
  vector<vector<TH2D>> chVec_DeadChan_QIE10_ADC_TDC(2,tsVec_QIE10_ADC_TDC);
  vector<vector<TH1D>> chVec_DeadChan_QIE10_TDC(2,tsVec_QIE10_TDC);
  vector<vector<TH1D>> chVec_DeadChan_QIE10_ADC(2,tsVec_QIE10_ADC);
  vector<vector<TH1D>> chVec_DeadChan_QIE10_CapIdOk(2,tsVec_QIE10_CapIdOk);
  vector<vector<TH1D>> chVec_DeadChan_QIE10_TS0CapId(2,tsVec_QIE10_TS0CapId);
  vector<vector<TH2D>> chVec_DeadChan_QIE10_TDC_TDC(2,tsVec_QIE10_TDC_TDC);

  vector<TH1D>         chVec_DeadChan_QIE10_CapIdPattern(2,QIE10_CapIdPattern);
  vector<TH1D>         chVec_DeadChan_QIE10_IsAligned(2,QIE10_IsAligned);
  vector<TH2D>         chVec_DeadChan_QIE10_CapIdPattern_2D(2,QIE10_CapIdPattern_2D);
 
 

  // Loop over the events (tree entries)
  int eventN=0;
  while( evt->loadNext() ){

  // only analyze a limited number of events
  //if(eventN>30000)break;

  if(verbose==2)cout << "###### \n eventN: " << eventN << endl;


// Ahmad_Oct27

  int NTS = evt->HFDigiCapID_()->at(0).size();
  //cout << " number of time sample: " << NTS << endl;
  string capidpattern_previous="";
  for(int ich=0;ich<evt->HFDigiFC_()->size();ich++){
    string capidpattern="";

    for(int its=0; its < NTS; its++){
      sprintf(tempname,"%d",evt->HFDigiCapID_()->at(ich)[its]);
      capidpattern.append(tempname);
    }
    if(capidpattern!="0123"&&capidpattern!="1230"&&capidpattern!="2301"&&capidpattern!="3012")
    cout << capidpattern << endl;
    if(ich!=0&&capidpattern!=capidpattern_previous)cout << " ich: " << ich << " not alligned \n";
    capidpattern_previous=capidpattern;
  }


/*
  int NTS = evt->QIE10DigiCapID_()->at(0).size();
  //cout << " number of time sample: " << NTS << endl;
  string capidpattern_previous="";
  for(int ich=0;ich<evt->QIE10DigiFC_()->size();ich++){
    string capidpattern="";
    
    for(int its=0; its < NTS; its++){
      sprintf(tempname,"%d",evt->QIE10DigiCapID_()->at(ich)[its]);
      capidpattern.append(tempname);
    }
    if(capidpattern!="012301"&&capidpattern!="123012"&&capidpattern!="230123"&&capidpattern!="301230")
    cout << capidpattern << endl;
    if(ich!=0&&capidpattern!=capidpattern_previous)cout << " ich: " << ich << " not alligned \n";
    capidpattern_previous=capidpattern;
  }
*/
// Ahmad_Oct27



/*Ahmad_Oct27

  // dead HF channels
  for(int ich=0;ich<2;ich++){

    // ich 0 <-> 15 and  1<-> 19 are the two corresponidinf qie10s in the dead HF channels 
    int ich_=-1;
    if(ich==0)ich_=15;
    if(ich==1)ich_=19;

    //record the pattern
    sprintf(tempname,"%d%d%d%d",evt->QIE10DigiCapID_()->at(ich)[0]
                       ,evt->QIE10DigiCapID_()->at(ich)[1]
                       ,evt->QIE10DigiCapID_()->at(ich)[2]
                       ,evt->QIE10DigiCapID_()->at(ich)[3]);

    chVec_DeadChan_QIE10_CapIdPattern[ich].Fill(capid_StringToInt[tempname]); 

    // set the label if bin content is one ( one time is enough )
    if(chVec_DeadChan_QIE10_CapIdPattern[ich].GetBinContent(capid_StringToInt[tempname])<1.001)
      chVec_DeadChan_QIE10_CapIdPattern[ich].GetXaxis()->SetBinLabel(capid_StringToInt[tempname],tempname);


    if(evt->QIE10DigiCapIdOk_()->at(ich)==0){

      if(verbose==2)cout << "capid not ok\n";

      sprintf(wrongPattern,"%d%d%d%d",evt->QIE10DigiCapID_()->at(ich)[0]
                       ,evt->QIE10DigiCapID_()->at(ich)[1]
                       ,evt->QIE10DigiCapID_()->at(ich)[2]
                       ,evt->QIE10DigiCapID_()->at(ich)[3]);

      int capid0, capid1, capid2, capid3;      
 
      for(int ich2=0;ich2<evt->QIE10DigiFC_()->size();ich2++){
        if(evt->QIE10DigiCapIdOk_()->at(ich2)==1){
          sprintf(tempname,"%d%d%d%d",evt->QIE10DigiCapID_()->at(ich2)[0]
                       ,evt->QIE10DigiCapID_()->at(ich2)[1]
                       ,evt->QIE10DigiCapID_()->at(ich2)[2]
                       ,evt->QIE10DigiCapID_()->at(ich2)[3]);
          capid0=evt->QIE10DigiCapID_()->at(ich2)[0];
          capid1=evt->QIE10DigiCapID_()->at(ich2)[1];
          capid2=evt->QIE10DigiCapID_()->at(ich2)[2];
          capid3=evt->QIE10DigiCapID_()->at(ich2)[3];
          break;
        }
      }
      chVec_DeadChan_QIE10_CapIdPattern_2D[ich].Fill(capid_StringToInt[wrongPattern],capid_StringToInt[tempname]);
      if(chVec_DeadChan_QIE10_CapIdPattern_2D[ich].GetBinContent(capid_StringToInt[wrongPattern],capid_StringToInt[tempname])<1.001){
        chVec_DeadChan_QIE10_CapIdPattern_2D[ich].GetYaxis()->SetBinLabel(capid_StringToInt[tempname],tempname);
        chVec_DeadChan_QIE10_CapIdPattern_2D[ich].GetXaxis()->SetBinLabel(capid_StringToInt[wrongPattern],wrongPattern);
      }

      if(capid0!=evt->QIE10DigiCapID_()->at(ich)[0])chVec_DeadChan_QIE10_IsAligned[ich].Fill(0);
      if(capid1!=evt->QIE10DigiCapID_()->at(ich)[1])chVec_DeadChan_QIE10_IsAligned[ich].Fill(1);
      if(capid2!=evt->QIE10DigiCapID_()->at(ich)[2])chVec_DeadChan_QIE10_IsAligned[ich].Fill(2);
      if(capid3!=evt->QIE10DigiCapID_()->at(ich)[3])chVec_DeadChan_QIE10_IsAligned[ich].Fill(3);

    }


    // loop over time samples
    for(int its=0;its<nTS;its++){


      QIE10_ADC_TDC_Integ_DeadChan_allTS.Fill(evt->QIE10DigiLETDC_()->at(ich)[its],evt->QIE10DigiADC_()->at(ich)[its]);
      if(evt->QIE10DigiSOI_()->at(ich)[its]==1){ // for integrated histograms only soi is written
            QIE10_ADC_TDC_Integ_DeadChan_SOI.Fill(evt->QIE10DigiLETDC_()->at(ich)[its],evt->QIE10DigiADC_()->at(ich)[its]);
      }
      chVec_DeadChan_QIE10_ADC_TDC[ich][its].Fill(evt->QIE10DigiLETDC_()->at(ich)[its],evt->QIE10DigiADC_()->at(ich)[its]);

      if(evt->QIE10DigiADC_()->at(ich)[its]>20.){

        QIE10_TDC_Integ_DeadChan_allTS.Fill(evt->QIE10DigiLETDC_()->at(ich)[its]);

        QIE10_charge_TDC_Integ_DeadChan_allTS.Fill(evt->QIE10DigiLETDC_()->at(ich)[its],evt->QIE10DigiFC_()->at(ich)[its]);

        if(evt->QIE10DigiSOI_()->at(ich)[its]==1){

          QIE10_TDC_Integ_DeadChan_SOI.Fill(evt->QIE10DigiLETDC_()->at(ich)[its]);

          QIE10_charge_TDC_Integ_DeadChan_SOI.Fill(evt->QIE10DigiLETDC_()->at(ich)[its],evt->QIE10DigiFC_()->at(ich)[its]);

        }
        chVec_DeadChan_QIE10_TDC[ich][its].Fill(evt->QIE10DigiLETDC_()->at(ich)[its]);
        chVec_DeadChan_QIE10_TDC_TDC[ich][its].Fill(evt->QIE10DigiLETDC_()->at(ich_)[its],evt->QIE10DigiLETDC_()->at(ich)[its]);
     

        chVec_DeadChan_QIE10_charge_TDC[ich][its].Fill(evt->QIE10DigiLETDC_()->at(ich)[its],evt->QIE10DigiFC_()->at(ich)[its]);

      }

      if(evt->QIE10DigiLETDC_()->at(ich)[its]<60.){

        QIE10_ADC_Integ_DeadChan_allTS.Fill(evt->QIE10DigiADC_()->at(ich)[its]);

        QIE10_charge_ADC_Integ_DeadChan_allTS.Fill(evt->QIE10DigiADC_()->at(ich)[its],evt->QIE10DigiFC_()->at(ich)[its]);

        if(evt->QIE10DigiSOI_()->at(ich)[its]==1){
          QIE10_ADC_Integ_DeadChan_SOI.Fill(evt->QIE10DigiADC_()->at(ich)[its]);

          QIE10_charge_ADC_Integ_DeadChan_SOI.Fill(evt->QIE10DigiADC_()->at(ich)[its],evt->QIE10DigiFC_()->at(ich)[its]);
        }

        chVec_DeadChan_QIE10_ADC[ich][its].Fill(evt->QIE10DigiADC_()->at(ich)[its]);

        chVec_DeadChan_QIE10_charge_ADC[ich][its].Fill(evt->QIE10DigiADC_()->at(ich)[its],evt->QIE10DigiFC_()->at(ich)[its]);

      }


      if(evt->QIE10DigiADC_()->at(ich)[its]>20.&&evt->QIE10DigiLETDC_()->at(ich)[its]<60.){

        QIE10vs8_charge_Integ_DeadChan_allTS.Fill(evt->QIE10DigiFC_()->at(ich_)[its],evt->QIE10DigiFC_()->at(ich)[its]);

        QIE10_CapIdOk_Integ_DeadChan_allTS.Fill(evt->QIE10DigiCapIdOk_()->at(ich));

        QIE10_TS0CapId_Integ_DeadChan_allTS.Fill(evt->QIE10DigiCapID_()->at(ich)[0]);

        if(evt->QIE10DigiSOI_()->at(ich)[its]==1){

          QIE10vs8_charge_Integ_DeadChan_SOI.Fill(evt->QIE10DigiFC_()->at(ich_)[its],evt->QIE10DigiFC_()->at(ich)[its]);

          QIE10_CapIdOk_Integ_DeadChan_SOI.Fill(evt->QIE10DigiCapIdOk_()->at(ich));

          QIE10_TS0CapId_Integ_DeadChan_SOI.Fill(evt->QIE10DigiCapID_()->at(ich)[0]);

          QIE10_CapIdOk_iEta_Integ_DeadChan_SOI.Fill(evt->QIE10DigiIEta_()->at(ich),evt->QIE10DigiCapIdOk_()->at(ich));

        }

        if(evt->QIE10DigiADC_()->at(ich_)[its]>20.&&evt->QIE10DigiLETDC_()->at(ich_)[its]<60.){// since the same cut is applied on the other anode
          chVec_DeadChan_QIE10vs8_charge[ich][its].Fill(evt->QIE10DigiFC_()->at(ich_)[its],evt->QIE10DigiFC_()->at(ich)[its]);
          if(evt->QIE10DigiLETDC_()->at(ich)[its]<24.&&evt->QIE10DigiLETDC_()->at(ich_)[its]<24.Ahmad_Oct27*//*&&
             evt->QIE10DigiFC_()->at(ich_)[its]>400.&&evt->QIE10DigiFC_()->at(ich)[its]>400.*//*Ahmad_Oct27
            ){
            chVec_DeadChan_QIE10vs8_charge_noise[ich][its].Fill(evt->QIE10DigiFC_()->at(ich_)[its],evt->QIE10DigiFC_()->at(ich)[its]);
            chVec_DeadChan_QIE10_chargeAsym[ich][its].Fill((evt->QIE10DigiFC_()->at(ich_)[its]-evt->QIE10DigiFC_()->at(ich)[its])
                                                          /(evt->QIE10DigiFC_()->at(ich_)[its]+evt->QIE10DigiFC_()->at(ich)[its]));
          }
        }

        chVec_DeadChan_QIE10_CapIdOk[ich][its].Fill(evt->QIE10DigiCapIdOk_()->at(ich));

        chVec_DeadChan_QIE10_TS0CapId[ich][its].Fill(evt->QIE10DigiCapID_()->at(ich)[0]);

      }

    }

  }


  
  // dual anode readout channel 
  // loop over channels
  for(int ich=0;ich<evt->HFDigiFC_()->size();ich++){
  

    //record the pattern
    sprintf(tempname,"%d%d%d%d",evt->QIE10DigiCapID_()->at(ich+2)[0]
                       ,evt->QIE10DigiCapID_()->at(ich+2)[1]
                       ,evt->QIE10DigiCapID_()->at(ich+2)[2]
                       ,evt->QIE10DigiCapID_()->at(ich+2)[3]);

    if(verbose==2)cout << "channel#: " << ich <<  " cap id distribution: " << tempname << endl;

    chVec_QIE10_CapIdPattern[ich].Fill(capid_StringToInt[tempname]); 

    // set the label if bin content is one ( one time is enough )
    if(chVec_QIE10_CapIdPattern[ich].GetBinContent(capid_StringToInt[tempname])<1.001)
      chVec_QIE10_CapIdPattern[ich].GetXaxis()->SetBinLabel(capid_StringToInt[tempname],tempname);


    if(evt->QIE10DigiCapIdOk_()->at(ich+2)==0){

      if(verbose==2)cout << "capid not ok\n";

      sprintf(wrongPattern,"%d%d%d%d",evt->QIE10DigiCapID_()->at(ich+2)[0]
                       ,evt->QIE10DigiCapID_()->at(ich+2)[1]
                       ,evt->QIE10DigiCapID_()->at(ich+2)[2]
                       ,evt->QIE10DigiCapID_()->at(ich+2)[3]);

      int capid0, capid1, capid2, capid3;      
 
      for(int ich2=0;ich2<evt->HFDigiFC_()->size();ich2++){
        if(evt->QIE10DigiCapIdOk_()->at(ich2+2)==1){
          sprintf(tempname,"%d%d%d%d",evt->QIE10DigiCapID_()->at(ich2+2)[0]
                       ,evt->QIE10DigiCapID_()->at(ich2+2)[1]
                       ,evt->QIE10DigiCapID_()->at(ich2+2)[2]
                       ,evt->QIE10DigiCapID_()->at(ich2+2)[3]);
          capid0=evt->QIE10DigiCapID_()->at(ich2+2)[0];
          capid1=evt->QIE10DigiCapID_()->at(ich2+2)[1];
          capid2=evt->QIE10DigiCapID_()->at(ich2+2)[2];
          capid3=evt->QIE10DigiCapID_()->at(ich2+2)[3];
          break;
        }
      }
      chVec_QIE10_CapIdPattern_2D[ich].Fill(capid_StringToInt[wrongPattern],capid_StringToInt[tempname]);
      if(chVec_QIE10_CapIdPattern_2D[ich].GetBinContent(capid_StringToInt[wrongPattern],capid_StringToInt[tempname])<1.001){
        chVec_QIE10_CapIdPattern_2D[ich].GetYaxis()->SetBinLabel(capid_StringToInt[tempname],tempname);
        chVec_QIE10_CapIdPattern_2D[ich].GetXaxis()->SetBinLabel(capid_StringToInt[wrongPattern],wrongPattern);
      }

      if(capid0!=evt->QIE10DigiCapID_()->at(ich+2)[0])chVec_QIE10_IsAligned[ich].Fill(0);
      if(capid1!=evt->QIE10DigiCapID_()->at(ich+2)[1])chVec_QIE10_IsAligned[ich].Fill(1);
      if(capid2!=evt->QIE10DigiCapID_()->at(ich+2)[2])chVec_QIE10_IsAligned[ich].Fill(2);
      if(capid3!=evt->QIE10DigiCapID_()->at(ich+2)[3])chVec_QIE10_IsAligned[ich].Fill(3);

    }


    // loop over time samples
    for(int its=0;its<nTS;its++){

      QIE10_SOI.Fill(evt->QIE10DigiSOI_()->at(ich+2)[its]);

      QIE10_ADC_TDC_Integ_allTS.Fill(evt->QIE10DigiLETDC_()->at(ich+2)[its],evt->QIE10DigiADC_()->at(ich+2)[its]);
      if(evt->QIE10DigiSOI_()->at(ich+2)[its]==1){ // for integrated histograms only soi is written
            QIE10_ADC_TDC_Integ_SOI.Fill(evt->QIE10DigiLETDC_()->at(ich+2)[its],evt->QIE10DigiADC_()->at(ich+2)[its]);
      }
      chVec_QIE10_ADC_TDC[ich][its].Fill(evt->QIE10DigiLETDC_()->at(ich+2)[its],evt->QIE10DigiADC_()->at(ich+2)[its]);

      if(evt->QIE10DigiADC_()->at(ich+2)[its]>20.){

        QIE10_TDC_Integ_allTS.Fill(evt->QIE10DigiLETDC_()->at(ich+2)[its]);

        QIE10_charge_TDC_Integ_allTS.Fill(evt->QIE10DigiLETDC_()->at(ich+2)[its],evt->QIE10DigiFC_()->at(ich+2)[its]);

        if(evt->QIE10DigiSOI_()->at(ich+2)[its]==1){

          QIE10_TDC_Integ_SOI.Fill(evt->QIE10DigiLETDC_()->at(ich+2)[its]);

          QIE10_charge_TDC_Integ_SOI.Fill(evt->QIE10DigiLETDC_()->at(ich+2)[its],evt->QIE10DigiFC_()->at(ich+2)[its]);

        }


        chVec_QIE10_TDC[ich][its].Fill(evt->QIE10DigiLETDC_()->at(ich+2)[its]);

        chVec_QIE10_charge_TDC[ich][its].Fill(evt->QIE10DigiLETDC_()->at(ich+2)[its],evt->QIE10DigiFC_()->at(ich+2)[its]);

      }

      if(evt->QIE10DigiLETDC_()->at(ich+2)[its]<60.){

        QIE10_ADC_Integ_allTS.Fill(evt->QIE10DigiADC_()->at(ich+2)[its]);

        QIE10_charge_ADC_Integ_allTS.Fill(evt->QIE10DigiADC_()->at(ich+2)[its],evt->QIE10DigiFC_()->at(ich+2)[its]);

        if(evt->QIE10DigiSOI_()->at(ich+2)[its]==1){
          QIE10_ADC_Integ_SOI.Fill(evt->QIE10DigiADC_()->at(ich+2)[its]);

          QIE10_charge_ADC_Integ_SOI.Fill(evt->QIE10DigiADC_()->at(ich+2)[its],evt->QIE10DigiFC_()->at(ich+2)[its]);
        }

        chVec_QIE10_ADC[ich][its].Fill(evt->QIE10DigiADC_()->at(ich+2)[its]);
        chVec_QIE10_charge[ich][its].Fill(evt->QIE10DigiFC_()->at(ich+2)[its]);
        if(evt->QIE10DigiLETDC_()->at(ich+2)[its]<24.)chVec_QIE10_Noisecharge[ich][its].Fill(evt->QIE10DigiFC_()->at(ich+2)[its]);

        chVec_QIE10_charge_ADC[ich][its].Fill(evt->QIE10DigiADC_()->at(ich+2)[its],evt->QIE10DigiFC_()->at(ich+2)[its]);

      }


      if(evt->QIE10DigiADC_()->at(ich+2)[its]>20.&&evt->QIE10DigiLETDC_()->at(ich+2)[its]<60.){

        QIE10vs8_charge_Integ_allTS.Fill(evt->HFDigiFC_()->at(ich)[2],evt->QIE10DigiFC_()->at(ich+2)[its]);

        QIE10_CapIdOk_Integ_allTS.Fill(evt->QIE10DigiCapIdOk_()->at(ich+2));

        QIE10_TS0CapId_Integ_allTS.Fill(evt->QIE10DigiCapID_()->at(ich+2)[0]);

        if(evt->QIE10DigiSOI_()->at(ich+2)[its]==1){

          QIE10vs8_charge_Integ_SOI.Fill(evt->HFDigiFC_()->at(ich)[2],evt->QIE10DigiFC_()->at(ich+2)[its]);

          QIE10_CapIdOk_Integ_SOI.Fill(evt->QIE10DigiCapIdOk_()->at(ich+2));

          QIE10_TS0CapId_Integ_SOI.Fill(evt->QIE10DigiCapID_()->at(ich+2)[0]);

          QIE10_CapIdOk_iEta_Integ_SOI.Fill(evt->HFDigiIEta_()->at(ich),evt->QIE10DigiCapIdOk_()->at(ich+2));

        }

        chVec_QIE10vs8_charge[ich][its].Fill(evt->HFDigiFC_()->at(ich)[2],evt->QIE10DigiFC_()->at(ich+2)[its]);

        chVec_QIE10_CapIdOk[ich][its].Fill(evt->QIE10DigiCapIdOk_()->at(ich+2));

        chVec_QIE10_TS0CapId[ich][its].Fill(evt->QIE10DigiCapID_()->at(ich+2)[0]);

      }

    }
  }
   
Ahmad_Oct27 */


    eventN++;
  }// End of loop over events


  // write the histograms
  sprintf(tempname,"%s/results_%s_%s.root",Outdir.c_str(),subSampleKey.c_str(),inputnumber.c_str());
  TFile *resFile = new TFile(tempname, "RECREATE");


  // dead HF channel
  TDirectory *allTSDir, *DeadChanDir, * SOIDir, *cdto_EtaDepthTS, *cdto_EtaDepth, *cdto_indivChip;
  DeadChanDir = resFile->mkdir("DeadChannels");
  DeadChanDir->cd();

  allTSDir = DeadChanDir->mkdir("Integ_DeadChanratedHists_allTS");
  allTSDir->cd();

  QIE10vs8_charge_Integ_DeadChan_allTS.Write("QIE10vs8_charge_Integ_DeadChan_allTS");
  QIE10_charge_TDC_Integ_DeadChan_allTS.Write("QIE10_charge_TDC_Integ_DeadChan_allTS");
  QIE10_charge_ADC_Integ_DeadChan_allTS.Write("QIE10_charge_ADC_Integ_DeadChan_allTS");
  QIE10_ADC_TDC_Integ_DeadChan_allTS.Write("QIE10_ADC_TDC_Integ_DeadChan_allTS");

  QIE10_TDC_Integ_DeadChan_allTS.Write("QIE10_TDC_Integ_DeadChan_allTS");
  QIE10_ADC_Integ_DeadChan_allTS.Write("QIE10_ADC_Integ_DeadChan_allTS");
  QIE10_CapIdOk_Integ_DeadChan_allTS.Write("QIE10_CapIdOk_Integ_DeadChan_allTS");
  QIE10_TS0CapId_Integ_DeadChan_allTS.Write("QIE10_TS0CapId_Integ_DeadChan_allTS");






  SOIDir = DeadChanDir->mkdir("Integ_DeadChanratedHists_SOI");
  SOIDir->cd();

  QIE10vs8_charge_Integ_DeadChan_SOI.Write("QIE10vs8_charge_Integ_DeadChan_SOI");
  QIE10_charge_TDC_Integ_DeadChan_SOI.Write("QIE10_charge_TDC_Integ_DeadChan_SOI");
  QIE10_charge_ADC_Integ_DeadChan_SOI.Write("QIE10_charge_ADC_Integ_DeadChan_SOI");
  QIE10_ADC_TDC_Integ_DeadChan_SOI.Write("QIE10_ADC_TDC_Integ_DeadChan_SOI");

  QIE10_TDC_Integ_DeadChan_SOI.Write("QIE10_TDC_Integ_DeadChan_SOI");
  QIE10_ADC_Integ_DeadChan_SOI.Write("QIE10_ADC_Integ_DeadChan_SOI");
  QIE10_CapIdOk_Integ_DeadChan_SOI.Write("QIE10_CapIdOk_Integ_DeadChan_SOI");
  QIE10_TS0CapId_Integ_DeadChan_SOI.Write("QIE10_TS0CapId_Integ_DeadChan_SOI");
  QIE10_CapIdOk_iEta_Integ_DeadChan_SOI.Write("QIE10_CapIdOk_iEta_Integ_DeadChan_SOI");

  cdto_indivChip = DeadChanDir->mkdir("individual_chips");
  cdto_indivChip->cd();

  // loop over channels
  for(int ich=0;ich<2;ich++){

    sprintf(tempname,"ieta%d_depth%d",evt->QIE10DigiIEta_()->at(ich),evt->QIE10DigiDepth_()->at(ich));
    cdto_EtaDepth = cdto_indivChip->mkdir(tempname);
    cdto_EtaDepth->cd();

    sprintf(tempname,"QIE10_CapIdPattern_ieta%d_depth%d",evt->QIE10DigiIEta_()->at(ich),evt->QIE10DigiDepth_()->at(ich));
    chVec_DeadChan_QIE10_CapIdPattern[ich].Write(tempname);
    sprintf(tempname,"QIE10_IsAligned_ieta%d_depth%d",evt->QIE10DigiIEta_()->at(ich),evt->QIE10DigiDepth_()->at(ich));
    chVec_DeadChan_QIE10_IsAligned[ich].Write(tempname);
    sprintf(tempname,"QIE10_CapIdPattern_2D_ieta%d_depth%d",evt->QIE10DigiIEta_()->at(ich),evt->QIE10DigiDepth_()->at(ich));
    chVec_DeadChan_QIE10_CapIdPattern_2D[ich].Write(tempname);

    // loop over time samples
    for(int its=0;its<nTS;its++){

      sprintf(tempname,"ieta%d_depth%d_ts%d",evt->QIE10DigiIEta_()->at(ich),evt->QIE10DigiDepth_()->at(ich),its);
      cdto_EtaDepthTS = cdto_EtaDepth->mkdir(tempname);
      cdto_EtaDepthTS->cd();

      sprintf(tempname,"Charge_QIE_10_8_ieta%d_depth%d_ts%d",evt->QIE10DigiIEta_()->at(ich),evt->QIE10DigiDepth_()->at(ich),its);
      chVec_DeadChan_QIE10vs8_charge[ich][its].Write(tempname);
      sprintf(tempname,"NoiseCharge_QIE_10_8_ieta%d_depth%d_ts%d",evt->QIE10DigiIEta_()->at(ich),evt->QIE10DigiDepth_()->at(ich),its);
      chVec_DeadChan_QIE10vs8_charge_noise[ich][its].Write(tempname);
      sprintf(tempname,"NoiseChargeAsym_ieta%d_depth%d_ts%d",evt->QIE10DigiIEta_()->at(ich),evt->QIE10DigiDepth_()->at(ich),its);
      chVec_DeadChan_QIE10_chargeAsym[ich][its].Write(tempname);
      sprintf(tempname,"QIE10_charge_TDC_ieta%d_depth%d_ts%d",evt->QIE10DigiIEta_()->at(ich),evt->QIE10DigiDepth_()->at(ich),its);
      chVec_DeadChan_QIE10_charge_TDC[ich][its].Write(tempname);
      sprintf(tempname,"QIE10_charge_ADC_ieta%d_depth%d_ts%d",evt->QIE10DigiIEta_()->at(ich),evt->QIE10DigiDepth_()->at(ich),its);
      chVec_DeadChan_QIE10_charge_ADC[ich][its].Write(tempname);
      sprintf(tempname,"QIE10_ADC_TDC_ieta%d_depth%d_ts%d",evt->QIE10DigiIEta_()->at(ich),evt->QIE10DigiDepth_()->at(ich),its);
      chVec_DeadChan_QIE10_ADC_TDC[ich][its].Write(tempname);
      sprintf(tempname,"QIE10_TDC_TDC_ieta%d_depth%d_ts%d",evt->QIE10DigiIEta_()->at(ich),evt->QIE10DigiDepth_()->at(ich),its);
      chVec_DeadChan_QIE10_TDC_TDC[ich][its].Write(tempname);

      sprintf(tempname,"QIE10TDC_ieta%d_depth%d_ts%d",evt->QIE10DigiIEta_()->at(ich),evt->QIE10DigiDepth_()->at(ich),its);
      chVec_DeadChan_QIE10_TDC[ich][its].Write(tempname);
      sprintf(tempname,"QIE10ADC_ieta%d_depth%d_ts%d",evt->QIE10DigiIEta_()->at(ich),evt->QIE10DigiDepth_()->at(ich),its);
      chVec_DeadChan_QIE10_ADC[ich][its].Write(tempname);
      sprintf(tempname,"QIE10CapIdOk_ieta%d_depth%d_ts%d",evt->QIE10DigiIEta_()->at(ich),evt->QIE10DigiDepth_()->at(ich),its);
      chVec_DeadChan_QIE10_CapIdOk[ich][its].Write(tempname);
      sprintf(tempname,"QIE10TS0CapId_ieta%d_depth%d_ts%d",evt->QIE10DigiIEta_()->at(ich),evt->QIE10DigiDepth_()->at(ich),its);
      chVec_DeadChan_QIE10_TS0CapId[ich][its].Write(tempname);
    }
  }




  // dual anode readout channel 
  allTSDir = resFile->mkdir("IntegratedHists_allTS");
  allTSDir->cd();

  QIE10vs8_charge_Integ_allTS.Write("QIE10vs8_charge_Integ_allTS");
  QIE10_charge_TDC_Integ_allTS.Write("QIE10_charge_TDC_Integ_allTS");
  QIE10_charge_ADC_Integ_allTS.Write("QIE10_charge_ADC_Integ_allTS");
  QIE10_ADC_TDC_Integ_allTS.Write("QIE10_ADC_TDC_Integ_allTS");

  QIE10_SOI.Write("QIE10_SOI");

  QIE10_TDC_Integ_allTS.Write("QIE10_TDC_Integ_allTS");
  QIE10_ADC_Integ_allTS.Write("QIE10_ADC_Integ_allTS");
  QIE10_CapIdOk_Integ_allTS.Write("QIE10_CapIdOk_Integ_allTS");
  QIE10_TS0CapId_Integ_allTS.Write("QIE10_TS0CapId_Integ_allTS");






  SOIDir = resFile->mkdir("IntegratedHists_SOI");
  SOIDir->cd();

  QIE10vs8_charge_Integ_SOI.Write("QIE10vs8_charge_Integ_SOI");
  QIE10_charge_TDC_Integ_SOI.Write("QIE10_charge_TDC_Integ_SOI");
  QIE10_charge_ADC_Integ_SOI.Write("QIE10_charge_ADC_Integ_SOI");
  QIE10_ADC_TDC_Integ_SOI.Write("QIE10_ADC_TDC_Integ_SOI");

  QIE10_TDC_Integ_SOI.Write("QIE10_TDC_Integ_SOI");
  QIE10_ADC_Integ_SOI.Write("QIE10_ADC_Integ_SOI");
  QIE10_CapIdOk_Integ_SOI.Write("QIE10_CapIdOk_Integ_SOI");
  QIE10_TS0CapId_Integ_SOI.Write("QIE10_TS0CapId_Integ_SOI");
  QIE10_CapIdOk_iEta_Integ_SOI.Write("QIE10_CapIdOk_iEta_Integ_SOI");

  cdto_indivChip = resFile->mkdir("individual_chips");
  cdto_indivChip->cd();

  // loop over channels
  for(int ich=0;ich<evt->HFDigiFC_()->size();ich++){

    sprintf(tempname,"ieta%d_depth%d",evt->HFDigiIEta_()->at(ich),evt->HFDigiDepth_()->at(ich));
    cdto_EtaDepth = cdto_indivChip->mkdir(tempname);
    cdto_EtaDepth->cd();

    sprintf(tempname,"QIE10_CapIdPattern_ieta%d_depth%d",evt->HFDigiIEta_()->at(ich),evt->HFDigiDepth_()->at(ich));
    chVec_QIE10_CapIdPattern[ich].Write(tempname);
    sprintf(tempname,"QIE10_IsAligned_ieta%d_depth%d",evt->HFDigiIEta_()->at(ich),evt->HFDigiDepth_()->at(ich));
    chVec_QIE10_IsAligned[ich].Write(tempname);
    sprintf(tempname,"QIE10_CapIdPattern_2D_ieta%d_depth%d",evt->HFDigiIEta_()->at(ich),evt->HFDigiDepth_()->at(ich));
    chVec_QIE10_CapIdPattern_2D[ich].Write(tempname);

    // loop over time samples
    for(int its=0;its<nTS;its++){

      sprintf(tempname,"ieta%d_depth%d_ts%d",evt->HFDigiIEta_()->at(ich),evt->HFDigiDepth_()->at(ich),its);
      cdto_EtaDepthTS = cdto_EtaDepth->mkdir(tempname);
      cdto_EtaDepthTS->cd();

      sprintf(tempname,"Charge_QIE_10_8_ieta%d_depth%d_ts%d",evt->HFDigiIEta_()->at(ich),evt->HFDigiDepth_()->at(ich),its);
      chVec_QIE10vs8_charge[ich][its].Write(tempname);
      sprintf(tempname,"QIE10_charge_TDC_ieta%d_depth%d_ts%d",evt->HFDigiIEta_()->at(ich),evt->HFDigiDepth_()->at(ich),its);
      chVec_QIE10_charge_TDC[ich][its].Write(tempname);
      sprintf(tempname,"QIE10_charge_ADC_ieta%d_depth%d_ts%d",evt->HFDigiIEta_()->at(ich),evt->HFDigiDepth_()->at(ich),its);
      chVec_QIE10_charge_ADC[ich][its].Write(tempname);
      sprintf(tempname,"QIE10_ADC_TDC_ieta%d_depth%d_ts%d",evt->HFDigiIEta_()->at(ich),evt->HFDigiDepth_()->at(ich),its);
      chVec_QIE10_ADC_TDC[ich][its].Write(tempname);

      sprintf(tempname,"QIE10TDC_ieta%d_depth%d_ts%d",evt->HFDigiIEta_()->at(ich),evt->HFDigiDepth_()->at(ich),its);
      chVec_QIE10_TDC[ich][its].Write(tempname);
      sprintf(tempname,"QIE10ADC_ieta%d_depth%d_ts%d",evt->HFDigiIEta_()->at(ich),evt->HFDigiDepth_()->at(ich),its);
      chVec_QIE10_ADC[ich][its].Write(tempname);

      sprintf(tempname,"QIE10charge_ieta%d_depth%d_ts%d",evt->QIE10DigiIEta_()->at(ich),evt->QIE10DigiDepth_()->at(ich),its);
      chVec_QIE10_charge[ich][its].Write(tempname);
      sprintf(tempname,"QIE10Noisecharge_ieta%d_depth%d_ts%d",evt->QIE10DigiIEta_()->at(ich),evt->QIE10DigiDepth_()->at(ich),its);
      chVec_QIE10_Noisecharge[ich][its].Write(tempname);

      sprintf(tempname,"QIE10CapIdOk_ieta%d_depth%d_ts%d",evt->HFDigiIEta_()->at(ich),evt->HFDigiDepth_()->at(ich),its);
      chVec_QIE10_CapIdOk[ich][its].Write(tempname);
      sprintf(tempname,"QIE10TS0CapId_ieta%d_depth%d_ts%d",evt->HFDigiIEta_()->at(ich),evt->HFDigiDepth_()->at(ich),its);
      chVec_QIE10_TS0CapId[ich][its].Write(tempname);
    }
  }

  
  //QIE_10_Charge_8_Energy->Write();
  //TDC->Write();
  //TDC_low8over10->Write();
  //TDC_low10over8->Write();
 
  resFile->Close();





}


// a function that convert capIdPatterns to a number from 1 to 256
  std::map <int,std::string> CapId_IntToString(){
    int binN=0;
    std::map <int,std::string> binMap;
    char tempname[200];
    for(int icap3=0; icap3<=3;  icap3++){
      for(int icap2=0; icap2<=3;  icap2++){
        for(int icap1=0; icap1<=3;  icap1++){
          for(int icap0=0; icap0<=3;  icap0++){
            std::ostringstream binS;
            binS << 1000*icap0+100*icap1+10*icap2+icap3;
            sprintf(tempname,"%d%d%d%d",icap0,icap1,icap2,icap3);
            binN++;
            //binMap[binS.str()]=binN;
            binMap[binN]=tempname;
            std::cout << "binString: " << tempname << " corresponing with binNumber: " <<binN << std::endl;
            
          }    
        }
      }
    }
    return binMap;
  }

  std::map <std::string,int> CapId_StringToInt(){
    int binN=0;
    std::map <std::string,int> binMap;
    char tempname[200];
    for(int icap3=0; icap3<=3;  icap3++){
      for(int icap2=0; icap2<=3;  icap2++){
        for(int icap1=0; icap1<=3;  icap1++){
          for(int icap0=0; icap0<=3;  icap0++){
            std::ostringstream binS;
            binS << 1000*icap0+100*icap1+10*icap2+icap3;
            sprintf(tempname,"%d%d%d%d",icap0,icap1,icap2,icap3);
            binN++;
            binMap[tempname]=binN;
          }
        }
      }
    }
    return binMap;
  }




// we would like to know given eta and depth, which component
// of the vector to look at
int QIE10map(double eta, double phi, double depth){

  if(phi!=39){
    cout << "phi has to be 39\n";
    exit(EXIT_FAILURE);
  }
  
  if(depth== 2 &&eta== 30 &&phi== 39){return 0 ;}
  if(depth== 2 &&eta== 34 &&phi== 39){return 1 ;}
  if(depth== 3 &&eta== 29 &&phi== 39){return 2 ;}
  if(depth== 3 &&eta== 30 &&phi== 39){return 3 ;}
  if(depth== 3 &&eta== 31 &&phi== 39){return 4 ;}
  if(depth== 3 &&eta== 32 &&phi== 39){return 5 ;}
  if(depth== 3 &&eta== 33 &&phi== 39){return 6 ;}
  if(depth== 3 &&eta== 34 &&phi== 39){return 7 ;}
  if(depth== 3 &&eta== 35 &&phi== 39){return 8 ;}
  if(depth== 3 &&eta== 36 &&phi== 39){return 9 ;}
  if(depth== 3 &&eta== 37 &&phi== 39){return 10 ;}
  if(depth== 3 &&eta== 38 &&phi== 39){return 11 ;}
  if(depth== 3 &&eta== 39 &&phi== 39){return 12 ;}
  if(depth== 3 &&eta== 41 &&phi== 39){return 13 ;}
  if(depth== 4 &&eta== 29 &&phi== 39){return 14 ;}
  if(depth== 4 &&eta== 30 &&phi== 39){return 15 ;}
  if(depth== 4 &&eta== 31 &&phi== 39){return 16 ;}
  if(depth== 4 &&eta== 32 &&phi== 39){return 17 ;}
  if(depth== 4 &&eta== 33 &&phi== 39){return 18 ;}
  if(depth== 4 &&eta== 34 &&phi== 39){return 19 ;}
  if(depth== 4 &&eta== 35 &&phi== 39){return 20 ;}
  if(depth== 4 &&eta== 36 &&phi== 39){return 21 ;}
  if(depth== 4 &&eta== 37 &&phi== 39){return 22 ;}
  if(depth== 4 &&eta== 38 &&phi== 39){return 23 ;}
  if(depth== 4 &&eta== 39 &&phi== 39){return 24 ;}
  if(depth== 4 &&eta== 41 &&phi== 39){return 25 ;}  

  return -1;
}

// the same function as for QIE10
int QIE8map(double eta, double phi, double depth){

  if(phi!=39){
    cout << "phi has to be 39\n";
    exit(EXIT_FAILURE);
  }

  if(depth== 1 &&eta== 29 &&phi== 39){return 2 -2;}
  if(depth== 1 &&eta== 30 &&phi== 39){return 3 -2;}
  if(depth== 1 &&eta== 31 &&phi== 39){return 4 -2;}
  if(depth== 1 &&eta== 32 &&phi== 39){return 5 -2;}
  if(depth== 1 &&eta== 33 &&phi== 39){return 6 -2;}
  if(depth== 1 &&eta== 34 &&phi== 39){return 7 -2;}
  if(depth== 1 &&eta== 35 &&phi== 39){return 8 -2;}
  if(depth== 1 &&eta== 36 &&phi== 39){return 9 -2;}
  if(depth== 1 &&eta== 37 &&phi== 39){return 10 -2;}
  if(depth== 1 &&eta== 38 &&phi== 39){return 11 -2;}
  if(depth== 1 &&eta== 39 &&phi== 39){return 12 -2;}
  if(depth== 1 &&eta== 41 &&phi== 39){return 13 -2;}
  if(depth== 2 &&eta== 29 &&phi== 39){return 14 -2;}
  if(depth== 2 &&eta== 30 &&phi== 39){return 15 -2;}
  if(depth== 2 &&eta== 31 &&phi== 39){return 16 -2;}
  if(depth== 2 &&eta== 32 &&phi== 39){return 17 -2;}
  if(depth== 2 &&eta== 33 &&phi== 39){return 18 -2;}
  if(depth== 2 &&eta== 34 &&phi== 39){return 19 -2;}
  if(depth== 2 &&eta== 35 &&phi== 39){return 20 -2;}
  if(depth== 2 &&eta== 36 &&phi== 39){return 21 -2;}
  if(depth== 2 &&eta== 37 &&phi== 39){return 22 -2;}
  if(depth== 2 &&eta== 38 &&phi== 39){return 23 -2;}
  if(depth== 2 &&eta== 39 &&phi== 39){return 24 -2;}
  if(depth== 2 &&eta== 41 &&phi== 39){return 25 -2;}

  return -1;
}

