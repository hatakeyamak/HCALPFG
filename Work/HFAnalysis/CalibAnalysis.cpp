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
double ADCtofC(double ADC);

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

  //KH -- to be updated with more accurate pedestal values from database
  double pedestal_QIE10_depth1_anodeA[13]={
    12.6824, 13.1416, 13.3694, 12.0163, 12.4801, 15.3118, 14.5098, 14.6575, 13.8828, 12.4334, 15.6986, 0, 14.6407};
  double pedestal_QIE10_depth2_anodeA[13]={
    12.9529, 14.4946, 14.7677, 15.7202, 14.7866, 16.3763, 14.1789, 16.5638, 15.4683, 15.864, 14.643, 0, 16.3341};
  double pedestal_QIE10_depth1_anodeB[13]={
    0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};
  double pedestal_QIE10_depth2_anodeB[13]={
    0., 15.8044, 0., 0., 0., 14.7922, 0., 0., 0., 0., 0., 0., 0.};

  //KH -- Natalia's phi symmetry calibration constants for HF+ iphi=39 QIE8 channels - 15/07/2016
  double calib_QIE8_iphi39_depth1[13]={
    0.00,0.55,0.74,0.86,0.77,1.15,0.99,1.01,1.05,1.09,1.14,0.00,0.00};
  double calib_QIE8_iphi39_depth2[13]={
    0.00,0.00,0.75,0.78,0.86,0.00,0.91,0.86,0.80,0.85,0.85,0.00,0.00};

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

  // read pedestal for QIE10s
  map<string,double> QIE10PedMap;
  fstream pedFile("pedestals_26QIE10.txt", std::fstream::in);
  string line;
  int eta__,phi__,dep__;
  string det__,DetId__;
  double cap0,cap1,cap2,cap3,widthcap0, widthcap1, widthcap2, widthcap3;  
  while (getline(pedFile, line)){
    if (line.empty()) continue;
    if (line[0] == '#') continue;
    stringstream ss; ss << line;
    ss >> eta__>>phi__>>dep__>>det__>>cap0>>cap1>>cap2>>cap3>>widthcap0>>widthcap1>>widthcap2>>widthcap3>>DetId__;
    sprintf(tempname,"eta%d_phi%d_dep%d_capid%d",eta__,phi__,dep__,0);
    QIE10PedMap[tempname]=ADCtofC(cap0);
    sprintf(tempname,"eta%d_phi%d_dep%d_capid%d",eta__,phi__,dep__,1);
    QIE10PedMap[tempname]=ADCtofC(cap1);
    sprintf(tempname,"eta%d_phi%d_dep%d_capid%d",eta__,phi__,dep__,2);
    QIE10PedMap[tempname]=ADCtofC(cap2);
    sprintf(tempname,"eta%d_phi%d_dep%d_capid%d",eta__,phi__,dep__,3);
    QIE10PedMap[tempname]=ADCtofC(cap3);    
    printf("eta%d_phi%d_dep%d: cap0: %g cap1: %g cap2: %g cap3: %g\n",eta__,phi__,dep__,cap0,cap1,cap2,cap3);
  } 
  pedFile.close();

  // a vector of histograms for each ieta
  map<string,TH1D> Map_RecEnergy;
  map<string,TH1D> Map_Charge;

  //
  // Loop over the events (tree entries)
  //
  int eventN=0;
  while( evt->loadNext() ){

    // only analyze a limited number of events
    //if(eventN>1000)break;

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
      if (iphi==39 || iphi==35 || iphi==43){
        sprintf(tempname,"ieta%d_iphi%d_depth%d",ieta,iphi,depth);
        Map_RecEnergy[tempname]=HFEnergy;
        sprintf(tempname,"Charge_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
        Map_Charge[tempname]=HFCharge;

        if(iphi==39){
	  sprintf(tempname,"ieta%d_iphinot%d_depth%d",ieta,iphi,depth);
	  Map_RecEnergy[tempname]=HFEnergy;

          sprintf(tempname,"AnodeA_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
          Map_RecEnergy[tempname]=HFEnergy;
          sprintf(tempname,"AnodeB_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
          Map_RecEnergy[tempname]=HFEnergy;

          sprintf(tempname,"AnodeB_QIE8x2_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
          Map_RecEnergy[tempname]=HFEnergy;
          sprintf(tempname,"AnodeB_QIE8calib_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
          Map_RecEnergy[tempname]=HFEnergy;

          sprintf(tempname,"Charge_AnodeA_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
          Map_Charge[tempname]=HFCharge;
          sprintf(tempname,"Charge_AnodeB_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
          Map_Charge[tempname]=HFCharge;
        }

      } // iphi==39 || iphi==35 || iphi==43
      } // eventN==0

      double rcgain = evt->HFDigiRCGain_()->at(ich)[2];
      /*
      std::cout << evt->HFDigiRCGain_()->at(ich)[0]
		<< evt->HFDigiRCGain_()->at(ich)[1]
		<< evt->HFDigiRCGain_()->at(ich)[2]
		<< evt->HFDigiRCGain_()->at(ich)[3] << std::endl;
      */

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

	      sprintf(tempname,"eta%d_phi%d_dep%d_capid%d",ieta,39,2,its_); 
	      double qie10a_ped=QIE10PedMap[tempname];
	      sprintf(tempname,"eta%d_phi%d_dep%d_capid%d",ieta,39,4,its_);
	      double qie10b_ped=QIE10PedMap[tempname];

	      /*
	      std::cout << qie10a_ped << " " 
			<< qie10b_ped << std::endl;
	      std::cout << pedestal_QIE10_depth2_anodeA[ieta_bin] << " " 
			<< pedestal_QIE10_depth2_anodeB[ieta_bin] << std::endl;
	      */

	      if (its>=1){ // TS1+2+3
              qie1010ab += evt->QIE10DigiFC_()->at(qie10chA)[its_]-qie10a_ped 
		          +evt->QIE10DigiFC_()->at(qie10chB)[its_]-qie10b_ped; // pedestal subtracted
              qie10a += evt->QIE10DigiFC_()->at(qie10chA)[its_]-qie10a_ped;    // pedestal subtracted
              qie10b += evt->QIE10DigiFC_()->at(qie10chB)[its_]-qie10b_ped;    // pedestal subtracted
	      }
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
	    double ped;           // pedestal
	    if (depth==2) ped=pedestal_QIE10_depth2_anodeA[ieta_bin];    // pedestal

            for(int its_=0;its_<4;its_++){

	      sprintf(tempname,"eta%d_phi%d_dep%d_capid%d",ieta,iphi,depth+2,its_); 
	      double qie10a_ped=QIE10PedMap[tempname];
	      
	      if (its>=1){ // TS1+2+3
              qie10a+=evt->QIE10DigiFC_()->at(qie10ch)[its_]-qie10a_ped;
              qie8b +=evt->HFDigiFC_()->at(ich)[its_];
	      }
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
	    sprintf(tempname,"AnodeB_QIE8x2_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
	    Map_RecEnergy[tempname].Fill(rcgain*(qie8b)*2);
	    //
	    double calib = calib_QIE8_iphi39_depth1[ieta_bin];
	    if (depth==2) calib = calib_QIE8_iphi39_depth2[ieta_bin];
	    sprintf(tempname,"AnodeB_QIE8calib_ieta%d_iphi%d_depth%d",ieta,iphi,depth);
	    Map_RecEnergy[tempname].Fill(rcgain*(qie8b)*2*calib);
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
        double qie8b=  //evt->HFDigiFC_()->at(ich)[0]+
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
	if (iphi==39 || iphi==35 || iphi==43)	
	  Map_RecEnergy[tempname].Fill(rcgain*(qie8b));
	
        sprintf(tempname,"ieta%d_iphinot39_depth%d",ieta,depth);
	Map_RecEnergy[tempname].Fill(rcgain*(qie8b));

        //if(evt->HFDigiFC_()->at(ich)[2]+evt->HFDigiPedFC_()->at(ich)[2] < 0.)cout << "HFFC: " << evt->HFDigiFC_()->at(ich)[2] << " HFPedFC: " << evt->HFDigiPedFC_()->at(ich)[2] << endl;
      } // iphi!=39

    }   // loop over channels

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
  //if(iphi!=35 && iphi!=39 && iphi!=43 )return true;
  if(ieta!=29&&ieta!=30&&ieta!=31&&ieta!=32&&ieta!=33&&ieta!=34&&ieta!=35&&ieta!=36&&ieta!=37&&ieta!=38&&ieta!=39&& ieta!=41)return true;

  return false; 
}

double ADCtofC(double ADC){

  // NEEDS UPDATING
  double adc2fC_QIE10[256]={
  // - - - - - - - range 0 - - - - - - - -
  //subrange0
  1.58, 4.73, 7.88, 11.0, 14.2, 17.3, 20.5, 23.6,
  26.8, 29.9, 33.1, 36.2, 39.4, 42.5, 45.7, 48.8,
  //subrange1
  53.6, 60.1, 66.6, 73.0, 79.5, 86.0, 92.5, 98.9,
  105, 112, 118, 125, 131, 138, 144, 151,
  //subrange2
  157, 164, 170, 177, 186, 199, 212, 225,
  238, 251, 264, 277, 289, 302, 315, 328,
  //subrange3
  341, 354, 367, 380, 393, 406, 418, 431,
  444, 464, 490, 516, 542, 568, 594, 620,

  // - - - - - - - range 1 - - - - - - - -
  //subrange0
  569, 594, 619, 645, 670, 695, 720, 745,
  771, 796, 821, 846, 871, 897, 922, 947,
  //subrange1
  960, 1010, 1060, 1120, 1170, 1220, 1270, 1320,
  1370, 1430, 1480, 1530, 1580, 1630, 1690, 1740,
  //subrange2
  1790, 1840, 1890, 1940,  2020, 2120, 2230, 2330,
  2430, 2540, 2640, 2740, 2850, 2950, 3050, 3150,
  //subrange3
  3260, 3360, 3460, 3570, 3670, 3770, 3880, 3980,
  4080, 4240, 4450, 4650, 4860, 5070, 5280, 5490,

  // - - - - - - - range 2 - - - - - - - -
  //subrange0
  5080, 5280, 5480, 5680, 5880, 6080, 6280, 6480,
  6680, 6890, 7090, 7290, 7490, 7690, 7890, 8090,
  //subrange1
  8400, 8810, 9220, 9630, 10000, 10400, 10900, 11300,
  11700, 12100, 12500, 12900, 13300, 13700, 14100, 14500,
  //subrange2
  15000, 15400, 15800, 16200, 16800, 17600, 18400, 19300,
  20100, 20900, 21700, 22500, 23400, 24200, 25000, 25800,
  //subrange3
  26600, 27500, 28300, 29100, 29900, 30700, 31600, 32400,
  33200, 34400, 36100, 37700, 39400, 41000, 42700, 44300,

  // - - - - - - - range 3 - - - - - - - - -
  //subrange0
  41100, 42700, 44300, 45900, 47600, 49200, 50800, 52500,
  54100, 55700, 57400, 59000, 60600, 62200, 63900, 65500,
  //subrange1
  68000, 71300, 74700, 78000, 81400, 84700, 88000, 91400,
  94700, 98100, 101000, 105000, 108000, 111000, 115000, 118000,
  //subrange2
  121000, 125000, 128000, 131000, 137000, 145000, 152000, 160000,
  168000, 176000, 183000, 191000, 199000, 206000, 214000, 222000,
  //subrange3
  230000, 237000, 245000, 253000, 261000, 268000, 276000, 284000,
  291000, 302000, 316000, 329000, 343000, 356000, 370000, 384000
  };

  int ADC_lo = floor(ADC);
  int ADC_hi = ceil(ADC);
  
  double Charge_lo = adc2fC_QIE10[ADC_lo];
  double Charge_hi = adc2fC_QIE10[ADC_hi];
  
  double Charge = Charge_lo;
  if (ADC_hi>ADC_lo) 
    Charge += (Charge_hi-Charge_lo)
      *(ADC-double(ADC_lo))/(double(ADC_hi-ADC_lo));

  return Charge;
  
}
