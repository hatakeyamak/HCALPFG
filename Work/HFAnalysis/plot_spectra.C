
//
// --- emean  ---
//
double emean(TH1F* htmp, double high, double low){

  //htmp->Print();
  double tot_val=0.;
  double tot_ent=0.;
  for (int ibin=0;ibin<htmp->GetNbinsX();ibin++){
    double bin_value = htmp->GetBinCenter(ibin+1);
    double bin_ent   = htmp->GetBinContent(ibin+1);
    if (bin_value>low && bin_value<high){
      tot_ent += bin_ent;
      tot_val += bin_ent*bin_value;
    }
  }
  double emean = 0.;
  if (tot_ent>0.) emean = tot_val/tot_ent;
  return emean;

}

//
// plot spectra
//
void plot_spectra(){

  //
  // Customization
  //

  gStyle->SetTitleAlign(33);
  gStyle->SetTitleX(.65);
  gStyle->SetTitleY(.92);
  gStyle->SetTitleFontSize(0.08);

  //
  // Open input root files, read in necessary histograms
  //

  TFile *_file0 = TFile::Open("OutPut/results_KH4.root");
  TH1F *energy_anode_iphi39[13][2][2]; // ieta=29-41:13, depth:2, anode A&B:2
  TH1F *energy_QIE8x2_iphi39[13][2];     // ieta=29-41:13, depth:2, QIE8 x2 (as done for default reco)
  TH1F *energy_QIE8calib_iphi39[13][2];  // ieta=29-41:13, depth:2, QIE8 phi symmetry corrections
  TH1F *energy_iphi39[13][2];          // ieta=29-41:13, depth:2
  TH1F *energy_iphi35[13][2];          // ieta=29-41:13, depth:2
  TH1F *energy_iphi43[13][2];          // ieta=29-41:13, depth:2
  TH1F *energy_iphinot39[13][2];       // ieta=29-41:13, depth:2

  double emean_hi=400.;
  double emean_lo=40.;
  TH1F *relative_response_vsieta_iphi39_depth1 = new TH1F("relative_response_vsieta_iphi39_depth1","relative_response_vsieta_iphi39_depth1",13,28.5,41.5);
  TH1F *relative_response_vsieta_iphi39_depth2 = new TH1F("relative_response_vsieta_iphi39_depth2","relative_response_vsieta_iphi39_depth2",13,28.5,41.5);
  TH1F *relative_response_iphi39_depth1 = new TH1F("relative_response_iphi39_depth1","relative_response_iphi39_depth1",100,0.5,1.5);
  TH1F *relative_response_iphi39_depth2 = new TH1F("relative_response_iphi39_depth2","relative_response_iphi39_depth2",100,0.5,1.5);

  char tmp[40];

  for (int ieta_bin=0; ieta_bin<=12; ieta_bin++){
    int ieta=ieta_bin+29;
    if (ieta==40) continue;

    for (int idepth_bin=0; idepth_bin<=1; idepth_bin++){
      int idepth=idepth_bin+1;

      sprintf(tmp,"ieta%d_iphi39_depth%d",ieta,idepth);
      //std::cout << tmp << std::endl;
      energy_iphi39[ieta_bin][idepth_bin] = (TH1F*)_file0->Get(tmp);       // iphi=39 

      sprintf(tmp,"ieta%d_iphinot39_depth%d",ieta,idepth);
      energy_iphinot39[ieta_bin][idepth_bin] = (TH1F*)_file0->Get(tmp);    // iphi!=39
      if (ieta>=40) energy_iphinot39[ieta_bin][idepth_bin]->Scale(1./17.); // nphi=18-1
      else          energy_iphinot39[ieta_bin][idepth_bin]->Scale(1./35.); // nphi=36-1

      sprintf(tmp,"ieta%d_iphi35_depth%d",ieta,idepth);
      energy_iphi35[ieta_bin][idepth_bin] = (TH1F*)_file0->Get(tmp);       // iphi=35

      sprintf(tmp,"ieta%d_iphi43_depth%d",ieta,idepth);
      energy_iphi43[ieta_bin][idepth_bin] = (TH1F*)_file0->Get(tmp);       // iphi=43

      sprintf(tmp,"AnodeB_QIE8x2_ieta%d_iphi39_depth%d",ieta,idepth);
      energy_QIE8x2_iphi39[ieta_bin][idepth_bin] = (TH1F*)_file0->Get(tmp);          // iphi=39 

      sprintf(tmp,"AnodeB_QIE8calib_ieta%d_iphi39_depth%d",ieta,idepth);
      energy_QIE8calib_iphi39[ieta_bin][idepth_bin] = (TH1F*)_file0->Get(tmp);       // iphi=39 

      for (int ianode_bin=0; ianode_bin<=1; ianode_bin++){
	if      (ianode_bin==0) sprintf(tmp,"AnodeA_ieta%d_iphi39_depth%d",ieta,idepth);
	else if (ianode_bin==1) sprintf(tmp,"AnodeB_ieta%d_iphi39_depth%d",ieta,idepth);
	std::cout << tmp << std::endl;
	energy_anode_iphi39[ieta_bin][idepth_bin][ianode_bin] = (TH1F*)_file0->Get(tmp);
      } // ianode_bin

    }   // idepth_bin
  }     // ieta_bin


  //
  // Plotting - Depth1
  //
  TCanvas *c1 = new TCanvas("c_HFP_depth1","c1_HFP_depth1",1600,1200);
  TLegend* catLeg1 = new TLegend(0.55,0.5,0.9,0.85);
  catLeg1->SetTextSize(0.064);
  catLeg1->SetTextFont(42);
  catLeg1->SetLineColor(0);

  c1->Divide(3,4);
  for (int ieta_bin=0; ieta_bin<=12; ieta_bin++){
    int ieta=ieta_bin+29;
    if (ieta==40) continue;
    if (ieta>40) c1->cd(ieta_bin); // skip ieta=40
    else         c1->cd(ieta_bin+1);
    gPad->SetLogy();
    gPad->SetTopMargin(0.02);
    gPad->SetBottomMargin(0.16);
    gPad->SetRightMargin(0.04);
    gPad->SetLeftMargin(0.16);

    energy_iphi39[ieta_bin][0]->SetMinimum(0.8);
    energy_iphi39[ieta_bin][0]->GetXaxis()->SetTitle("Energy (GeV)");
    energy_iphi39[ieta_bin][0]->GetYaxis()->SetTitle("Number of entries");
    energy_iphi39[ieta_bin][0]->SetTitleSize(0.08);
    energy_iphi39[ieta_bin][0]->SetTitleSize(0.08,"X");
    energy_iphi39[ieta_bin][0]->SetTitleSize(0.08,"Y");
    energy_iphi39[ieta_bin][0]->SetTitleOffset(1.0,"X");
    energy_iphi39[ieta_bin][0]->SetTitleOffset(1.0,"Y");
    energy_iphi39[ieta_bin][0]->SetLabelSize(0.06,"X");
    energy_iphi39[ieta_bin][0]->SetLabelSize(0.06,"Y");
    energy_iphi39[ieta_bin][0]->SetStats(false);
    energy_iphi39[ieta_bin][0]->SetLineColor(1);
    energy_iphi35[ieta_bin][0]->SetLineColor(2);
    energy_iphi43[ieta_bin][0]->SetLineColor(4);
    energy_iphinot39[ieta_bin][0]->SetLineColor(6);

    sprintf(tmp,"ieta = %d, depth=1",ieta);
    energy_iphi39[ieta_bin][0]->SetTitle(tmp);
    energy_iphi39[ieta_bin][0]->Draw();
    energy_iphi35[ieta_bin][0]->Draw("same");
    energy_iphi43[ieta_bin][0]->Draw("same");
    //energy_iphinot39[ieta_bin][0]->Draw("same");

    if (ieta_bin==0){
      catLeg1->AddEntry(energy_iphi35[ieta_bin][0],"iphi=35: Q #times gain","l");
      catLeg1->AddEntry(energy_iphi39[ieta_bin][0],"iphi=39: Q_{A}+Q_{B} #times gain","l");
      catLeg1->AddEntry(energy_iphi43[ieta_bin][0],"iphi=43: Q #times gain","l");
      //catLeg1->AddEntry(energy_iphinot39[ieta_bin][0],"iphi#neq39: Q #times gain","l");
    }
    catLeg1->Draw();

    std::cout << ieta << " "
              << emean(energy_iphi35[ieta_bin][0],emean_hi,emean_lo) << " "
              << emean(energy_iphi39[ieta_bin][0],emean_hi,emean_lo) << " "
              << emean(energy_iphi43[ieta_bin][0],emean_hi,emean_lo) << std::endl;
    relative_response_vsieta_iphi39_depth1->SetBinContent(ieta_bin+1,
                                                   2.*emean(energy_iphi39[ieta_bin][0],emean_hi,emean_lo)
                                                   /(emean(energy_iphi35[ieta_bin][0],emean_hi,emean_lo)+emean(energy_iphi43[ieta_bin][0],emean_hi,emean_lo)));    
    relative_response_vsieta_iphi39_depth1->SetBinError(ieta_bin+1,0.);
    relative_response_iphi39_depth1->Fill(
                                                   2.*emean(energy_iphi39[ieta_bin][0],emean_hi,emean_lo)
                                                   /(emean(energy_iphi35[ieta_bin][0],emean_hi,emean_lo)+emean(energy_iphi43[ieta_bin][0],emean_hi,emean_lo)));
   
  }
  c1->SaveAs("c_HFP_depth1.gif");
  c1->SaveAs("c_HFP_depth1.pdf");

  //
  // Plotting
  //
  TCanvas *c1b = new TCanvas("c_HFP_depth1_b","c1b_HFP_depth1",1600,1200);
  TLegend* catLeg1b = new TLegend(0.55,0.5,0.9,0.85);
  catLeg1b->SetTextSize(0.064);
  catLeg1b->SetTextFont(42);
  catLeg1b->SetLineColor(0);
  TLegend* catLeg1b2 = new TLegend(0.55,0.5,0.9,0.85);
  catLeg1b2->SetTextSize(0.064);
  catLeg1b2->SetTextFont(42);
  catLeg1b2->SetLineColor(0);

  c1b->Divide(3,4);
  for (int ieta_bin=0; ieta_bin<=12; ieta_bin++){
    int ieta=ieta_bin+29;
    if (ieta==40) continue;
    if (ieta>40) c1b->cd(ieta_bin); // skip ieta=40
    else         c1b->cd(ieta_bin+1);
    gPad->SetLogy();
    gPad->SetTopMargin(0.02);
    gPad->SetBottomMargin(0.16);
    gPad->SetRightMargin(0.04);
    gPad->SetLeftMargin(0.16);

    energy_iphinot39[ieta_bin][0]->SetLineColor(6);

    sprintf(tmp,"ieta = %d, depth=1",ieta);
    energy_iphi39[ieta_bin][0]->SetTitle(tmp);
    energy_iphi39[ieta_bin][0]->Draw();
    energy_iphinot39[ieta_bin][0]->Draw("same");

    if (ieta_bin==0){
      catLeg1b->AddEntry(energy_iphi39[ieta_bin][0],"iphi=39: Q_{A}+Q_{B} #times gain","l");
      catLeg1b->AddEntry(energy_iphinot39[ieta_bin][0],"iphi#neq39: Q #times gain (/35)","l");
    }
    if (ieta==41){
      catLeg1b2->AddEntry(energy_iphi39[ieta_bin][0],"iphi=39: Q_{A}+Q_{B} #times gain","l");
      catLeg1b2->AddEntry(energy_iphinot39[ieta_bin][0],"iphi#neq39: Q #times gain (/17)","l");
    }
    if (ieta==41) catLeg1b2->Draw();
    else          catLeg1b->Draw();
   
  }
  c1b->SaveAs("c_HFP_depth1_b.gif");
  c1b->SaveAs("c_HFP_depth1_b.pdf");

  //
  // Plotting
  //
  TCanvas *c1c = new TCanvas("c_HFP_depth1_c","c1c_HFP_depth1",1600,1200);
  TLegend* catLeg1c = new TLegend(0.55,0.5,0.9,0.85);
  catLeg1c->SetTextSize(0.064);
  catLeg1c->SetTextFont(42);
  catLeg1c->SetLineColor(0);
  TLegend* catLeg1c2 = new TLegend(0.55,0.5,0.9,0.85);
  catLeg1c2->SetTextSize(0.064);
  catLeg1c2->SetTextFont(42);
  catLeg1c2->SetLineColor(0);

  c1c->Divide(3,4);
  for (int ieta_bin=0; ieta_bin<=12; ieta_bin++){
    int ieta=ieta_bin+29;
    if (ieta==40) continue;
    if (ieta>40) c1c->cd(ieta_bin); // skip ieta=40
    else         c1c->cd(ieta_bin+1);
    gPad->SetLogy();
    gPad->SetTopMargin(0.02);
    gPad->SetBottomMargin(0.16);
    gPad->SetRightMargin(0.04);
    gPad->SetLeftMargin(0.16);
    
    energy_iphinot39[ieta_bin][0]->SetMinimum(0.8);
    energy_iphinot39[ieta_bin][0]->GetXaxis()->SetTitle("Energy (GeV)");
    energy_iphinot39[ieta_bin][0]->GetYaxis()->SetTitle("Number of entries");
    energy_iphinot39[ieta_bin][0]->SetTitleSize(0.08);
    energy_iphinot39[ieta_bin][0]->SetTitleSize(0.08,"X");
    energy_iphinot39[ieta_bin][0]->SetTitleSize(0.08,"Y");
    energy_iphinot39[ieta_bin][0]->SetTitleOffset(1.0,"X");
    energy_iphinot39[ieta_bin][0]->SetTitleOffset(1.0,"Y");
    energy_iphinot39[ieta_bin][0]->SetLabelSize(0.06,"X");
    energy_iphinot39[ieta_bin][0]->SetLabelSize(0.06,"Y");
    energy_iphinot39[ieta_bin][0]->SetStats(false);
    energy_iphinot39[ieta_bin][0]->SetLineColor(1);
    
    energy_iphinot39[ieta_bin][0]->SetLineColor(6);
    energy_QIE8x2_iphi39[ieta_bin][0]->SetLineColor(2);
    energy_QIE8calib_iphi39[ieta_bin][0]->SetLineColor(4);

    sprintf(tmp,"ieta = %d, depth=1",ieta);
    energy_iphinot39[ieta_bin][0]->SetTitle(tmp);
    energy_iphinot39[ieta_bin][0]->Draw();
    energy_QIE8x2_iphi39[ieta_bin][0]->Draw("same");
    if (ieta!=29 && ieta!=41) energy_QIE8calib_iphi39[ieta_bin][0]->Draw("same");

    if (ieta_bin==0){
      catLeg1c->AddEntry(energy_iphinot39[ieta_bin][0],"iphi#neq39: Q #times gain","l");
      catLeg1c->AddEntry(energy_QIE8x2_iphi39[ieta_bin][0],"iphi=39: Q_{B} #times gain #times 2");
      //catLeg1c->AddEntry(energy_QIE8calib_iphi39[ieta_bin][0],"iphi=39: Q_{B} #times gain (calibrated)");
    }
    if (ieta_bin==1){
      catLeg1c2->AddEntry(energy_iphinot39[ieta_bin][0],"iphi#neq39: Q #times gain","l");
      catLeg1c2->AddEntry(energy_QIE8x2_iphi39[ieta_bin][0],"iphi=39: Q_{B} #times gain #times 2");
      catLeg1c2->AddEntry(energy_QIE8calib_iphi39[ieta_bin][0],"iphi=39: Q_{B} #times gain (calib)");
    }
    if (ieta!=29 && ieta!=41) catLeg1c2->Draw();
    else                      catLeg1c->Draw();

  }
  c1c->SaveAs("c_HFP_depth1_c.gif");
  c1c->SaveAs("c_HFP_depth1_c.pdf");

  //
  // Plotting - Depth2
  //
  TCanvas *c2 = new TCanvas("c_HFP_depth2","c2_HFP_depth2",1600,1200);
  TLegend* catLeg2 = new TLegend(0.55,0.5,0.9,0.85);
  catLeg2->SetTextSize(0.064);
  catLeg2->SetTextFont(42);
  catLeg2->SetLineColor(0);

  c2->Divide(3,4);
  for (int ieta_bin=0; ieta_bin<=12; ieta_bin++){
    int ieta=ieta_bin+29;
    if (ieta==40) continue;
    if (ieta>40) c2->cd(ieta_bin); // skip ieta=40
    else         c2->cd(ieta_bin+1);
    gPad->SetLogy();
    gPad->SetTopMargin(0.02);
    gPad->SetBottomMargin(0.16);
    gPad->SetRightMargin(0.04);
    gPad->SetLeftMargin(0.16);

    energy_iphi39[ieta_bin][1]->SetMinimum(0.8);
    energy_iphi39[ieta_bin][1]->GetXaxis()->SetTitle("Energy (GeV)");
    energy_iphi39[ieta_bin][1]->GetYaxis()->SetTitle("Number of entries");
    energy_iphi39[ieta_bin][1]->SetTitleSize(0.08);
    energy_iphi39[ieta_bin][1]->SetTitleSize(0.08,"X");
    energy_iphi39[ieta_bin][1]->SetTitleSize(0.08,"Y");
    energy_iphi39[ieta_bin][1]->SetTitleOffset(1.0,"X");
    energy_iphi39[ieta_bin][1]->SetTitleOffset(1.0,"Y");
    energy_iphi39[ieta_bin][1]->SetLabelSize(0.06,"X");
    energy_iphi39[ieta_bin][1]->SetLabelSize(0.06,"Y");
    energy_iphi39[ieta_bin][1]->SetStats(false);
    energy_iphi39[ieta_bin][1]->SetLineColor(1);
    energy_iphi35[ieta_bin][1]->SetLineColor(2);
    energy_iphi43[ieta_bin][1]->SetLineColor(4);
    energy_iphinot39[ieta_bin][1]->SetLineColor(6);

    sprintf(tmp,"ieta = %d, depth=2",ieta);
    energy_iphi39[ieta_bin][1]->SetTitle(tmp);
    energy_iphi39[ieta_bin][1]->Draw();
    energy_iphi35[ieta_bin][1]->Draw("same");
    energy_iphi43[ieta_bin][1]->Draw("same");
    //energy_iphinot39[ieta_bin][1]->Draw("same");

    if (ieta_bin==0){
      catLeg2->AddEntry(energy_iphi35[ieta_bin][1],"iphi=35: Q #times gain","l");
      catLeg2->AddEntry(energy_iphi39[ieta_bin][1],"iphi=39: Q_{A}+Q_{B} #times gain","l");
      catLeg2->AddEntry(energy_iphi43[ieta_bin][1],"iphi=43: Q #times gain","l");
      //catLeg2->AddEntry(energy_iphi43[ieta_bin][1],"iphi#neq39: Q #times gain","l");
    }
    catLeg2->Draw();

    std::cout << ieta << " "
              << emean(energy_iphi35[ieta_bin][1],500.,50.) << " "
              << emean(energy_iphi39[ieta_bin][1],500.,50.) << " "
              << emean(energy_iphi43[ieta_bin][1],500.,50.) << std::endl;
    relative_response_vsieta_iphi39_depth2->SetBinContent(ieta_bin+1,
                                                   2.*emean(energy_iphi39[ieta_bin][1],emean_hi,emean_lo)
                                                   /(emean(energy_iphi35[ieta_bin][1],emean_hi,emean_lo)+emean(energy_iphi43[ieta_bin][1],emean_hi,emean_lo)));
    relative_response_vsieta_iphi39_depth2->SetBinError(ieta_bin+1,0.);
    relative_response_iphi39_depth2->Fill(
                                                   2.*emean(energy_iphi39[ieta_bin][1],emean_hi,emean_lo)
                                                   /(emean(energy_iphi35[ieta_bin][1],emean_hi,emean_lo)+emean(energy_iphi43[ieta_bin][1],emean_hi,emean_lo)));
   
  }
  c2->SaveAs("c_HFP_depth2.gif");
  c2->SaveAs("c_HFP_depth2.pdf");

  //
  //
  //
  TCanvas *c2b = new TCanvas("c_HFP_depth2_b","c2b_HFP_depth2",1600,1200);
  TLegend* catLeg2b = new TLegend(0.55,0.5,0.9,0.85);
  catLeg2b->SetTextSize(0.064);
  catLeg2b->SetTextFont(42);
  catLeg2b->SetLineColor(0);
  TLegend* catLeg2b2 = new TLegend(0.55,0.5,0.9,0.85);
  catLeg2b2->SetTextSize(0.064);
  catLeg2b2->SetTextFont(42);
  catLeg2b2->SetLineColor(0);

  c2b->Divide(3,4);
  for (int ieta_bin=0; ieta_bin<=12; ieta_bin++){
    int ieta=ieta_bin+29;
    if (ieta==40) continue;
    if (ieta>40) c2b->cd(ieta_bin); // skip ieta=40
    else         c2b->cd(ieta_bin+1);
    gPad->SetLogy();
    gPad->SetTopMargin(0.02);
    gPad->SetBottomMargin(0.16);
    gPad->SetRightMargin(0.04);
    gPad->SetLeftMargin(0.16);

    sprintf(tmp,"ieta = %d, depth=2",ieta);
    energy_iphi39[ieta_bin][1]->SetTitle(tmp);
    energy_iphi39[ieta_bin][1]->Draw();
    energy_iphinot39[ieta_bin][1]->Draw("same");

    if (ieta_bin==0){
      catLeg2b->AddEntry(energy_iphi39[ieta_bin][1],"iphi=39: Q_{A}+Q_{B} #times gain","l");
      catLeg2b->AddEntry(energy_iphinot39[ieta_bin][1],"iphi#neq39: Q #times gain (/35)","l");
    }
    if (ieta==41){
      catLeg2b2->AddEntry(energy_iphi39[ieta_bin][1],"iphi=39: Q_{A}+Q_{B} #times gain","l");
      catLeg2b2->AddEntry(energy_iphinot39[ieta_bin][1],"iphi#neq39: Q #times gain (/17)","l");
    }
    if (ieta==41) catLeg2b2->Draw();
    else          catLeg2b->Draw();
   
  }
  c2b->SaveAs("c_HFP_depth2_b.gif");
  c2b->SaveAs("c_HFP_depth2_b.pdf");

  //
  // Plotting
  //
  TCanvas *c2c = new TCanvas("c_HFP_depth2_c","c2c_HFP_depth2",1600,1200);
  TLegend* catLeg2c = new TLegend(0.55,0.5,0.9,0.85);
  catLeg2c->SetTextSize(0.064);
  catLeg2c->SetTextFont(42);
  catLeg2c->SetLineColor(0);
  TLegend* catLeg2c2 = new TLegend(0.55,0.5,0.9,0.85);
  catLeg2c2->SetTextSize(0.064);
  catLeg2c2->SetTextFont(42);
  catLeg2c2->SetLineColor(0);
  TLegend* catLeg2c3 = new TLegend(0.55,0.5,0.9,0.85);
  catLeg2c3->SetTextSize(0.064);
  catLeg2c3->SetTextFont(42);
  catLeg2c3->SetLineColor(0);

  c2c->Divide(3,4);
  for (int ieta_bin=0; ieta_bin<=12; ieta_bin++){
    int ieta=ieta_bin+29;
    if (ieta==40) continue;
    if (ieta>40) c2c->cd(ieta_bin); // skip ieta=40
    else         c2c->cd(ieta_bin+1);
    gPad->SetLogy();
    gPad->SetTopMargin(0.02);
    gPad->SetBottomMargin(0.16);
    gPad->SetRightMargin(0.04);
    gPad->SetLeftMargin(0.16);
    
    energy_iphinot39[ieta_bin][1]->SetMinimum(0.8);
    energy_iphinot39[ieta_bin][1]->GetXaxis()->SetTitle("Energy (GeV)");
    energy_iphinot39[ieta_bin][1]->GetYaxis()->SetTitle("Number of entries");
    energy_iphinot39[ieta_bin][1]->SetTitleSize(0.08);
    energy_iphinot39[ieta_bin][1]->SetTitleSize(0.08,"X");
    energy_iphinot39[ieta_bin][1]->SetTitleSize(0.08,"Y");
    energy_iphinot39[ieta_bin][1]->SetTitleOffset(1.0,"X");
    energy_iphinot39[ieta_bin][1]->SetTitleOffset(1.0,"Y");
    energy_iphinot39[ieta_bin][1]->SetLabelSize(0.06,"X");
    energy_iphinot39[ieta_bin][1]->SetLabelSize(0.06,"Y");
    energy_iphinot39[ieta_bin][1]->SetStats(false);
    energy_iphinot39[ieta_bin][1]->SetLineColor(1);
    
    energy_iphinot39[ieta_bin][1]->SetLineColor(6);
    energy_QIE8x2_iphi39[ieta_bin][1]->SetLineColor(2);
    energy_QIE8calib_iphi39[ieta_bin][1]->SetLineColor(4);

    sprintf(tmp,"ieta = %d, depth=1",ieta);
    energy_iphinot39[ieta_bin][1]->SetTitle(tmp);
    energy_iphinot39[ieta_bin][1]->Draw();
    energy_QIE8x2_iphi39[ieta_bin][1]->Draw("same");
    if (ieta!=29 && ieta!=41) energy_QIE8calib_iphi39[ieta_bin][1]->Draw("same");

    if (ieta_bin==0){
      catLeg2c->AddEntry(energy_iphinot39[ieta_bin][1],"iphi#neq39: Q #times gain","l");
      catLeg2c->AddEntry(energy_QIE8x2_iphi39[ieta_bin][1],"iphi=39: Q_{B} #times gain #times 2");
      //catLeg2c->AddEntry(energy_QIE8calib_iphi39[ieta_bin][1],"iphi=39: Q_{B} #times gain (calibrated)");
    }
    if (ieta_bin==1){
      catLeg2c3->AddEntry(energy_iphinot39[ieta_bin][1],"iphi#neq39: Q #times gain","l");
      //catLeg2c3->AddEntry(energy_QIE8x2_iphi39[ieta_bin][1],"iphi=39: Q_{B} #times gain #times 2");
      //catLeg2c3->AddEntry(energy_QIE8calib_iphi39[ieta_bin][1],"iphi=39: Q_{B} #times gain (calib)");
    }
    if (ieta_bin==2){
      catLeg2c2->AddEntry(energy_iphinot39[ieta_bin][1],"iphi#neq39: Q #times gain","l");
      catLeg2c2->AddEntry(energy_QIE8x2_iphi39[ieta_bin][1],"iphi=39: Q_{B} #times gain #times 2");
      catLeg2c2->AddEntry(energy_QIE8calib_iphi39[ieta_bin][1],"iphi=39: Q_{B} #times gain (calib)");
    }
    if      (ieta==29 || ieta==41) catLeg2c->Draw();
    else if (ieta==30 || ieta==34) catLeg2c3->Draw();
    else                           catLeg2c2->Draw();

  }
  c2c->SaveAs("c_HFP_depth2_c.gif");
  c2c->SaveAs("c_HFP_depth2_c.pdf");

  //
  // Plotting
  //
  TCanvas *c3 = new TCanvas("c_HFP_iphi39_depth1_anodeAB","c3_HFP_iphi39_depth1_anodeAB",1600,1200);
  TLegend* catLeg3 = new TLegend(0.55,0.5,0.9,0.85);
  catLeg3->SetTextSize(0.064);
  catLeg3->SetTextFont(42);
  catLeg3->SetLineColor(0);

  c3->Divide(3,4);
  for (int ieta_bin=0; ieta_bin<=12; ieta_bin++){
    int ieta=ieta_bin+29;
    if (ieta==40) continue;
    if (ieta>40) c3->cd(ieta_bin); // skip ieta=40
    else         c3->cd(ieta_bin+1);
    gPad->SetLogy();
    gPad->SetTopMargin(0.02);
    gPad->SetBottomMargin(0.16);
    gPad->SetRightMargin(0.04);
    gPad->SetLeftMargin(0.16);

    energy_anode_iphi39[ieta_bin][0][0]->SetMinimum(0.8);
    energy_anode_iphi39[ieta_bin][0][0]->GetXaxis()->SetTitle("Energy (GeV)");
    energy_anode_iphi39[ieta_bin][0][0]->GetYaxis()->SetTitle("Number of entries");
    energy_anode_iphi39[ieta_bin][0][0]->SetTitleSize(0.08);
    energy_anode_iphi39[ieta_bin][0][0]->SetTitleSize(0.08,"X");
    energy_anode_iphi39[ieta_bin][0][0]->SetTitleSize(0.08,"Y");
    energy_anode_iphi39[ieta_bin][0][0]->SetTitleOffset(1.0,"X");
    energy_anode_iphi39[ieta_bin][0][0]->SetTitleOffset(1.0,"Y");
    energy_anode_iphi39[ieta_bin][0][0]->SetLabelSize(0.06,"X");
    energy_anode_iphi39[ieta_bin][0][0]->SetLabelSize(0.06,"Y");
    energy_anode_iphi39[ieta_bin][0][0]->SetStats(false);
    energy_anode_iphi39[ieta_bin][0][0]->SetLineColor(1);
    energy_anode_iphi39[ieta_bin][0][1]->SetLineColor(2);

    sprintf(tmp,"ieta = %d, depth=1",ieta);
    energy_anode_iphi39[ieta_bin][0][0]->SetTitle(tmp);
    energy_anode_iphi39[ieta_bin][0][0]->Draw();
    energy_anode_iphi39[ieta_bin][0][1]->Draw("same");

    if (ieta_bin==0){
      catLeg3->AddEntry(energy_anode_iphi39[ieta_bin][0][0],"iphi=39: Q_{A} #times gain","l");
      catLeg3->AddEntry(energy_anode_iphi39[ieta_bin][0][1],"iphi=39: Q_{B} #times gain","l");
    }
    catLeg3->Draw();
   
  }
  c3->Update();
  c3->Print("c_HFP_iphi39_depth1_anodeAB.gif");
  c3->SaveAs("c_HFP_iphi39_depth1_anodeAB.pdf");

  //
  // Plotting
  //
  TCanvas *c4 = new TCanvas("c_HFP_iphi39_depth2_anodeAB","c4_HFP_iphi39_depth2_anodeAB",1600,1200);
  TLegend* catLeg4 = new TLegend(0.55,0.5,0.9,0.85);
  catLeg4->SetTextSize(0.064);
  catLeg4->SetTextFont(42);
  catLeg4->SetLineColor(0);

  c4->Divide(3,4);
  for (int ieta_bin=0; ieta_bin<=12; ieta_bin++){
    int ieta=ieta_bin+29;
    if (ieta==40) continue;
    if (ieta>40) c4->cd(ieta_bin); // skip ieta=40
    else         c4->cd(ieta_bin+1);
    gPad->SetLogy();
    gPad->SetTopMargin(0.02);
    gPad->SetBottomMargin(0.16);
    gPad->SetRightMargin(0.04);
    gPad->SetLeftMargin(0.16);

    energy_anode_iphi39[ieta_bin][1][0]->SetMinimum(0.8);
    energy_anode_iphi39[ieta_bin][1][0]->GetXaxis()->SetTitle("Energy (GeV)");
    energy_anode_iphi39[ieta_bin][1][0]->GetYaxis()->SetTitle("Number of entries");
    energy_anode_iphi39[ieta_bin][1][0]->SetTitleSize(0.08);
    energy_anode_iphi39[ieta_bin][1][0]->SetTitleSize(0.08,"X");
    energy_anode_iphi39[ieta_bin][1][0]->SetTitleSize(0.08,"Y");
    energy_anode_iphi39[ieta_bin][1][0]->SetTitleOffset(1.0,"X");
    energy_anode_iphi39[ieta_bin][1][0]->SetTitleOffset(1.0,"Y");
    energy_anode_iphi39[ieta_bin][1][0]->SetLabelSize(0.06,"X");
    energy_anode_iphi39[ieta_bin][1][0]->SetLabelSize(0.06,"Y");
    energy_anode_iphi39[ieta_bin][1][0]->SetStats(false);
    energy_anode_iphi39[ieta_bin][1][0]->SetLineColor(1);
    energy_anode_iphi39[ieta_bin][1][1]->SetLineColor(2);

    sprintf(tmp,"ieta = %d, depth=2",ieta);
    energy_anode_iphi39[ieta_bin][1][0]->SetTitle(tmp);
    energy_anode_iphi39[ieta_bin][1][0]->Draw();
    energy_anode_iphi39[ieta_bin][1][1]->Draw("same");

    if (ieta_bin==0){
      catLeg4->AddEntry(energy_anode_iphi39[ieta_bin][1][0],"iphi=39: Q_{A} #times gain","l");
      catLeg4->AddEntry(energy_anode_iphi39[ieta_bin][1][1],"iphi=39: Q_{B} #times gain","l");
    }
    catLeg4->Draw();
   
  }
  c4->Update();
  c4->SaveAs("c_HFP_iphi39_depth2_anodeAB.gif");
  c4->SaveAs("c_HFP_iphi39_depth2_anodeAB.pdf");

  //
  // Plotting
  //
  TCanvas *c5 = new TCanvas("c_relative_response_vsieta_iphi39","",800,500);
  TLegend* catLeg5 = new TLegend(0.55,0.7,0.9,0.85);
  catLeg5->SetTextSize(0.044);
  catLeg5->SetTextFont(42);
  catLeg5->SetLineColor(0);

  //c5->Divide(1,2);
    
  //c5->cd(1);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.16);
  gPad->SetRightMargin(0.04);
  gPad->SetLeftMargin(0.16);
  
  relative_response_vsieta_iphi39_depth1->SetMinimum(0.4);
  relative_response_vsieta_iphi39_depth1->SetMaximum(1.5);

  relative_response_vsieta_iphi39_depth1->GetXaxis()->SetTitle("ieta");
  relative_response_vsieta_iphi39_depth1->GetYaxis()->SetTitle("<E>(iphi=39,Q_{A}+Q_{B}) / <E>(iphi=35,43)");
  relative_response_vsieta_iphi39_depth1->SetTitle("");
  relative_response_vsieta_iphi39_depth1->SetName("");
  relative_response_vsieta_iphi39_depth1->SetTitleSize(0.001);
  relative_response_vsieta_iphi39_depth1->SetTitleSize(0.05,"X");
  relative_response_vsieta_iphi39_depth1->SetTitleSize(0.05,"Y");
  relative_response_vsieta_iphi39_depth1->SetTitleOffset(1.0,"X");
  relative_response_vsieta_iphi39_depth1->SetTitleOffset(1.0,"Y");
  relative_response_vsieta_iphi39_depth1->SetLabelSize(0.04,"X");
  relative_response_vsieta_iphi39_depth1->SetLabelSize(0.04,"Y");
  relative_response_vsieta_iphi39_depth1->SetStats(false);
  relative_response_vsieta_iphi39_depth1->SetLineColor(2);
  relative_response_vsieta_iphi39_depth2->SetLineColor(4);

  relative_response_vsieta_iphi39_depth1->SetMarkerStyle(20);
  relative_response_vsieta_iphi39_depth2->SetMarkerStyle(23);
  relative_response_vsieta_iphi39_depth1->Draw("E0");
  relative_response_vsieta_iphi39_depth2->Draw("samee0");

  catLeg5->AddEntry(relative_response_vsieta_iphi39_depth1,"HF+ long fiber (depth 1)","l");
  catLeg5->AddEntry(relative_response_vsieta_iphi39_depth2,"HF+ short fiber (depth 2)","l");
  catLeg5->Draw();

  relative_response_iphi39_depth1->Print();
  relative_response_iphi39_depth2->Print();
  
  c5->SaveAs("c_relative_response_vsieta_iphi39.gif");
  c5->SaveAs("c_relative_response_vsieta_iphi39.pdf");


}

