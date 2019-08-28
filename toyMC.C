#include <TRandom.h>
#include <TH1D.h>
#include <TF1.h>

void toyMC()
{
   TF1 *f = new TF1("f","[0]*TMath::Gaus(x,[1],[2])");
   f->SetParameters(1,10,1);
   f->SetRange(5,15);
   
   TH1D *h = new TH1D("h","",100,5,15);
   
   for (int i=0;i<10000;i++) {
   
      h->Fill(f->GetRandom());
   }
   h->Draw();
}
