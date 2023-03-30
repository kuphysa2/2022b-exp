#include <fstream>
#include <iostream>
#include <TF1.h>
#include <TFile.h>
#include <TH1D.h>
#include <TROOT.h>
#include <TStyle.h>
#define NAME_LEN 64

void draw_tdcadc()
{
    int exp_date = 320;
    int ana_date = 327;
    int hist_minADC = 20;
    int tdc_nBins = 1000;
    int adc_nBins = 1000;
    int tdc_range[] = {0, 1000};
    int adc_range[] = {20, 2800};
    int tFit_range[] = {150+72, 150+654};

    // Fitの統計情報を記載
    gStyle->SetOptFit(1111);

    TH1S *htdc = new TH1S("h1", "h1", tdc_nBins, tdc_range[0], tdc_range[1]);
    TH1S *hadc0 = new TH1S("h1", "h1", adc_nBins, adc_range[0], adc_range[1]);
    TH1S *hadc1 = new TH1S("h1", "h1", adc_nBins, adc_range[0], adc_range[1]);

    char ifs_name[NAME_LEN];
    snprintf(ifs_name, NAME_LEN, "../exp%04d/a%04d/exp%04d_acalib.dat", exp_date, ana_date, exp_date);
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

    TF1 *ftdc = new TF1("ftdc", "[0] * exp(-(x + [1]) / [2]) + [3]");
    ftdc->SetParameters(1, 600, 100, -20);
    ftdc->SetParNames("N_0", "x_0", "#tau", "BG");
    htdc->Fit("ftdc", "", "", tFit_range[0], tFit_range[1]);

    TCanvas *canvases[4];
    // drawing TDC histogram
    canvases[0] = new TCanvas(TString::Format("canvas%d", 0), "", 500, 500);
    gPad->SetLogy(1);
    htdc->Draw();
    canvases[0]->Update();
    char tdc_ofs_name[NAME_LEN];
    snprintf(tdc_ofs_name, NAME_LEN, "../exp%04d/a%04d/pdf/tdc.pdf", exp_date, ana_date);
    canvases[0]->Print(tdc_ofs_name);
    snprintf(tdc_ofs_name, NAME_LEN, "../exp%04d/a%04d/img/tdc.png", exp_date, ana_date);
    canvases[0]->Print(tdc_ofs_name);

    // drawing ADC1 histogram
    canvases[1] = new TCanvas(TString::Format("canvas%d", 1), "", 500, 500);
    hadc0->Draw();
    canvases[1]->Update();
    char adc1_ofs_name[NAME_LEN];
    snprintf(adc1_ofs_name, NAME_LEN, "../exp%04d/a%04d/pdf/adc1.pdf", exp_date, ana_date);
    canvases[1]->Print(adc1_ofs_name);
    snprintf(adc1_ofs_name, NAME_LEN, "../exp%04d/a%04d/img/adc1.png", exp_date, ana_date);
    canvases[1]->Print(adc1_ofs_name);

    // drawing ADC2 histogram
    canvases[2] = new TCanvas(TString::Format("canvas%d", 2), "", 500, 500);
    hadc1->Draw();
    canvases[2]->Update();
    char adc2_ofs_name[NAME_LEN];
    snprintf(adc2_ofs_name, NAME_LEN, "../exp%04d/a%04d/pdf/adc2.pdf", exp_date, ana_date);
    canvases[2]->Print(adc2_ofs_name);
    snprintf(adc2_ofs_name, NAME_LEN, "../exp%04d/a%04d/img/adc2.png", exp_date, ana_date);
    canvases[2]->Print(adc2_ofs_name);
}
