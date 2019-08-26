#include <TFile.h>
#include <TH1D.h>

void multiplyTAMU(TH1D *h,TH1D *hTAMU)
{
   double RAA=1;
   // Correct by TAMU up to 30 GeV, beyond that, if no TAMU, then use RAA at 30 GeV
   for (int i=1;i<=h->GetNbinsX();i++) {
      if (hTAMU->GetBinContent(i)!=0) {
         RAA=hTAMU->GetBinContent(i);
      }
      h->SetBinContent(i,h->GetBinContent(i)*RAA);
   }
}

void reference()
{
   TFile *infTAMU = new TFile("tamu.root");
   TFile *infFONLL = new TFile("data/BmesonFONLL5TeV.root");

   TFile *outf = new TFile("spectraVariation.root","recreate");
   
   TH1D *hNominalPP = (TH1D*)infFONLL->Get("h8");
   hNominalPP->SetName("hNominalPP");
   hNominalPP->Write();
   TH1D *hVariationPP = (TH1D*)infFONLL->Get("h6");
   hVariationPP->SetName("hVariationPP");
   hVariationPP->Write();
   
   TH1D *hTAMU = (TH1D*)infTAMU->Get("h");
   TH1D *hNominalTAMU = (TH1D*)hNominalPP->Clone("hNominalTAMU");
   TH1D *hVariationTAMU = (TH1D*)hVariationPP->Clone("hVariationTAMU");
   multiplyTAMU(hNominalTAMU,hTAMU);
   multiplyTAMU(hVariationTAMU,hTAMU);
   hNominalTAMU->Write();
   hVariationTAMU->Write();
   
}
