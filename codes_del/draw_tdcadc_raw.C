#include <fstream>
#include <iostream>
#include <TF1.h>
#include <TFile.h>
#include <TH1D.h>
#include <TROOT.h>
#include <TStyle.h>

void draw_tdcadc_raw()
{
    int exp_date = 0;
    int ana_date = 317;
    int hist_minADC = 0;
    int ADC1Fit0[] = {140, 160};
    int ADC2Fit0[] = {160, 200};
    int ADC1Fit1274[] = {1800, 2100};
    int ADC2Fit1274[] = {1800, 2000};

    // Fitの統計情報を記載
    gStyle->SetOptFit(1111);

    TH1S *htdc = new TH1S("h1", "h1", 1000, 0, 1000);
    TH1S *hadc0 = new TH1S("h1", "h1", 1000, hist_minADC, 2800);
    TH1S *hadc1 = new TH1S("h1", "h1", 1000, hist_minADC, 2800);

    char ifs_name[64];
    snprintf(ifs_name, 64, "../exp%04d/a%04d/exp%04d.dat", exp_date, ana_date, exp_date);
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
    htdc->SetTitle("TDC; time [TDS]; count;");
    hadc0->SetTitle("ADC1 raw; energy [ADC]; count;");
    hadc1->SetTitle("ADC2 raw; energy [ADC]; count;");

    // Fitting
    TF1 *f00 = new TF1(Form("fit%d", 0), "gaus");
    hadc0->Fit(f00, "", "", ADC1Fit0[0], ADC1Fit0[1]);
    TF1 *f0 = new TF1(Form("fit%d", 0), "gaus");
    hadc0->Fit(f0, "", "", ADC1Fit1274[0], ADC1Fit1274[1]);
    TF1 *f10 = new TF1(Form("fit%d", 1), "gaus");
    hadc1->Fit(f10, "", "", ADC2Fit0[0], ADC2Fit0[1]);
    TF1 *f1 = new TF1(Form("fit%d", 1), "gaus");
    hadc1->Fit(f1, "", "", ADC2Fit1274[0], ADC2Fit1274[1]);

    // drawing TDC histogram
    TCanvas *canvases[4];
    canvases[0] = new TCanvas(TString::Format("canvas%d", 0), "", 500, 500);
    gPad->SetLogy(1);
    htdc->Draw();
    canvases[0]->Update();
    char tdc_ofs_name[64];
    snprintf(tdc_ofs_name, 64, "../exp%04d/a%04d/tdc_raw.pdf", exp_date, ana_date);
    canvases[0]->Print(tdc_ofs_name);

    // drawing ADC1 histogram
    canvases[1] = new TCanvas(TString::Format("canvas%d", 1), "", 500, 500);
    hadc0->Draw();
    canvases[1]->Update();
    char adc1_ofs_name[64];
    snprintf(adc1_ofs_name, 64, "../exp%04d/a%04d/adc1_raw.pdf", exp_date, ana_date);
    canvases[1]->Print(adc1_ofs_name);

    // drawing ADC2 histogram
    canvases[2] = new TCanvas(TString::Format("canvas%d", 2), "", 500, 500);
    hadc1->Draw();
    canvases[2]->Update();
    char adc2_ofs_name[64];
    snprintf(adc2_ofs_name, 64, "../exp%04d/a%04d/adc2_raw.pdf", exp_date, ana_date);
    canvases[2]->Print(adc2_ofs_name);
}
