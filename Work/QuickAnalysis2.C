
bool loadNext(TTree * ttree_ , int & currentEntry_);

void QuickAnalysis2(){
  char filenames[500];
  char tempname[200];
  TChain *tree = new TChain("hcalTupleTree/tree");
  sprintf(tempname,"/home/borzou/HCAL/CMSSW_8_0_17/src/HCALPFG/HcalTupleMaker/test/HcalTupleMaker.root"); 
  tree->Add(tempname);
  TTree * ttree_ = (TTree*) tree;
  
  

  //loop over events
  int currentEntry_=-1;
  while(loadNext(ttree_,currentEntry_)){

    


  } // end of loop over events

}




















// Functions

bool loadNext(TTree * ttree_ , int & currentEntry_) {

    int template_Entries = ttree_->GetEntries();
    if( currentEntry_ < 0 ) {
      std::cout << "Processing " << template_Entries << " events" << std::endl;
    }

    ++currentEntry_;

    if( currentEntry_ == template_Entries ) {

      return false;

    } else {

      if( currentEntry_ > 0 && currentEntry_%10000 == 0 ) {
        std::cout << " Processed " << currentEntry_ << " events" << std::endl;
      }

      ttree_->GetEntry(currentEntry_);
      return true;

    }


  }


