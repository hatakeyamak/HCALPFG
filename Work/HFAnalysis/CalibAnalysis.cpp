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

int QIE10map(double eta, double phi, double depth);
int QIE8map(double eta, double phi, double depth);
bool ChanSel(int ieta, int iphi, int dep);

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

  //KH 
  double pedestal_QIE10_depth1_anodeA[13]={
    12.6824, 13.1416, 13.3694, 12.0163, 12.4801, 15.3118, 14.5098, 14.6575, 13.8828, 12.4334, 15.6986, 0, 14.6407};
  double pedestal_QIE10_depth2_anodeA[13]={
    12.9529, 14.4946, 14.7677, 15.7202, 14.7866, 16.3763, 14.1789, 16.5638, 15.4683, 15.864, 14.643, 0, 16.3341};
  double pedestal_QIE10_depth1_anodeB[13]={
    0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};
  double pedestal_QIE10_depth2_anodeB[13]={
    0., 15.8044, 0., 0., 0., 14.7922, 0., 0., 0., 0., 0., 0., 0.};

  //some varaibles
  char filenames[500];
  vector<string> filesVec;
  ifstream fin(InRootList.c_str());
  TChain *sample_AUX = new TChain("hcalTupleTree/tree");

  char tempname[200];


  ///read the file names from the .txt files and load them to a vector.
  while(fin.getline(filenames, 500) ){filesVec.push_back(filenames);}
  cout<< "\nProcessing " << subSampleKey << " ... " << endl;

  for(unsigned int in=0; in<filesVec.size(); in++){ sample_AUX->Add(filesVec.at(in).c_str()); }

  // --- Analyse the events --------------------------------------------

  // Interface to the event content
  Events * evt = new Events(sample_AUX, subSampleKey,verbose);

  // define some histograms
  TH1D  HFEnergy      =  TH1D("HFEnergy","",105,-50.,1000.); // 10 GeV bin
  TH1D  HFCharge      =  TH1D("HFCharge","",200,0.,100.);    // 1 fC bin

  // a vector of histograms for each ieta
  map<string,TH1D> Map_RecEnergy;
  map<string,TH1D> Map_Charge;

  // Loop over the events (tree entries)
  int eventN=0;
  while( evt->loadNext() ){

    // only analyze a limited number of events
    //if(eventN>10000)break;

    //if(eventN%1000==0)cout<< " ############# \nEvent#: " << eventN << endl; 

    /*
    std::cout << evt->HFDigiFC_()->size() << " "
	      << evt->QIE10DigiFC_()->size() << std::endl;
    */

    // loop over channels
    for(int ich=0;ich<evt->HFDigiFC_()->size();ich++){
      int ieta = evt->HFDigiIEta_()->at(ich);
      int iphi = evt->HFDigiIPhi_()->at(ich);
      int depth= evt->HFDigiDepth_()->at(ich);

      // only analayze a selected set of channels
      if(ChanSel(ieta,iphi,depth))continue;

      //printf("ieta%d_iphi%d_depth%d: ich: %d \n",ieta,iphi,depth,ich);

      // initialize histograms
      if(eventN==0){
        sprintf(tempname,"ieta%d_iphi%d_depth%d",ieta,iphi,depth);
        Map_RecEnergy[tempname]=HFEnergy;
        sprintf(tempname,"Charge_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
        Map_Charge[tempname]=HFCharge;
        if(iphi==39){
          sprintf(tempname,"AnodeA_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
          Map_RecEnergy[tempname]=HFEnergy;
          sprintf(tempname,"AnodeB_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
          Map_RecEnergy[tempname]=HFEnergy;
          sprintf(tempname,"Charge_AnodeA_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
          Map_Charge[tempname]=HFCharge;
          sprintf(tempname,"Charge_AnodeB_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
          Map_Charge[tempname]=HFCharge;
        }
      }

      double rcgain = evt->HFDigiRCGain_()->at(ich)[2];
      std::cout << evt->HFDigiRCGain_()->at(ich)[0]
		<< evt->HFDigiRCGain_()->at(ich)[1]
		<< evt->HFDigiRCGain_()->at(ich)[2]
		<< evt->HFDigiRCGain_()->at(ich)[3] << std::endl;

      //
      // both qie10 anods 
      //
      if(iphi==39&&(ieta==30||ieta==34)&&depth==2){
	/*
	std::cout << "aaa: " << ieta << " " << iphi << " " << depth << std::endl;
	printf("%3d %3d %3d %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e\n",ieta,iphi,depth,
	       evt->HFDigiFC_()->at(ich)[0],evt->HFDigiFC_()->at(ich)[1],
	       evt->HFDigiFC_()->at(ich)[2],evt->HFDigiFC_()->at(ich)[3],
	       evt->HFDigiPedFC_()->at(ich)[0],evt->HFDigiPedFC_()->at(ich)[1],
	       evt->HFDigiPedFC_()->at(ich)[2],evt->HFDigiPedFC_()->at(ich)[3]
	       );
	*/
        int qie10chA=QIE10map(ieta, iphi,2);
        int qie10chB=QIE10map(ieta, iphi,4);
        //printf("chA: %d ieta: %d iphi: %d depth: %d\n",qie10chA,evt->QIE10DigiIEta_()->at(qie10chA),evt->QIE10DigiIPhi_()->at(qie10chA),evt->QIE10DigiDepth_()->at(qie10chA));
        //printf("chB: %d ieta: %d iphi: %d depth: %d\n",qie10chB,evt->QIE10DigiIEta_()->at(qie10chB),evt->QIE10DigiIPhi_()->at(qie10chB),evt->QIE10DigiDepth_()->at(qie10chB));
        for(int its=0;its<4;its++){
          if(evt->QIE10DigiSOI_()->at(qie10chB)[its]==1){
            double qie1010ab=0., qie10a=0., qie10b=0.;
            for(int its_=0;its_<4;its_++){
	      int ieta_bin=ieta-29;
              qie1010ab += evt->QIE10DigiFC_()->at(qie10chA)[its_]-pedestal_QIE10_depth2_anodeA[ieta_bin] 
		          +evt->QIE10DigiFC_()->at(qie10chB)[its_]-pedestal_QIE10_depth2_anodeB[ieta_bin]; // pedestal subtracted
              qie10a += evt->QIE10DigiFC_()->at(qie10chA)[its_]-pedestal_QIE10_depth2_anodeA[ieta_bin];    // pedestal subtracted
              qie10b += evt->QIE10DigiFC_()->at(qie10chB)[its_]-pedestal_QIE10_depth2_anodeB[ieta_bin];    // pedestal subtracted
            }
            //printf("1010:\nFCa: %g FCb: %g \n",evt->QIE10DigiFC_()->at(qie10chA)[its],evt->QIE10DigiFC_()->at(qie10chB)[its]);
            sprintf(tempname,"ieta%d_iphi%d_depth%d",ieta,iphi,depth);
            //Map_RecEnergy[tempname].Fill(rcgain*(evt->QIE10DigiFC_()->at(qie10chA)[its]+evt->QIE10DigiFC_()->at(qie10chB)[its]));
            Map_RecEnergy[tempname].Fill(rcgain*(qie1010ab));
            sprintf(tempname,"AnodeA_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
            //Map_RecEnergy[tempname].Fill(rcgain*evt->QIE10DigiFC_()->at(qie10chA)[its]);
            Map_RecEnergy[tempname].Fill(rcgain*qie10a);
            sprintf(tempname,"AnodeB_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
            //Map_RecEnergy[tempname].Fill(rcgain*evt->QIE10DigiFC_()->at(qie10chB)[its]);
            Map_RecEnergy[tempname].Fill(rcgain*qie10b);
	    //
            sprintf(tempname,"Charge_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
            Map_Charge[tempname].Fill(evt->QIE10DigiFC_()->at(qie10chA)[0]+evt->QIE10DigiFC_()->at(qie10chB)[0]);
            sprintf(tempname,"Charge_AnodeA_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
            Map_Charge[tempname].Fill(evt->QIE10DigiFC_()->at(qie10chA)[0]);
            sprintf(tempname,"Charge_AnodeB_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
            Map_Charge[tempname].Fill(evt->QIE10DigiFC_()->at(qie10chB)[0]);
          }
        }
      }
      else if(iphi==39){
        rcgain/=2.;
        int qie10ch=QIE10map(ieta, iphi,depth+2);
        //printf("chA: %d ieta: %d iphi: %d depth: %d\n",qie10ch,evt->QIE10DigiIEta_()->at(qie10ch),evt->QIE10DigiIPhi_()->at(qie10ch),evt->QIE10DigiDepth_()->at(qie10ch));
        for(int its=0;its<4;its++){
          if(evt->QIE10DigiSOI_()->at(qie10ch)[its]==1){
            double qie10a=0.,qie8b=0.;
	    int ieta_bin=ieta-29;
	    double ped=pedestal_QIE10_depth1_anodeA[ieta_bin];           // pedestal
	    if (depth==2) ped=pedestal_QIE10_depth2_anodeA[ieta_bin];    // pedestal
            for(int its_=0;its_<4;its_++){
              qie10a+=evt->QIE10DigiFC_()->at(qie10ch)[its_]-ped;
              qie8b +=evt->HFDigiFC_()->at(ich)[its_];
            }
            //printf("108:\nFCa: %g FCb: %g \n",evt->QIE10DigiFC_()->at(qie10ch)[its],evt->HFDigiFC_()->at(ich)[2]);
            sprintf(tempname,"ieta%d_iphi%d_depth%d",ieta,iphi,depth);
            //Map_RecEnergy[tempname].Fill(rcgain*(evt->QIE10DigiFC_()->at(qie10ch)[its]+evt->HFDigiFC_()->at(ich)[2]+evt->HFDigiPedFC_()->at(ich)[2]));
            Map_RecEnergy[tempname].Fill(rcgain*(qie10a+qie8b));
            sprintf(tempname,"AnodeA_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
            //Map_RecEnergy[tempname].Fill(rcgain*(evt->QIE10DigiFC_()->at(qie10ch)[its]));
            Map_RecEnergy[tempname].Fill(rcgain*(qie10a));
            sprintf(tempname,"AnodeB_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
            //Map_RecEnergy[tempname].Fill(rcgain*(evt->HFDigiFC_()->at(ich)[2]+evt->HFDigiPedFC_()->at(ich)[2]));
            Map_RecEnergy[tempname].Fill(rcgain*(qie8b));
	    //
            sprintf(tempname,"Charge_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
            Map_Charge[tempname].Fill(evt->QIE10DigiFC_()->at(qie10ch)[0]+evt->HFDigiFC_()->at(ich)[0]);
            sprintf(tempname,"Charge_AnodeA_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
            Map_Charge[tempname].Fill(evt->QIE10DigiFC_()->at(qie10ch)[0]);
            sprintf(tempname,"Charge_AnodeB_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
            Map_Charge[tempname].Fill(evt->HFDigiFC_()->at(ich)[0]);
          }
        }
	/*
	printf("%3d %3d %3d %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e\n",ieta,iphi,depth,
	       evt->HFDigiFC_()->at(ich)[0],evt->HFDigiFC_()->at(ich)[1],
	       evt->HFDigiFC_()->at(ich)[2],evt->HFDigiFC_()->at(ich)[3],
	       evt->HFDigiPedFC_()->at(ich)[0],evt->HFDigiPedFC_()->at(ich)[1],
	       evt->HFDigiPedFC_()->at(ich)[2],evt->HFDigiPedFC_()->at(ich)[3]
	       );
	*/
      }
      else if(iphi!=39){
        sprintf(tempname,"ieta%d_iphi%d_depth%d",ieta,iphi,depth);
        //printf("8:\nFC: %g \n",evt->HFDigiFC_()->at(ich)[2]);
        double qie8b=evt->HFDigiFC_()->at(ich)[0]+
                     evt->HFDigiFC_()->at(ich)[1]+
                     evt->HFDigiFC_()->at(ich)[2]+
                     evt->HFDigiFC_()->at(ich)[3];
	/*
	printf("%3d %3d %3d %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e %10.3e\n",ieta,iphi,depth,
	       evt->HFDigiFC_()->at(ich)[0],evt->HFDigiFC_()->at(ich)[1],
	       evt->HFDigiFC_()->at(ich)[2],evt->HFDigiFC_()->at(ich)[3],
	       evt->HFDigiPedFC_()->at(ich)[0],evt->HFDigiPedFC_()->at(ich)[1],
	       evt->HFDigiPedFC_()->at(ich)[2],evt->HFDigiPedFC_()->at(ich)[3]
	       );
	*/
        //Map_RecEnergy[tempname].Fill(rcgain*(evt->HFDigiFC_()->at(ich)[2]+evt->HFDigiPedFC_()->at(ich)[2])); // pedestal is added back
        Map_RecEnergy[tempname].Fill(rcgain*(qie8b));
        //if(evt->HFDigiFC_()->at(ich)[2]+evt->HFDigiPedFC_()->at(ich)[2] < 0.)cout << "HFFC: " << evt->HFDigiFC_()->at(ich)[2] << " HFPedFC: " << evt->HFDigiPedFC_()->at(ich)[2] << endl;
      }




    }

    eventN++;
  }// End of loop over events



  // write the histograms
  sprintf(tempname,"%s/results_%s_%s.root",Outdir.c_str(),subSampleKey.c_str(),inputnumber.c_str());
  TFile *resFile = new TFile(tempname, "RECREATE");

  TDirectory *cdto_EtaDepthTS, *cdto_EtaDepth, *cdto_indivChip, *cdto_Integrated, *cdto_Integ_Eta;
  
  for(map<string, TH1D>::iterator it=Map_RecEnergy.begin(); it!=Map_RecEnergy.end();it++){
    it->second.Write(it->first.c_str());
  }    
  for(map<string, TH1D>::iterator it=Map_Charge.begin(); it!=Map_Charge.end();it++){
    it->second.Write(it->first.c_str());
  }    

  resFile->Close();



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

bool ChanSel(int ieta, int iphi, int dep){

  //if(dep!=2) return true;
  if(iphi!=35 && iphi!=39 && iphi!=43 )return true;
  if(ieta!=29&&ieta!=30&&ieta!=31&&ieta!=32&&ieta!=33&&ieta!=34&&ieta!=35&&ieta!=36&&ieta!=37&&ieta!=38&&ieta!=39&& ieta!=41)return true;

  return false; 
}
