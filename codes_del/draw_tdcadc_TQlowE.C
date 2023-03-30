#include <fstream>
#include <iostream>
#include <TF1.h>
#include <TFile.h>
#include <TH1D.h>
#include <TROOT.h>
#include <TStyle.h>
#define NAME_LEN 64
#define TIME_FLOOR 50
#define MAX_ENERGY 450

void draw_tdcadc_TQlowE()
{
    int adc_channel = 2;
    int exp_date = 0;
    int ana_date = 320;
    double tFitRange[] = {72.5, 654};
    double adcRange[] = {50, 2500};
    char ifs_name[NAME_LEN];
    adc_channel--;
    snprintf(ifs_name, NAME_LEN, "../exp%04d/a%04d/exp%04d_TQcor%d.dat", exp_date, ana_date, exp_date, adc_channel + 1);

    // Fitの統計情報を記載
    gStyle->SetOptFit(1111);

    TH1S *htdc = new TH1S("TDC", "TDC", 1000, 0, 1000);
    TH1S *hadc = new TH1S("ADC", "ADC", 1000, adcRange[0], adcRange[1]);
    // TH1S *hadc0 = new TH1S("h1", "h1", 1000, -500, 2800);
    // TH1S *hadc1 = new TH1S("h1", "h1", 1000, -500, 2800);
    ifstream data(ifs_name);
    double tdc, adc[] = {0, 0};

    // input
    while (!data.eof())
    {
        data >> adc[0] >> adc[1] >> tdc;
        if (tdc > TIME_FLOOR && adc[adc_channel] < MAX_ENERGY)
        {
            htdc->Fill(tdc);
            hadc->Fill(adc[adc_channel]);
        }
    }

    // graph titles
    htdc->SetTitle(Form("TDC Ch. %d corrected (t>%d, ADC%d<%d); time [ns]; count;", adc_channel + 1, TIME_FLOOR, adc_channel + 1, MAX_ENERGY));
    hadc->SetTitle(Form("ADC Ch. %d corrected (t>%d, ADC%d<%d); energy [keV]; count;", adc_channel + 1, TIME_FLOOR, adc_channel + 1, MAX_ENERGY));

    // Fitting
    TF1 *ftdc = new TF1("ftdc", "[0] * exp(-(x + [1]) / [2]) + [3]");
    ftdc->SetParameters(1, 600, 100, -20);
    ftdc->SetParNames("N_0", "x_0", "#tau", "BG");
    htdc->Fit("ftdc", "", "", tFitRange[0], tFitRange[1]);

    // drawing TDC histogram
    TCanvas *canvases[4];
    canvases[0] = new TCanvas(TString::Format("canvas%d", 0), "", 500, 500);
    gPad->SetLogy(1);
    htdc->Draw();
    canvases[0]->Update();
    char out_tdc_name[NAME_LEN];
    snprintf(out_tdc_name, NAME_LEN, "../exp%04d/a%04d/pdf/t_cut/tdcTQ%dlowE_Tover%d.pdf", exp_date, ana_date, adc_channel + 1, TIME_FLOOR);
    canvases[0]->Print(out_tdc_name);

    // drawing ADC histogram
    // canvases[1] = new TCanvas(TString::Format("canvas%d", 1), "", 500, 500);
    // gPad->SetLogy(0);
    // hadc->Draw();
    // canvases[1]->Update();
    // char out_adc_name[NAME_LEN];
    // snprintf(out_adc_name, NAME_LEN, "../exp%04d/a%04d/pdf/t_cut/adcTQ%dlowE_Tover%d.pdf", exp_date, ana_date, adc_channel + 1, TIME_FLOOR);
}
