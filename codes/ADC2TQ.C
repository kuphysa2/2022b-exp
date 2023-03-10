void ADC2TQ()
{
  gStyle->SetOptFit();
  float x[30],y[30];
  int i=0;
  TF1 f1("f1","[0]/pow((x-[1]),[2])+[3]");
  ifstream data("../exp0227/a0310/ADC2TQ.dat");
  while(!data.eof())
    {
      data >> x[i] >> y[i];
      i++;
    }
  TGraph *graph = new TGraph(16,x,y);
  graph->SetMarkerStyle(8);
  graph->Draw("AP");
  f1->SetParameters(1000,50,1,0);
  graph->Fit("f1");
}
