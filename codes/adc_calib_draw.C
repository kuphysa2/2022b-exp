#include <fstream>
#include <iostream>
#include <TF1.h>
#include <TFile.h>
#include <TH1D.h>
#include <TROOT.h>
#include <TStyle.h>
#define NAME_LEN 64

void adc_calib_draw()
{
    int exp_date = 320;
    int ana_date = 323;
    int nBins[] = {50, 100};
    int E_val[] = {0, 1275};
    // channel, energy, up/down
    int range[2][2][2] = {{{-100, 200}, {1800, 2200}}, {{-100, 200}, {1750, 2300}}};
    int fit_range[][2][2] = {{{-100, 200}, {1800, 2200}}, {{-100, 200}, {1750, 2300}}};
    int tdc, adc[2];
    int energy, channel;
    // channel, energy
    int means[2][2], sigmas[2][2];

    // Fitの統計情報を記載
    gStyle->SetOptFit(1111);

    TH1S *hist[2][2];
    TF1 *fit[4];
    for (channel = 0; channel < 2; channel++)
    {
        for (energy = 0; energy < 2; energy++)
        {
            hist[channel][energy] = new TH1S(Form("ADC%d(E~%d)", channel + 1, E_val[energy]), Form("ADC%d(E~%d)", channel + 1, E_val[energy]), nBins[energy], range[channel][energy][0], range[channel][energy][1]);
            hist[channel][energy]->SetTitle(Form("ADC%d(E~%d); energy [ADC]; count", channel + 1, E_val[energy]));
        }
    }
    fit[0] = new TF1("f0", "gaus", fit_range[0][0][0], fit_range[0][0][1]);
    fit[1] = new TF1("f1", "gaus", fit_range[0][1][0], fit_range[0][1][1]);
    fit[2] = new TF1("f2", "gaus", fit_range[1][0][0], fit_range[1][0][1]);
    fit[3] = new TF1("f3", "gaus", fit_range[1][1][0], fit_range[1][1][1]);

    // input
    char ifs_name[NAME_LEN];
    snprintf(ifs_name, NAME_LEN, "../exp%04d/a%04d/exp%04d.dat", exp_date, ana_date, exp_date);
    ifstream data(ifs_name);
    int flag = 0;
    while (!data.eof())
    {
        data >> adc[0] >> adc[1] >> tdc;

        for (channel = 0; channel < 2; channel++)
        {
            for (energy = 0; energy < 2; energy++)
            {
                hist[channel][energy]->Fill(adc[channel]);
            }
        }
    }
    std::cout << flag << endl;

    TCanvas *canvases[3][2];
    // Ch 1
    // E=0
    hist[0][0]->Fit("f0");
    hist[0][1]->Fit("f1");
    hist[1][0]->Fit("f2");
    hist[1][1]->Fit("f3");

    for (channel = 0; channel < 2; channel++)
    {
        for (energy = 0; energy < 2; energy++)
        {
            canvases[channel][energy] = new TCanvas(Form("ADC%d(E~%d)", channel + 1, E_val[energy]), Form("ADC%d(E~%d)", channel + 1, E_val[energy]), 500, 500);
            if (energy == 0)
            {
                gStyle->SetStatX(0.6);
            }
            else
            {
                gStyle->SetStatX(0.8);
            }
            hist[channel][energy]->Draw();
            canvases[channel][energy]->Update();
            canvases[channel][energy]->Print(Form("../exp%04d/a%04d/pdf/adc%d_%d.pdf", exp_date, ana_date, channel + 1, E_val[energy]));
            means[channel][energy] = fit[channel * 2 + energy]->GetParameter(1);
            sigmas[channel][energy] = fit[channel * 2 + energy]->GetParError(1);

        }
    }

    for ( channel = 0; channel < 2; channel++)
    {
        for ( energy = 0; energy < 2; energy++)
        {
            std::cout << means[channel][energy] << " " << sigmas[channel][energy] << endl;
        }
    }


    // graph draw
    canvases[2][0] = new TCanvas("", "", 500, 500);
    gStyle->SetOptStat(1111);
    gStyle->SetStatX(0.5);
    gStyle->SetStatY(0.8);
    TGraph *graph1 = new TGraph(2, means[0], E_val);
    graph1->SetMarkerStyle(8);
    graph1->Fit("pol1");
    graph1->SetTitle("ADC1 calibration; ADC value; energy [keV];");
    graph1->Draw("AP");
    canvases[2][0]->Update();
    canvases[2][0]->Print(Form("../exp%04d/a%04d/pdf/ADC1_calib.pdf", exp_date, ana_date));
    canvases[2][1] = new TCanvas("", "", 500, 500);
    gStyle->SetOptFit();
    TGraph *graph2 = new TGraph(2, means[1], E_val);
    graph2->SetMarkerStyle(8);
    graph2->Fit("pol1");
    graph2->SetTitle("ADC2 calibration; ADC value; energy [keV];");
    graph2->Draw("AP");
    canvases[2][1]->Update();
    canvases[2][1]->Print(Form("../exp%04d/a%04d/pdf/ADC2_calib.pdf", exp_date, ana_date));

    return;
}
