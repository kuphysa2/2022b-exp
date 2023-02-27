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

    TH1S *htdc = new TH1S("h1", "h1", 1000, 0, 4000);
    TH1S *hadc0 = new TH1S("h1", "h1", 1000, 0, 4500);
    TH1S *hadc1 = new TH1S("h1", "h1", 1000, 0, 4500);
    TH1S *hadc2 = new TH1S("h1", "h1", 1000, 0, 4500);
    ifstream data("../exp0216/a0227/exp0216_halved.dat");
    double tdc, adc[] = {0, 0, 0};
    while (!data.eof())
    {
        data >> adc[0] >> adc[1] >> adc[2] >> tdc;
        htdc->Fill(tdc);
        hadc0->Fill(adc[0]);
        hadc1->Fill(adc[1]);
        hadc2->Fill(adc[2]);
    }
    htdc->SetTitle("TDC; value; count;");
    hadc0->SetTitle("ADC1; value; count;");
    hadc1->SetTitle("ADC2; value; count;");
    hadc2->SetTitle("ADC3; value; count;");

    // ADC calibration
    TF1 *f0 = new TF1(Form("fit%d", 0), "gaus");
    hadc0->Fit(f0, "", "", 1850, 2200);
    TF1 *f1 = new TF1(Form("fit%d", 1), "gaus");
    hadc1->Fit(f1, "", "", 1650, 1900);
    // TF1 *f2 = new TF1(Form("fit%d", 2), "gaus", 1800, 2200);

    TCanvas *canvases[4];
    canvases[0] = new TCanvas(TString::Format("canvas%d", 0), "", 500, 500);
    gPad->SetLogy(1);
    htdc->Draw();
    canvases[1] = new TCanvas(TString::Format("canvas%d", 1), "", 500, 500);
    hadc0->Draw();
    canvases[2] = new TCanvas(TString::Format("canvas%d", 2), "", 500, 500);
    hadc1->Draw();
    canvases[3] = new TCanvas(TString::Format("canvas%d", 3), "", 500, 500);
    hadc2->Draw();
}
