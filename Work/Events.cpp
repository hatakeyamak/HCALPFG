#include "Events.h"
#include <vector>


  //Constructor
  Events::Events(TTree * ttree_, const std::string sampleKeyString, int verbose) : currentEntry_(-1) {

   // Set object pointer
   QIE10DigiFC = 0;
   HFDigiEta = 0;
   HFDigiPhi = 0;
   HFDigiRecEnergy = 0;
   HFDigiRecTime = 0;
   HFDigiAllFC = 0;
   HFDigiEnergy = 0;
   HFDigiFC = 0;
   HFDigiGain = 0;
   HFDigiNomFC = 0;
   HFDigiPedFC = 0;
   HFDigiRCGain = 0;
   HFDigiDepth = 0;
   HFDigiElectronicsID = 0;
   HFDigiFiberIdleOffset = 0;
   HFDigiIEta = 0;
   HFDigiIPhi = 0;
   HFDigiPresamples = 0;
   HFDigiRawID = 0;
   HFDigiSize = 0;
   HFDigiSubdet = 0;
   QIE10DigiDepth = 0;
   QIE10DigiFlags = 0;
   QIE10DigiIEta = 0;
   QIE10DigiIPhi = 0;
   QIE10DigiLinkError = 0;
   QIE10DigiRawID = 0;
   QIE10DigiSubdet = 0;
   QIE10DigiCapIdOk = 0;
   HFDigiADC = 0;
   HFDigiCapID = 0;
   HFDigiDV = 0;
   HFDigiER = 0;
   HFDigiFiber = 0;
   HFDigiFiberChan = 0;
   HFDigiLADC = 0;
   HFDigiRaw = 0;
   QIE10DigiADC = 0;
   QIE10DigiCapID = 0;
   QIE10DigiLETDC = 0;
   QIE10DigiOK = 0;
   QIE10DigiSOI = 0;
   QIE10DigiTETDC = 0;


   fChain = ttree_;

   // Set branch addresses and branch pointers
   fChain->SetBranchAddress("QIE10DigiFC", &QIE10DigiFC);
   fChain->SetBranchAddress("HFDigiEta", &HFDigiEta);
   fChain->SetBranchAddress("HFDigiPhi", &HFDigiPhi);
   fChain->SetBranchAddress("HFDigiRecEnergy", &HFDigiRecEnergy);
   fChain->SetBranchAddress("HFDigiRecTime", &HFDigiRecTime);
   fChain->SetBranchAddress("HFDigiAllFC", &HFDigiAllFC);
   fChain->SetBranchAddress("HFDigiEnergy", &HFDigiEnergy);
   fChain->SetBranchAddress("HFDigiFC", &HFDigiFC);
   fChain->SetBranchAddress("HFDigiGain", &HFDigiGain);
   fChain->SetBranchAddress("HFDigiNomFC", &HFDigiNomFC);
   fChain->SetBranchAddress("HFDigiPedFC", &HFDigiPedFC);
   fChain->SetBranchAddress("HFDigiRCGain", &HFDigiRCGain);
   fChain->SetBranchAddress("HFDigiDepth", &HFDigiDepth);
   fChain->SetBranchAddress("HFDigiElectronicsID", &HFDigiElectronicsID);
   fChain->SetBranchAddress("HFDigiFiberIdleOffset", &HFDigiFiberIdleOffset);
   fChain->SetBranchAddress("HFDigiIEta", &HFDigiIEta);
   fChain->SetBranchAddress("HFDigiIPhi", &HFDigiIPhi);
   fChain->SetBranchAddress("HFDigiPresamples", &HFDigiPresamples);
   fChain->SetBranchAddress("HFDigiRawID", &HFDigiRawID);
   fChain->SetBranchAddress("HFDigiSize", &HFDigiSize);
   fChain->SetBranchAddress("HFDigiSubdet", &HFDigiSubdet);
   fChain->SetBranchAddress("QIE10DigiDepth", &QIE10DigiDepth);
   fChain->SetBranchAddress("QIE10DigiFlags", &QIE10DigiFlags);
   fChain->SetBranchAddress("QIE10DigiIEta", &QIE10DigiIEta);
   fChain->SetBranchAddress("QIE10DigiIPhi", &QIE10DigiIPhi);
   fChain->SetBranchAddress("QIE10DigiLinkError", &QIE10DigiLinkError);
   fChain->SetBranchAddress("QIE10DigiRawID", &QIE10DigiRawID);
   fChain->SetBranchAddress("QIE10DigiSubdet", &QIE10DigiSubdet);
   fChain->SetBranchAddress("QIE10DigiCapIdOk", &QIE10DigiCapIdOk);
   fChain->SetBranchAddress("HFDigiADC", &HFDigiADC);
   fChain->SetBranchAddress("HFDigiCapID", &HFDigiCapID);
   fChain->SetBranchAddress("HFDigiDV", &HFDigiDV);
   fChain->SetBranchAddress("HFDigiER", &HFDigiER);
   fChain->SetBranchAddress("HFDigiFiber", &HFDigiFiber);
   fChain->SetBranchAddress("HFDigiFiberChan", &HFDigiFiberChan);
   fChain->SetBranchAddress("HFDigiLADC", &HFDigiLADC);
   fChain->SetBranchAddress("HFDigiRaw", &HFDigiRaw);
   fChain->SetBranchAddress("QIE10DigiADC", &QIE10DigiADC);
   fChain->SetBranchAddress("QIE10DigiCapID", &QIE10DigiCapID);
   fChain->SetBranchAddress("QIE10DigiLETDC", &QIE10DigiLETDC);
   fChain->SetBranchAddress("QIE10DigiOK", &QIE10DigiOK);
   fChain->SetBranchAddress("QIE10DigiSOI", &QIE10DigiSOI);
   fChain->SetBranchAddress("QIE10DigiTETDC", &QIE10DigiTETDC);
   fChain->SetBranchAddress("bx", &bx);
   fChain->SetBranchAddress("event", &event);
   fChain->SetBranchAddress("ls", &ls);
   fChain->SetBranchAddress("run", &run);
   


    // Number of total entries
    template_Entries = fChain->GetEntries();

  }//end of class constructor Event


  // A function to make events available
  bool Events::loadNext() {

    if( currentEntry_ < 0 ) {
      std::cout << "Processing " << template_Entries << " events" << std::endl;
    }

    ++currentEntry_;

    if( currentEntry_ == template_Entries ) {

      return false;

    } else {

      if( currentEntry_ > 0 && currentEntry_%10000 == 0 ) {
        std::cout << " Processed " << currentEntry_ << " events / " << template_Entries << std::endl;
      }

      fChain->GetEntry(currentEntry_);
      return true;

    }


  }

   UInt_t          Events::run_() const{return run;}
   vector<vector<double> > *Events::QIE10DigiFC_() const{return QIE10DigiFC;}
   vector<float>   *Events::HFDigiEta_() const{return HFDigiEta;}
   vector<float>   *Events::HFDigiPhi_() const{return HFDigiPhi;}
   vector<float>   *Events::HFDigiRecEnergy_() const{return HFDigiRecEnergy;}
   vector<float>   *Events::HFDigiRecTime_() const{return HFDigiRecTime;}
   vector<vector<float> > *Events::HFDigiAllFC_() const{return HFDigiAllFC;}
   vector<vector<float> > *Events::HFDigiEnergy_() const{return HFDigiEnergy;}
   vector<vector<float> > *Events::HFDigiFC_() const{return HFDigiFC;}
   vector<vector<float> > *Events::HFDigiGain_() const{return HFDigiGain;}
   vector<vector<float> > *Events::HFDigiNomFC_() const{return HFDigiNomFC;}
   vector<vector<float> > *Events::HFDigiPedFC_() const{return HFDigiPedFC;}
   vector<vector<float> > *Events::HFDigiRCGain_() const{return HFDigiRCGain;}
   vector<int>     *Events::HFDigiDepth_() const{return HFDigiDepth;}
   vector<int>     *Events::HFDigiElectronicsID_() const{return HFDigiElectronicsID;}
   vector<int>     *Events::HFDigiFiberIdleOffset_() const{return HFDigiFiberIdleOffset;}
   vector<int>     *Events::HFDigiIEta_() const{return HFDigiIEta;}
   vector<int>     *Events::HFDigiIPhi_() const{return HFDigiIPhi;}
   vector<int>     *Events::HFDigiPresamples_() const{return HFDigiPresamples;}
   vector<int>     *Events::HFDigiRawID_() const{return HFDigiRawID;}
   vector<int>     *Events::HFDigiSize_() const{return HFDigiSize;}
   vector<int>     *Events::HFDigiSubdet_() const{return HFDigiSubdet;}
   vector<int>     *Events::QIE10DigiDepth_() const{return QIE10DigiDepth;}
   vector<int>     *Events::QIE10DigiFlags_() const{return QIE10DigiFlags;}
   vector<int>     *Events::QIE10DigiIEta_() const{return QIE10DigiIEta;}
   vector<int>     *Events::QIE10DigiIPhi_() const{return QIE10DigiIPhi;}
   vector<int>     *Events::QIE10DigiLinkError_() const{return QIE10DigiLinkError;}
   vector<int>     *Events::QIE10DigiRawID_() const{return QIE10DigiRawID;}
   vector<int>     *Events::QIE10DigiSubdet_() const{return QIE10DigiSubdet;}
   vector<int>     *Events::QIE10DigiCapIdOk_() const{return QIE10DigiCapIdOk;}
   vector<vector<int> > *Events::HFDigiADC_() const{return HFDigiADC;}
   vector<vector<int> > *Events::HFDigiCapID_() const{return HFDigiCapID;}
   vector<vector<int> > *Events::HFDigiDV_() const{return HFDigiDV;}
   vector<vector<int> > *Events::HFDigiER_() const{return HFDigiER;}
   vector<vector<int> > *Events::HFDigiFiber_() const{return HFDigiFiber;}
   vector<vector<int> > *Events::HFDigiFiberChan_() const{return HFDigiFiberChan;}
   vector<vector<int> > *Events::HFDigiLADC_() const{return HFDigiLADC;}
   vector<vector<int> > *Events::HFDigiRaw_() const{return HFDigiRaw;}
   vector<vector<int> > *Events::QIE10DigiADC_() const{return QIE10DigiADC;}
   vector<vector<int> > *Events::QIE10DigiCapID_() const{return QIE10DigiCapID;}
   vector<vector<int> > *Events::QIE10DigiLETDC_() const{return QIE10DigiLETDC;}
   vector<vector<int> > *Events::QIE10DigiOK_() const{return QIE10DigiOK;}
   vector<vector<int> > *Events::QIE10DigiSOI_() const{return QIE10DigiSOI;}
   vector<vector<int> > *Events::QIE10DigiTETDC_() const{return QIE10DigiTETDC;}
   UInt_t          Events::bx_() const{return bx;}
   UInt_t          Events::event_() const{return event;}
   UInt_t          Events::ls_() const{return ls;}


//
