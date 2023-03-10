void  TQls()
{
  gStyle->SetOptFit();
  gStyle->SetOptLogy();
  TH1S *tq = new TH1S("tq","ADC1 TQcor", 425, 0, 900);
  ifstream data("../exp0227/a0310/exp0227_TQcor.dat");
  double t,a1,a2;
  while(data >> a1 >> a2 >> t)if(20 < a1 && a1 < 500)tq->Fill(t);
  tq->Draw();
  TF1 f1("f1","[0]*exp(-x/[1])+[2]");
  f1->SetParameters(300,50,50);
  tq->Fit("f1","","",30,654);
}
