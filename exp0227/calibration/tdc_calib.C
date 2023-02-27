#include <fstream>
#include <iostream>
#include <TF1.h>
#include <TFile.h>
#include <TH1D.h>
#include <TROOT.h>
#include <TStyle.h>

void tdc_calib()
{
    int channel = 12;

    TH1S *h1 = new TH1S("h0", "h0", 1000, 300, 500);
    h1->SetTitle(TString::Format("TDC calibration;;"));
    // ifstream dataSr("../data/Srch12.dat");
    ifstream dataNa("../data/TDCch093ns.dat");
    // ifstream dataCs("../data/Csch12.dat");
    // ifstream dataCo("../data/Coch12.dat");
    // ifstream dataBkg("../data/Bkgch12.dat");

    double x, y;
    // while (dataSr >> x >> y)
    //     if (x > 0)
    //         h0->Fill(x);
    while (dataNa >> x >> y)
        if (x > 0)
            h1->Fill(x);
    // while (dataCs >> x >> y)
    //     if (x > 0)
    //         h2->Fill(x);
    // while (dataCo >> x >> y)
    //     if (x > 0)
    //         h3->Fill(x);
    // while (dataBkg >> x >> y)
    //     if (x > 0)
    //         h4->Fill(x);

    TCanvas *canvases[8];

    // Sr
    canvases[0] = new TCanvas(TString::Format("canvas%d", 0), "", 500, 500);
    // h0->Fit("gaus", "", "", 0, 4500);
    // h0->Draw();
    // canvases[0]->Update();
    // canvases[0]->Print(TString::Format("../data/Srch%d.pdf", channel));

    // Na
    canvases[1] = new TCanvas(TString::Format("canvas%d", 1), "", 500, 500);
    h1->Fit("gaus", "", "", 0, 4500);
    h1->Draw();
    canvases[1]->Update();
    canvases[1]->Print(TString::Format("../data/TDCch0_93ns.pdf"));

    // // Cs
    // canvases[2] = new TCanvas(TString::Format("canvas%d", 2), "", 500, 500);
    // h2->Fit("gaus", "", "", 0, 4500);
    // h2->Draw();
    // canvases[2]->Update();
    // canvases[2]->Print(TString::Format("../data/Csch%d.pdf", channel));

    // // Co
    // canvases[3] = new TCanvas(TString::Format("canvas%d", 3), "", 500, 500);
    // h3->Fit("gaus", "", "", 0, 4500);
    // h3->Draw();
    // canvases[3]->Update();
    // canvases[3]->Print(TString::Format("../data/Coch%d.pdf", channel));

    // // Bkg
    // canvases[4] = new TCanvas(TString::Format("canvas%d", 4), "", 500, 500);
    // h4->Fit("gaus", "", "", 0, 4500);
    // h4->Draw();
    // canvases[4]->Update();
    // canvases[4]->Print(TString::Format("../data/Bkgch%d.pdf", channel));
}
