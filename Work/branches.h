//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Aug 18 03:02:34 2016 by ROOT version 6.06/01
// from TTree tree/
// found on file: /data3/store/user/borzou/ntuples/Aug_2016_run278406v3/JetHT/testName/0000/HcalTupleMaker_22.root
//////////////////////////////////////////////////////////

#ifndef branches_h
#define branches_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
#include "vector"
#include "vector"
#include "vector"

class branches {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
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

   // List of branches
   TBranch        *b_QIE10DigiFC;   //!
   TBranch        *b_HFDigiEta;   //!
   TBranch        *b_HFDigiPhi;   //!
   TBranch        *b_HFDigiRecEnergy;   //!
   TBranch        *b_HFDigiRecTime;   //!
   TBranch        *b_HFDigiAllFC;   //!
   TBranch        *b_HFDigiEnergy;   //!
   TBranch        *b_HFDigiFC;   //!
   TBranch        *b_HFDigiGain;   //!
   TBranch        *b_HFDigiNomFC;   //!
   TBranch        *b_HFDigiPedFC;   //!
   TBranch        *b_HFDigiRCGain;   //!
   TBranch        *b_HFDigiDepth;   //!
   TBranch        *b_HFDigiElectronicsID;   //!
   TBranch        *b_HFDigiFiberIdleOffset;   //!
   TBranch        *b_HFDigiIEta;   //!
   TBranch        *b_HFDigiIPhi;   //!
   TBranch        *b_HFDigiPresamples;   //!
   TBranch        *b_HFDigiRawID;   //!
   TBranch        *b_HFDigiSize;   //!
   TBranch        *b_HFDigiSubdet;   //!
   TBranch        *b_QIE10DigiCapIdOk;   //!
   TBranch        *b_QIE10DigiDepth;   //!
   TBranch        *b_QIE10DigiFlags;   //!
   TBranch        *b_QIE10DigiIEta;   //!
   TBranch        *b_QIE10DigiIPhi;   //!
   TBranch        *b_QIE10DigiLinkError;   //!
   TBranch        *b_QIE10DigiRawID;   //!
   TBranch        *b_QIE10DigiSubdet;   //!
   TBranch        *b_HFDigiADC;   //!
   TBranch        *b_HFDigiCapID;   //!
   TBranch        *b_HFDigiDV;   //!
   TBranch        *b_HFDigiER;   //!
   TBranch        *b_HFDigiFiber;   //!
   TBranch        *b_HFDigiFiberChan;   //!
   TBranch        *b_HFDigiLADC;   //!
   TBranch        *b_HFDigiRaw;   //!
   TBranch        *b_QIE10DigiADC;   //!
   TBranch        *b_QIE10DigiCapID;   //!
   TBranch        *b_QIE10DigiLETDC;   //!
   TBranch        *b_QIE10DigiOK;   //!
   TBranch        *b_QIE10DigiSOI;   //!
   TBranch        *b_QIE10DigiTETDC;   //!
   TBranch        *b_bx;   //!
   TBranch        *b_event;   //!
   TBranch        *b_ls;   //!
   TBranch        *b_run;   //!

   branches(TTree *tree=0);
   virtual ~branches();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef branches_cxx
branches::branches(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/data3/store/user/borzou/ntuples/Aug_2016_run278406v3/JetHT/testName/0000/HcalTupleMaker_22.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/data3/store/user/borzou/ntuples/Aug_2016_run278406v3/JetHT/testName/0000/HcalTupleMaker_22.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("/data3/store/user/borzou/ntuples/Aug_2016_run278406v3/JetHT/testName/0000/HcalTupleMaker_22.root:/hcalTupleTree");
      dir->GetObject("tree",tree);

   }
   Init(tree);
}

