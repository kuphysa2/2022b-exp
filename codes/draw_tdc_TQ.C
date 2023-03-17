#include<fstream>
#include<iostream>
#include<TF1.h>
#include<TFile.h>
#include<TH1D.h>
#include<TROOT.h>
#include<TStyle.h>

void draw_tdc_TQ()
{
    int adc_channel = 1;
    int exp_date = 0;
    int ana_date = 317;
    char ifs_name[64];
    adc_channel--;
    snprintf(ifs_name, 64, "../exp%04d/a%04d/exp%04d_TQcor%d.dat", exp_date, ana_date, exp_date, adc_channel + 1);

    // Fitの統計情報を記載
    gStyle->SetOptFit(1111);

    TH1S *htdc = new TH1S("h1", "h1", 1000, 0, 1000);
    TH1S *hadc0 = new TH1S("h1", "h1", 1000, -500, 2800);
    TH1S *hadc1 = new TH1S("h1", "h1", 1000, -500, 2800);
    ifstream data(ifs_name);
    double tdc, adc[] = {0, 0};

    // input
    while (!data.eof())
    {
        data >> adc[0] >> adc[1] >> tdc;
        htdc->Fill(tdc);
        hadc0->Fill(adc[0]);
        hadc1->Fill(adc[1]);
    }

    // graph titles
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
    htdc->Fit("ftdc", "", "", 20, 360);
    // TF1 *f2 = new TF1(Form("fit%d", 2), "gaus", 1800, 2200);

    // drawing TDC histogram
    TCanvas *canvases[4];
    canvases[0] = new TCanvas(TString::Format("canvas%d", 0), "", 500, 500);
    gPad->SetLogy(1);
    htdc->Draw();
    canvases[0]->Update();
    char out_tdc_name[64];
    snprintf(out_tdc_name, 64, "../exp%04d/a%04d/tdcTQ%d.pdf", exp_date, ana_date, adc_channel + 1);
    canvases[0]->Print(out_tdc_name);
}
