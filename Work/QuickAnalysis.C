

{
  string run;
  cout << " please enter run number\n We currently support 275311  277420 278018 278240 278406 \n"; 
  getline (cin,run);
  char filenames[500];
  char tempname[200];
  vector<string> filesVec;
  sprintf(tempname,"macros/HCAL/ListOfRootFiles_run_%s.txt",run.c_str());
  ifstream fin(tempname);
  TChain *tree = new TChain("hcalTupleTree/tree");
  //read the file names from the .txt files and load them to a vector.
  while(fin.getline(filenames, 500) ){filesVec.push_back(filenames);}
  for(unsigned int in=0; in<filesVec.size(); in++){ tree->Add(filesVec.at(in).c_str()); }

}
