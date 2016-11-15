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
  


  // define some histograms
  TH1D  HFEnergy      =  TH1D("HFEnergy","",100,-50.,250.);
  TH1D  HFCharge      =  TH1D("HFCharge","",100,0.,2500.);

  // Number of time samples
  int nTS = 1;

  // one histogram for each time sample of each channel
  vector<TH1D> tsVec_HFEnergy(nTS,HFEnergy);

  // Number of split channels
  int nChan = 1728;

  // a vector of histograms for each channel
  vector<vector<TH1D>> chVec_HFEnergy(nChan,tsVec_HFEnergy);

  int ieta[] = {-41,-40,-39,-38,-37,-36,-35,-34,-33,-32,-31,-30,-29,29,30,31,32,33,34,35,36,37,38,39,40,41};

  int nEta = sizeof(ieta)/sizeof(ieta[0]);
  cout << "nEta: " << nEta << endl;

  // a vector of histograms for each ieta
  vector<TH1D> Vec_RecEnergy(6,HFEnergy);

  vector<TH1D> etaVec_HFEnergy_iPhi39_depth1(nEta,HFEnergy);
  vector<TH1D> etaVec_HFEnergy_iPhiNot39_depth1(nEta,HFEnergy);
  vector<TH1D> etaVec_HFEnergy_iPhi39_depth2(nEta,HFEnergy);
  vector<TH1D> etaVec_HFEnergy_iPhiNot39_depth2(nEta,HFEnergy);
  
  vector<TH1D> etaVec_HFCharge_iPhi39_depth1(nEta,HFCharge);
  vector<TH1D> etaVec_HFCharge_iPhiNot39_depth1(nEta,HFCharge);
  vector<TH1D> etaVec_HFCharge_iPhi39_depth2(nEta,HFCharge);
  vector<TH1D> etaVec_HFCharge_iPhiNot39_depth2(nEta,HFCharge);

  // Loop over the events (tree entries)
  int eventN=0;
  while( evt->loadNext() ){

    // only analyze a limited number of events
    if(eventN>0)break;

    if(verbose==2)cout << "###### \n eventN: " << eventN << endl;

    // loop over channels
    for(int ich=0;ich<evt->HFDigiFC_()->size();ich++){

      if(evt->HFDigiDepth_()->at(ich)!=2)continue;
      if(evt->HFDigiIPhi_()->at(ich)!=37 && evt->HFDigiIPhi_()->at(ich)!=39 && evt->HFDigiIPhi_()->at(ich)!=41 )continue;
      if(evt->HFDigiIEta_()->at(ich)!=30 && evt->HFDigiIEta_()->at(ich)!=34 )continue;

      if(Vec_RecEnergy.size() < eventN+1){cout<< "Vec_RecEnergy.size() is not consistent\n"; return 2;}

/*
      if(evt->HFDigiIPhi_()->at(ich)==39&&evt->HFDigiIEta_()->at(ich)>0){
        cout << "Eta: " << evt->HFDigiIEta_()->at(ich) << " depth: " << evt->HFDigiDepth_()->at(ich) << endl;
        cout << " qieEta: " << evt->QIE10DigiIEta_()->at(qie10ch) << " qieDepth: " << evt->QIE10DigiDepth_()->at(qie10ch) << endl;
      }
*/

      int qie10ch=-1;
      if(evt->HFDigiIPhi_()->at(ich)==39&&evt->HFDigiIEta_()->at(ich)>0)
        qie10ch = QIE10map(evt->HFDigiIEta_()->at(ich), evt->HFDigiIPhi_()->at(ich), evt->HFDigiDepth_()->at(ich));

      double rcgain = evt->HFDigiRCGain_()->at(ich)[2];
      if(evt->HFDigiIPhi_()->at(ich)==39)rcgain/=2.;
      if(evt->HFDigiIPhi_()->at(ich)==39&&evt->HFDigiIEta_()->at(ich)==30&&evt->HFDigiDepth_()->at(ich)==2){
        rcgain*=2.;
        for(int its=0;its<4;its++){
          if(evt->QIE10DigiSOI_()->at(qie10ch)[its]==1){
            Vec_RecEnergy[eventN].Fill(rcgain*(evt->QIE10DigiFC_()->at(ich+2)[its]+evt->QIE10DigiFC_()->at(0)[its]));
          }
        }
      }
      else if(evt->HFDigiIPhi_()->at(ich)==39&&evt->HFDigiIEta_()->at(ich)==34&&evt->HFDigiDepth_()->at(ich)==2){
        rcgain*=2.;
        for(int its=0;its<4;its++){
          //if(evt->QIE10DigiSOI_()->at(ich+2)[its]==1){
            //Vec_RecEnergy[eventN].Fill(rcgain*(evt->QIE10DigiFC_()->at(ich+2)[its]+evt->QIE10DigiFC_()->at(1)[its]));
          //}
        }
      }
      else{
        Vec_RecEnergy[eventN].Fill(rcgain*evt->HFDigiFC_()->at(ich)[2]);
      }

      

        for(int ii=0; ii < nEta; ii++){
          if(evt->HFDigiIEta_()->at(ich)==ieta[ii]){
   
            //printf("ieta: %d iphi: %d depth: %d",evt->HFDigiIEta_()->at(ich),evt->HFDigiIPhi_()->at(ich),evt->HFDigiDepth_()->at(ich));
            //cout << " rcgain: " <<  rcgain << " Origrcgain: " << evt->HFDigiRCGain_()->at(ich)[2] << endl;

   
            if(evt->HFDigiIPhi_()->at(ich)==39&&evt->HFDigiDepth_()->at(ich)==1){
              etaVec_HFEnergy_iPhi39_depth1[ii].Fill(evt->HFDigiEnergy_()->at(ich)[2]);  
              etaVec_HFCharge_iPhi39_depth1[ii].Fill(evt->HFDigiFC_()->at(ich)[2]);
            }
            if(evt->HFDigiIPhi_()->at(ich)!=39&&evt->HFDigiDepth_()->at(ich)==1){
              etaVec_HFEnergy_iPhiNot39_depth1[ii].Fill(evt->HFDigiEnergy_()->at(ich)[2]);
              etaVec_HFCharge_iPhiNot39_depth1[ii].Fill(evt->HFDigiFC_()->at(ich)[2]);
            }
            if(evt->HFDigiIPhi_()->at(ich)==39&&evt->HFDigiDepth_()->at(ich)==2){
              etaVec_HFEnergy_iPhi39_depth2[ii].Fill(evt->HFDigiEnergy_()->at(ich)[2]);
              etaVec_HFCharge_iPhi39_depth2[ii].Fill(evt->HFDigiFC_()->at(ich)[2]);
            }
            if(evt->HFDigiIPhi_()->at(ich)!=39&&evt->HFDigiDepth_()->at(ich)==2){
              etaVec_HFEnergy_iPhiNot39_depth2[ii].Fill(evt->HFDigiEnergy_()->at(ich)[2]);
              etaVec_HFCharge_iPhiNot39_depth2[ii].Fill(evt->HFDigiFC_()->at(ich)[2]);
            }

          }
        }
       
  /*
     
        // loop over time samples
        for(int its=0;its<nTS;its++){
             
          chVec_HFEnergy.at(ich)[its].Fill(evt->HFDigiEnergy_()->at(ich)[2]);  
        }
     
        //cout << "charge: " << evt->HFDigiFC_()->at(ich)[1] << endl;
  */
    }
     

    eventN++;
  }// End of loop over events



  // write the histograms
  sprintf(tempname,"%s/results_%s_%s.root",Outdir.c_str(),subSampleKey.c_str(),inputnumber.c_str());
  TFile *resFile = new TFile(tempname, "RECREATE");

  TDirectory *cdto_EtaDepthTS, *cdto_EtaDepth, *cdto_indivChip, *cdto_Integrated, *cdto_Integ_Eta;
  

  cdto_Integrated = resFile->mkdir("Integrated");
  cdto_Integrated->cd();

  for(int ii=0; ii < nEta; ii++){

    sprintf(tempname,"Eta%d",ieta[ii]);
    cdto_Integ_Eta = cdto_Integrated->mkdir(tempname);
    cdto_Integ_Eta->cd();
    etaVec_HFEnergy_iPhi39_depth1[ii].Write("HFEnergy_iPhi39_depth1");
    etaVec_HFEnergy_iPhiNot39_depth1[ii].Write("HFEnergy_iPhiNot39_depth1");
    etaVec_HFEnergy_iPhi39_depth2[ii].Write("HFEnergy_iPhi39_depth2");
    etaVec_HFEnergy_iPhiNot39_depth2[ii].Write("HFEnergy_iPhiNot39_depth2");

    etaVec_HFCharge_iPhi39_depth1[ii].Write("HFCharge_iPhi39_depth1");
    etaVec_HFCharge_iPhiNot39_depth1[ii].Write("HFCharge_iPhiNot39_depth1");
    etaVec_HFCharge_iPhi39_depth2[ii].Write("HFCharge_iPhi39_depth2");
    etaVec_HFCharge_iPhiNot39_depth2[ii].Write("HFCharge_iPhiNot39_depth2");

  }






/*
  cdto_indivChip = resFile->mkdir("individual_chips");
  cdto_indivChip->cd();

  // loop over channels
  for(int ich=0;ich<evt->HFDigiFC_()->size();ich++){ 

    sprintf(tempname,"iphi%d_ieta%d_depth%d",evt->HFDigiIPhi_()->at(ich),evt->HFDigiIEta_()->at(ich),evt->HFDigiDepth_()->at(ich));
    cdto_EtaDepth = cdto_indivChip->mkdir(tempname);
    cdto_EtaDepth->cd();

    // loop over time samples
    for(int its=0;its<nTS;its++){

      sprintf(tempname,"HFEnergy_ieta%d_depth%d_ts%d",evt->HFDigiIEta_()->at(ich),evt->HFDigiDepth_()->at(ich),its+2);
      chVec_HFEnergy[ich][its].Write(tempname);
    }

  }
*/

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

