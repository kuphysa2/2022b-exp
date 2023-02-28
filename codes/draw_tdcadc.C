#include<fstream>
#include<iostream>
#include<TF1.h>
#include<TFile.h>
#include<TH1D.h>
#include<TROOT.h>
#include<TStyle.h>

void draw_tdcadc()
{
    // Fitの統計情報を記載
    gStyle->SetOptFit(1111);

    TH1S *htdc = new TH1S("h1", "h1", 1000, 0, 1000);
    TH1S *hadc0 = new TH1S("h1", "h1", 1000, -500, 2800);
    TH1S *hadc1 = new TH1S("h1", "h1", 1000, -500, 2800);
    ifstream data("../exp0227/a0228/exp0227_acalib.dat");
    double tdc, adc[] = {0, 0};
    while (!data.eof())
    {
        data >> adc[0] >> adc[1] >> tdc;
        htdc->Fill(tdc);
        hadc0->Fill(adc[0]);
        hadc1->Fill(adc[1]);
    }

    // titles
    htdc->SetTitle("TDC; time [ns]; count;");
    hadc0->SetTitle("ADC1; energy [keV]; count;");
    hadc1->SetTitle("ADC2; energy [keV]; count;");

    // Fitting
    TF1 *f0 = new TF1(Form("fit%d", 0), "gaus");
    hadc0->Fit(f0, "", "", 200, 300);
    TF1 *f1 = new TF1(Form("fit%d", 1), "gaus");
    hadc1->Fit(f1, "", "", 1650, 1900);
    TF1 *ftdc = new TF1("ftdc", "[0] * exp(-(x + [1]) / [2]) + [3]");
    ftdc->SetParameters(1, 600, 100, -20);
    ftdc->SetParNames("N_0", "x_0", "#tau", "BG");
    htdc->Fit("ftdc", "", "", 280, 360);
    // TF1 *f2 = new TF1(Form("fit%d", 2), "gaus", 1800, 2200);

    TCanvas *canvases[4];
    canvases[0] = new TCanvas(TString::Format("canvas%d", 0), "", 500, 500);
    gPad->SetLogy(1);
    htdc->Draw();
    canvases[0]->Update();
    canvases[0]->Print("../exp0227/a0228/tdc.pdf");

    canvases[1] = new TCanvas(TString::Format("canvas%d", 1), "", 500, 500);
    hadc0->Draw();
    canvases[1]->Update();
    canvases[1]->Print("../exp0227/a0228/adc1.pdf");

    canvases[2] = new TCanvas(TString::Format("canvas%d", 2), "", 500, 500);
    hadc1->Draw();
    canvases[2]->Update();
    canvases[2]->Print("../exp0227/a0228/adc2.pdf");
}