branches::~branches()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t branches::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t branches::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void branches::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

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
   QIE10DigiCapIdOk = 0;
   QIE10DigiDepth = 0;
   QIE10DigiFlags = 0;
   QIE10DigiIEta = 0;
   QIE10DigiIPhi = 0;
   QIE10DigiLinkError = 0;
   QIE10DigiRawID = 0;
   QIE10DigiSubdet = 0;
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
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("QIE10DigiFC", &QIE10DigiFC, &b_QIE10DigiFC);
   fChain->SetBranchAddress("HFDigiEta", &HFDigiEta, &b_HFDigiEta);
   fChain->SetBranchAddress("HFDigiPhi", &HFDigiPhi, &b_HFDigiPhi);
   fChain->SetBranchAddress("HFDigiRecEnergy", &HFDigiRecEnergy, &b_HFDigiRecEnergy);
   fChain->SetBranchAddress("HFDigiRecTime", &HFDigiRecTime, &b_HFDigiRecTime);
   fChain->SetBranchAddress("HFDigiAllFC", &HFDigiAllFC, &b_HFDigiAllFC);
   fChain->SetBranchAddress("HFDigiEnergy", &HFDigiEnergy, &b_HFDigiEnergy);
   fChain->SetBranchAddress("HFDigiFC", &HFDigiFC, &b_HFDigiFC);
   fChain->SetBranchAddress("HFDigiGain", &HFDigiGain, &b_HFDigiGain);
   fChain->SetBranchAddress("HFDigiNomFC", &HFDigiNomFC, &b_HFDigiNomFC);
   fChain->SetBranchAddress("HFDigiPedFC", &HFDigiPedFC, &b_HFDigiPedFC);
   fChain->SetBranchAddress("HFDigiRCGain", &HFDigiRCGain, &b_HFDigiRCGain);
   fChain->SetBranchAddress("HFDigiDepth", &HFDigiDepth, &b_HFDigiDepth);
   fChain->SetBranchAddress("HFDigiElectronicsID", &HFDigiElectronicsID, &b_HFDigiElectronicsID);
   fChain->SetBranchAddress("HFDigiFiberIdleOffset", &HFDigiFiberIdleOffset, &b_HFDigiFiberIdleOffset);
   fChain->SetBranchAddress("HFDigiIEta", &HFDigiIEta, &b_HFDigiIEta);
   fChain->SetBranchAddress("HFDigiIPhi", &HFDigiIPhi, &b_HFDigiIPhi);
   fChain->SetBranchAddress("HFDigiPresamples", &HFDigiPresamples, &b_HFDigiPresamples);
   fChain->SetBranchAddress("HFDigiRawID", &HFDigiRawID, &b_HFDigiRawID);
   fChain->SetBranchAddress("HFDigiSize", &HFDigiSize, &b_HFDigiSize);
   fChain->SetBranchAddress("HFDigiSubdet", &HFDigiSubdet, &b_HFDigiSubdet);
   fChain->SetBranchAddress("QIE10DigiCapIdOk", &QIE10DigiCapIdOk, &b_QIE10DigiCapIdOk);
   fChain->SetBranchAddress("QIE10DigiDepth", &QIE10DigiDepth, &b_QIE10DigiDepth);
   fChain->SetBranchAddress("QIE10DigiFlags", &QIE10DigiFlags, &b_QIE10DigiFlags);
   fChain->SetBranchAddress("QIE10DigiIEta", &QIE10DigiIEta, &b_QIE10DigiIEta);
   fChain->SetBranchAddress("QIE10DigiIPhi", &QIE10DigiIPhi, &b_QIE10DigiIPhi);
   fChain->SetBranchAddress("QIE10DigiLinkError", &QIE10DigiLinkError, &b_QIE10DigiLinkError);
   fChain->SetBranchAddress("QIE10DigiRawID", &QIE10DigiRawID, &b_QIE10DigiRawID);
   fChain->SetBranchAddress("QIE10DigiSubdet", &QIE10DigiSubdet, &b_QIE10DigiSubdet);
   fChain->SetBranchAddress("HFDigiADC", &HFDigiADC, &b_HFDigiADC);
   fChain->SetBranchAddress("HFDigiCapID", &HFDigiCapID, &b_HFDigiCapID);
   fChain->SetBranchAddress("HFDigiDV", &HFDigiDV, &b_HFDigiDV);
   fChain->SetBranchAddress("HFDigiER", &HFDigiER, &b_HFDigiER);
   fChain->SetBranchAddress("HFDigiFiber", &HFDigiFiber, &b_HFDigiFiber);
   fChain->SetBranchAddress("HFDigiFiberChan", &HFDigiFiberChan, &b_HFDigiFiberChan);
   fChain->SetBranchAddress("HFDigiLADC", &HFDigiLADC, &b_HFDigiLADC);
   fChain->SetBranchAddress("HFDigiRaw", &HFDigiRaw, &b_HFDigiRaw);
   fChain->SetBranchAddress("QIE10DigiADC", &QIE10DigiADC, &b_QIE10DigiADC);
   fChain->SetBranchAddress("QIE10DigiCapID", &QIE10DigiCapID, &b_QIE10DigiCapID);
   fChain->SetBranchAddress("QIE10DigiLETDC", &QIE10DigiLETDC, &b_QIE10DigiLETDC);
   fChain->SetBranchAddress("QIE10DigiOK", &QIE10DigiOK, &b_QIE10DigiOK);
   fChain->SetBranchAddress("QIE10DigiSOI", &QIE10DigiSOI, &b_QIE10DigiSOI);
   fChain->SetBranchAddress("QIE10DigiTETDC", &QIE10DigiTETDC, &b_QIE10DigiTETDC);
   fChain->SetBranchAddress("bx", &bx, &b_bx);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("ls", &ls, &b_ls);
   fChain->SetBranchAddress("run", &run, &b_run);
   Notify();
}

Bool_t branches::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void branches::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t branches::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef branches_cxx
