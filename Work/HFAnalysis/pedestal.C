
double pedestal_value(TH1F* htmp){

  //htmp->Print();
  double tot_val=0.;
  double tot_ent=0.;
  for (int ibin=0;ibin<htmp->GetNbinsX();ibin++){
    double bin_value = htmp->GetBinCenter(ibin+1);
    double bin_ent   = htmp->GetBinContent(ibin+1);
    if (bin_value<20.){
      tot_ent += bin_ent;
      tot_val += bin_ent*bin_value;
    }
  }
  double ped = 0.;
  if (tot_ent>0.) ped = tot_val/tot_ent;
  return ped;

}

void pedestal(){

  TFile *_file0 = TFile::Open("OutPut/results_run275311_allPhi_00_pedestal.root");
  TH1F *pedestal_charge_iphi39[13][2][2]; // ieta=29-41:13, depth:2, anode A&B:2

  char tmp[40];

  for (int ieta_bin=0; ieta_bin<=12; ieta_bin++){
    for (int idepth_bin=0; idepth_bin<=1; idepth_bin++){
      for (int ianode_bin=0; ianode_bin<=1; ianode_bin++){
	int ieta=ieta_bin+29;
	int idepth=idepth_bin+1;
	if (ieta==40) continue;
	if      (ianode_bin==0) sprintf(tmp,"Charge_AnodeA_ieta%d_iphi39_depth%d",ieta,idepth);
	else if (ianode_bin==1) sprintf(tmp,"Charge_AnodeB_ieta%d_iphi39_depth%d",ieta,idepth);
	std::cout << tmp << std::endl;
	pedestal_charge_iphi39[ieta_bin][idepth_bin][ianode_bin] = (TH1F*)_file0->Get(tmp);
	//pedestal_charge_iphi39[ieta_bin][idepth_bin][ianode_bin]->Print();	  
	double ped = pedestal_value(pedestal_charge_iphi39[ieta_bin][idepth_bin][ianode_bin]);
	std::cout << ped << std::endl;
      }
    }
  }

  //
  // Anode-A, depth=1
  std::cout << "double pedestal_QIE10_depth1_anodeA[13]={\n  ";
  for (int ieta_bin=0; ieta_bin<=12; ieta_bin++){
    int ieta=ieta_bin+29;
    if (ieta==41)
      std::cout << pedestal_value(pedestal_charge_iphi39[ieta_bin][0][0]) <<"};\n";
    else if (ieta==40)
      std::cout <<"0, ";
    else
      std::cout << pedestal_value(pedestal_charge_iphi39[ieta_bin][0][0]) <<", ";

  }
  //
  // Anode-A, depth=2
  std::cout << "double pedestal_QIE10_depth2_anodeA[13]={\n  ";
  for (int ieta_bin=0; ieta_bin<=12; ieta_bin++){
    int ieta=ieta_bin+29;
    if (ieta==41)
      std::cout << pedestal_value(pedestal_charge_iphi39[ieta_bin][1][0]) <<"};\n";
    else if (ieta==40)
      std::cout <<"0, ";
    else
      std::cout << pedestal_value(pedestal_charge_iphi39[ieta_bin][1][0]) <<", ";

  }

  //
  // Anode-B, depth=1
  std::cout << "double pedestal_QIE10_depth1_anodeB[13]={\n  ";
  for (int ieta_bin=0; ieta_bin<=12; ieta_bin++){
    int ieta=ieta_bin+29;
    if (ieta==41)
      std::cout << "0." <<"};\n";
    else if (ieta==40)
      std::cout <<"0., ";
    else
      std::cout << "0., ";

  }
  //
  // Anode-B, depth=2
  std::cout << "double pedestal_QIE10_depth2_anodeB[13]={\n  ";
  for (int ieta_bin=0; ieta_bin<=12; ieta_bin++){
    int ieta=ieta_bin+29;
    if (ieta==41)
      std::cout << "0." <<"};\n";
    else if (ieta==40)
      std::cout <<"0., ";
    else if (ieta==30 || ieta==34)
      std::cout << pedestal_value(pedestal_charge_iphi39[ieta_bin][1][1]) <<", ";
    else
      std::cout <<"0., ";

  }
  


}

