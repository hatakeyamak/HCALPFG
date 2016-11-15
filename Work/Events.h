#ifndef EVENT_H
#define EVENT_H
#include "TTree.h"
#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include "TLorentzVector.h"
#include <stdio.h>
#include "TColor.h"
#include "TF1.h"
#include "TLegend.h"
#include "TVector3.h"
#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TVector2.h"
#include "TCanvas.h"
using namespace std;

class Events{

  // Declaration of leaf types
  TTree *         fChain;
  int             currentEntry_;
  int template_Entries; 
   vector<vector<double> > *QIE10DigiFC;
   vector<float>   *HFDigiEta;
   vector<float>   *HFDigiPhi;
   vector<float>   *HFDigiRecEnergy;
   vector<float>   *HFDigiRecTime;
   vector<vector<float> > *HFDigiAllFC;
   vector<vector<float> > *HFDigiEnergy;
   vector<vector<float> > *HFDigiFC;
   vector<vector<float> > *HFDigiGain;
   vector<vector<float> > *HFDigiNomFC;
   vector<vector<float> > *HFDigiPedFC;
   vector<vector<float> > *HFDigiRCGain;
   vector<int>     *HFDigiDepth;
   vector<int>     *HFDigiElectronicsID;
   vector<int>     *HFDigiFiberIdleOffset;
   vector<int>     *HFDigiIEta;
   vector<int>     *HFDigiIPhi;
   vector<int>     *HFDigiPresamples;
   vector<int>     *HFDigiRawID;
   vector<int>     *HFDigiSize;
   vector<int>     *HFDigiSubdet;
   vector<int>     *QIE10DigiCapIdOk;
   vector<int>     *QIE10DigiDepth;
   vector<int>     *QIE10DigiFlags;
   vector<int>     *QIE10DigiIEta;
   vector<int>     *QIE10DigiIPhi;
   vector<int>     *QIE10DigiLinkError;
   vector<int>     *QIE10DigiRawID;
   vector<int>     *QIE10DigiSubdet;
   vector<vector<int> > *HFDigiADC;
   vector<vector<int> > *HFDigiCapID;
   vector<vector<int> > *HFDigiDV;
   vector<vector<int> > *HFDigiER;
   vector<vector<int> > *HFDigiFiber;
   vector<vector<int> > *HFDigiFiberChan;
   vector<vector<int> > *HFDigiLADC;
   vector<vector<int> > *HFDigiRaw;
   vector<vector<int> > *QIE10DigiADC;
   vector<vector<int> > *QIE10DigiCapID;
   vector<vector<int> > *QIE10DigiLETDC;
   vector<vector<int> > *QIE10DigiOK;
   vector<vector<int> > *QIE10DigiSOI;
   vector<vector<int> > *QIE10DigiTETDC;
   UInt_t          bx;
   UInt_t          event;
   UInt_t          ls;
   UInt_t          run;

  
  public:


  //constructor
  Events(TTree * ttree_, const std::string sampleKeyString="HCal", int verbose=0);

  //functions
   bool loadNext();


   vector<vector<double> > *QIE10DigiFC_() const;
   vector<float>   *HFDigiEta_() const;
   vector<float>   *HFDigiPhi_() const;
   vector<float>   *HFDigiRecEnergy_() const;
   vector<float>   *HFDigiRecTime_() const;
   vector<vector<float> > *HFDigiAllFC_() const;
   vector<vector<float> > *HFDigiEnergy_() const;
   vector<vector<float> > *HFDigiFC_() const;
   vector<vector<float> > *HFDigiGain_() const;
   vector<vector<float> > *HFDigiNomFC_() const;
   vector<vector<float> > *HFDigiPedFC_() const;
   vector<vector<float> > *HFDigiRCGain_() const;
   vector<int>     *HFDigiDepth_() const;
   vector<int>     *HFDigiElectronicsID_() const;
   vector<int>     *HFDigiFiberIdleOffset_() const;
   vector<int>     *HFDigiIEta_() const;
   vector<int>     *HFDigiIPhi_() const;
   vector<int>     *HFDigiPresamples_() const;
   vector<int>     *HFDigiRawID_() const;
   vector<int>     *HFDigiSize_() const;
   vector<int>     *HFDigiSubdet_() const;
   vector<int>     *QIE10DigiDepth_() const;
   vector<int>     *QIE10DigiFlags_() const;
   vector<int>     *QIE10DigiIEta_() const;
   vector<int>     *QIE10DigiIPhi_() const;
   vector<int>     *QIE10DigiLinkError_() const;
   vector<int>     *QIE10DigiRawID_() const;
   vector<int>     *QIE10DigiSubdet_() const;
   vector<int>     *QIE10DigiCapIdOk_() const;
   vector<vector<int> > *HFDigiADC_() const;
   vector<vector<int> > *HFDigiCapID_() const;
   vector<vector<int> > *HFDigiDV_() const;
   vector<vector<int> > *HFDigiER_() const;
   vector<vector<int> > *HFDigiFiber_() const;
   vector<vector<int> > *HFDigiFiberChan_() const;
   vector<vector<int> > *HFDigiLADC_() const;
   vector<vector<int> > *HFDigiRaw_() const;
   vector<vector<int> > *QIE10DigiADC_() const;
   vector<vector<int> > *QIE10DigiCapID_() const;
   vector<vector<int> > *QIE10DigiLETDC_() const;
   vector<vector<int> > *QIE10DigiOK_() const;
   vector<vector<int> > *QIE10DigiSOI_() const;
   vector<vector<int> > *QIE10DigiTETDC_() const;
   UInt_t          bx_() const;
   UInt_t          event_() const;
   UInt_t          ls_() const;
   UInt_t          run_() const;


};//end of class Events
#endif
