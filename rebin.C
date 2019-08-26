#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>

#include <iostream>




TH1D* rebinHist(TH1D *h, TH1D* hTarget, string name="")
{
   TH1D *hout = (TH1D*)hTarget->Clone(name.c_str());
   TH1D *hcount = (TH1D*)hTarget->Clone((name+"cout").c_str());
   hout->Reset();
   hcount->Reset();
 
   for (int j=1;j<=h->GetNbinsX();j++) {
//      for (int i=1;i<=hTarget->GetNbinsX();i++) {
//         if (h->GetBinCenter(j)>hTarget->GetBinLowEdge()&&h->GetBinCenter(j)<hTarget->GetBinLowEdge()) {
	    hout->Fill(h->GetBinCenter(j),h->GetBinContent(j));
	    hcount->Fill(h->GetBinCenter(j));
//      }
   }
   hout->Divide(hcount);
   hout->Scale(hTarget->Integral()/hout->Integral());
   
   cout <<name<<" Chi2Test="<<hTarget->Chi2Test(hout,"WW P CHI2/NDF")<<endl;
   hout->SetTitle(Form("#chi^{2}/NDF=%f",hTarget->Chi2Test(hout,"WW P CHI2/NDF")));
   return hout;
}

void rebin()
{

   TCanvas *c5TeV = new TCanvas("c5TeV","",600,600);
   TCanvas *c7TeV = new TCanvas("c7TeV","",600,600);
   TFile *infData = new TFile("data/HEPData-ins914325-v1-Table_2.root");
   
   TH1D *hData7TeV   = (TH1D*) infData->Get("Table 2/Hist1D_y1");
   TH1D *hDataE1 = (TH1D*) infData->Get("Table 2/Hist1D_y1_e1");  // stat
   TH1D *hDataE2 = (TH1D*) infData->Get("Table 2/Hist1D_y1_e2");  // syst
   
   const int Nbin5TeV = 3;
   double bins5TeV[Nbin5TeV+1] = {7,15,20,50};
   
   TH1D *hData5TeV = new TH1D("hData5TeV","",Nbin5TeV,bins5TeV);
   hData5TeV->SetBinContent(1,316000);
   hData5TeV->SetBinContent(2,34100);
   hData5TeV->SetBinContent(3,3830);
   hData5TeV->SetBinError(1,sqrt(37000.*37000.+62000.*62000.));
   hData5TeV->SetBinError(2,sqrt(6300.*6300.+3200.*3200.));
   hData5TeV->SetBinError(3,sqrt(670.*670.+360.*360.));


   string titles[10];
   titles[1]="Central";
   titles[2]="min_mass";
   titles[3]="max_mass";
   titles[4]="fr=.5 .5";
   titles[5]="fr=2 2";
   titles[6]="fr= 2 1";
   titles[7]="fr= 1 2";
   titles[8]="fr= 1 .5";
   titles[9]="fr= .5 1";

   // Read errors, only consider stat and syst
   for (int i=1;i<=hData7TeV->GetNbinsX();i++) {
      double error = sqrt( hDataE1->GetBinContent(i)*hDataE1->GetBinContent(i) +
                           hDataE2->GetBinContent(i)*hDataE2->GetBinContent(i) ); 
      hData7TeV->SetBinError(i,error);
   }
   hData5TeV->SetMarkerStyle(20);
   hData7TeV->SetMarkerStyle(20);
   hData7TeV->SetXTitle("B_{s} [GeV]");
   hData7TeV->SetYTitle("d#sigma/dp_{T} [pb/GeV]");
   hData5TeV->SetXTitle("B_{s} [GeV]");
   hData5TeV->SetYTitle("d#sigma/dp_{T} [pb/GeV]");
   hData7TeV->Scale(1000);
   hData7TeV->SetLabelSize(hData5TeV->GetLabelSize("X"),"X");
   hData7TeV->SetLabelSize(hData5TeV->GetLabelSize("Y"),"Y");
   hData7TeV->SetTitleSize(hData5TeV->GetTitleSize("X"),"X");
   hData7TeV->SetTitleSize(hData5TeV->GetTitleSize("Y"),"Y");

   c5TeV->cd();   
   hData5TeV->Draw();
   c7TeV->cd();   
   hData7TeV->Draw();
   
 
   TFile *inf5TeV = new TFile("data/BmesonFONLL5TeV.root");
   TFile *inf7TeV = new TFile("data/BmesonFONLL7TeV.root");

   int color[10]={0,1,kCyan,kGreen+2,4,kGray,kRed+2,kBlue+2,2,kGreen+4};

   TH1D *h5TeV[10];
   TH1D *hout5TeV[10];
   TH1D *h7TeV[10];
   TH1D *hout7TeV[10];

    for (int i=1;i<=9;i++) {
       c5TeV->cd();
       h5TeV[i] = (TH1D*)inf5TeV->Get(Form("h%d",i));
       h5TeV[i]->Scale(0.11);
       hout5TeV[i] = rebinHist(h5TeV[i],hData5TeV,Form("hout%d",i));
       hout5TeV[i]->SetLineColor(color[i]);
       hout5TeV[i]->Draw("same hist");
       c7TeV->cd();
       h7TeV[i] = (TH1D*)inf7TeV->Get(Form("h%d",i));
       h7TeV[i]->Scale(0.11);
       hout7TeV[i] = rebinHist(h7TeV[i],hData7TeV,Form("hout%d",i));
       hout7TeV[i]->SetLineColor(color[i]);
       hout7TeV[i]->Draw("same hist");
    }

    TLegend *leg5TeV = new TLegend(0.5,0.5,0.9,0.9);
    TLegend *leg7TeV = new TLegend(0.5,0.5,0.9,0.9);
    
    for (int i=1;i<=9;i++){
       leg5TeV->AddEntry(hout5TeV[i],Form("%s,%s",hout5TeV[i]->GetTitle(),titles[i].c_str()),"l");
       leg7TeV->AddEntry(hout7TeV[i],Form("%s,%s",hout7TeV[i]->GetTitle(),titles[i].c_str()),"l");
    }
    c5TeV->cd();
    leg5TeV->Draw();
    c7TeV->cd();
    leg7TeV->Draw();
    c5TeV->SaveAs("Results_5TeV.png");
    c5TeV->SetLogy();
    c5TeV->Update();
    c5TeV->SaveAs("Results_5TeV_log.png");
    c7TeV->SaveAs("Results_7TeV.png");
    c7TeV->SetLogy();
    c7TeV->Update();
    c7TeV->SaveAs("Results_7TeV_log.png");
}
