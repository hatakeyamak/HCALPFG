
void Plot_Calib(int iEta=30){

  gStyle->SetOptStat(0);
  gStyle->SetPalette(1) ; // for better color output

  // Open the file
  TFile * file = new TFile("results_run278963to279667_allPhi_.root","READ");

  TH1D * tempHist;
  char tempname[200];

  // Legend
  Float_t legendX1 = .60; //.50;
  Float_t legendX2 = .90; //.70;
  Float_t legendY1 = .65; //.65;
  Float_t legendY2 = .85;
  TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
  catLeg1->SetTextSize(0.042);
  catLeg1->SetTextFont(42);
  catLeg1->SetFillColor(0);
  catLeg1->SetLineColor(0);
  catLeg1->SetBorderSize(0);
  sprintf(tempname,"ieta: %d depth: 2",iEta);
  catLeg1->SetHeader(tempname);

  // Canvas
  int W = 600;
  int H = 600;
  int H_ref = 600;
  int W_ref = 800;
  float T = 0.08*H_ref;
  float B = 0.12*H_ref;
  float L = 0.12*W_ref;
  float R = 0.08*W_ref;
  TCanvas *canvas = new TCanvas("canvas","canvas",10,10,W,H);
  canvas->SetFillColor(0);
  canvas->SetBorderMode(0);
  canvas->SetFrameFillStyle(0);
  canvas->SetFrameBorderMode(0);
/*
  canvas->SetLeftMargin( L/W );
  canvas->SetRightMargin( R/W );
  canvas->SetRightMargin( 0.2 );
  canvas->SetTopMargin( T/H*3.0 );
  canvas->SetBottomMargin( B/H );
*/
  canvas->SetTickx(0);
  canvas->SetTicky(0);

  gPad->SetLogy(); 
 
  // 
  sprintf(tempname,"ieta30_iphi37_depth2");
  tempHist = (TH1D*) file->Get(tempname)->Clone();
  catLeg1->AddEntry(tempHist,"iphi: 37","l");
  //sprintf(tempname,"Energy distribution at iEta %d depth 1",iEta);
  //tempHist->SetTitle(tempname);
  tempHist->SetTitleOffset(0.5);
  tempHist->GetXaxis()->SetTitle("Energy");
  tempHist->GetXaxis()->SetTitleOffset(1.4);
  tempHist->GetYaxis()->SetTitleOffset(1.2);
  tempHist->GetYaxis()->SetTitle("N");
  tempHist->GetXaxis()->SetRangeUser(-5., 150.);
  tempHist->Rebin(4);
  tempHist->Draw();

  sprintf(tempname,"ieta30_iphi39_depth2");
  tempHist = (TH1D*) file->Get(tempname)->Clone();
  catLeg1->AddEntry(tempHist,"iphi: 39","l");
  tempHist->SetLineColor(2);
  tempHist->Rebin(4);
  tempHist->Draw("same");

  sprintf(tempname,"ieta30_iphi41_depth2");
  tempHist = (TH1D*) file->Get(tempname)->Clone();
  catLeg1->AddEntry(tempHist,"iphi: 41","l");
  tempHist->SetLineColor(3);
  tempHist->Rebin(4);
  tempHist->Draw("same");
  
    
  catLeg1->Draw();
  
  sprintf(tempname,"EnergyDistribution_iEta%d.png",iEta); 
  canvas->Print(tempname);
 
}
