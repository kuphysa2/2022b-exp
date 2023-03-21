#include <iostream>
#include <fstream>
#include <cstdio>
#include "TH2.h"
#include "TH2F.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TColor.h"
#include "TMath.h"

#define MAX_SEC 16 // ranges of E; E = E_0, E_1, ..., E_(MAX_SEC)
#define MAX_ROW 1500000
#define NAME_LEN 64

int TQcorrection()
{
    int adc_channel = 1; // 1 or 2
    int exp_date = 0;
    int ana_date = 320;

    // in case adc_channel is not valid
    if (adc_channel != 1 && adc_channel != 2)
    {
        std::cout << "Channel is not valid" << std::endl;
        return 0;
    }
    // adc_channel to 0 or 1
    adc_channel--;

    int row;
    int flag;
    Double_t tdc, adc[2];
    Double_t E[MAX_SEC] = {}; // center of energy ranges
    Double_t dt[MAX_SEC] = {};
    double E0 = 150;    // start point of E
    double dE = 20;     // E is set as E0, E0+dE, E0+2dE, ...
    double E_width = 5; // drawing histograms in range of E-E_width < energy < E+E_width
    int i;              // for energy range
    int j;              // for selection of ADC1/ADC2

    // initialize E; setting means of energy ranges
    for (i = 0; i < MAX_SEC; i++)
    {
        E[i] = E0 + i * dE;
    }

    // making histograms (before inputting data)
    TH1F *histograms[MAX_SEC];
    for (i = 0; i < MAX_SEC; i++)
    {
        histograms[i] = new TH1F(Form("histogram%d", i), Form("ADC%d Distribution (E=%f); energy [keV]; count", adc_channel + 1, E[i]), 75, 50, 200);
    }

    // input data
    row = 0;
    char ifs_name[NAME_LEN];
    sprintf(ifs_name, "../exp%04d/a%04d/exp%04d_acalib.dat", exp_date, ana_date, exp_date);
    std::ifstream ifs(ifs_name);
    while (!ifs.eof())
    {
        ifs >> adc[0] >> adc[1] >> tdc;
        row++;
        flag = 0;
        for (i = 0; i < MAX_SEC; i++)
        {
            if (adc[adc_channel] > E[i] - E_width && adc[adc_channel] < E[i] + E_width)
            {
                histograms[i]->Fill(tdc);
            }
        }
        // error
        if (row >= MAX_ROW)
        {
            std::cerr << "Error: too many rows in data file." << std::endl;
            return 1;
        }
    }
    ifs.close();

    // Fittings and recording dt
    char ofs_dt_name[NAME_LEN];
    sprintf(ofs_dt_name, "../exp%04d/a%04d/dt%d.dat", exp_date, ana_date, adc_channel + 1);
    std::ofstream ofs_dt(ofs_dt_name);
    TF1 *f[MAX_SEC];
    for (i = 0; i < MAX_SEC; i++)
    {
        // fitting
        f[i] = new TF1(Form("fit%d%d", adc_channel, i), "gaus");
        histograms[i]->Fit(f[i], "", "", 0, 500);
        dt[i] = f[i]->GetParameter(1);
        ofs_dt << E[i] << " " << dt[i] << std::endl;
    }

    // histograms output
    TCanvas *canvases[2];
    canvases[0] = new TCanvas("canvas", Form("ADC%d Distributions", adc_channel + 1), 1000, 800);
    canvases[0]->Divide(4, 4);
    for (int i = 0; i < MAX_SEC; i++)
    {
        // drawing ADC E[i] histogram
        canvases[0]->cd(i + 1);
        histograms[i]->Draw();
    }
    canvases[0]->Update();
    canvases[0]->Print(Form("../exp%04d/a%04d/pdf/TQ_Tdistrib%d.pdf", exp_date, ana_date, adc_channel + 1));
    canvases[0]->Print(Form("../exp%04d/a%04d/img/TQ_Tdistrib%d.png", exp_date, ana_date, adc_channel + 1));

    // E-dt fitting
    canvases[1] = new TCanvas("canvas", Form("ADC%d; E; dt;", adc_channel + 1));
    canvases[1]->Divide(1, 1);
    gStyle->SetOptFit();
    double x[MAX_SEC], y[MAX_SEC];
    TF1 f1("f1", "[0] / pow(x - [1], [2]) + [3]");
    for (i = 0; i < MAX_SEC; i++)
    {
        x[i] = E[i];
        y[i] = dt[i];
    }
    TGraph *graph = new TGraph(MAX_SEC, x, y);
    graph->SetMarkerStyle(8);
    f1.SetParameters(1000, 50, 1, 0);
    graph->Fit("f1");
    graph->SetTitle(Form("TQ correction f(t) Ch%d; E [keV]; dT [ns];", adc_channel + 1));
    graph->Draw("AP");
    canvases[1]->Print(Form("../exp%04d/a%04d/pdf/EdT%d.pdf", exp_date, ana_date, adc_channel + 1));
    canvases[1]->Print(Form("../exp%04d/a%04d/img/EdT%d.png", exp_date, ana_date, adc_channel + 1));

    // get the formula
    double p[4];
    for (j = 0; j < 4; j++)
    {
        p[j] = f1.GetParameter(j);
    }

    // rewrite data
    char ofs_name[NAME_LEN];
    snprintf(ofs_name, NAME_LEN, "../exp%04d/a%04d/exp%04d_TQcor%d.dat", exp_date, ana_date, exp_date, adc_channel + 1);
    std::ifstream ifs2(ifs_name);
    std::ofstream ofs(ofs_name);
    while (!ifs2.eof())
    {
        ifs2 >> adc[0] >> adc[1] >> tdc;
        if (adc[0] > 20)
        {
            ofs << adc[0] << " " << adc[1] << " " << tdc - p[0] / pow(adc[0] - p[1], p[2]) - p[3] << std::endl;
        }
        else if (adc[1] > 20)
        {
            ofs << adc[0] << " " << adc[1] << " " << tdc - p[0] / pow(adc[1] - p[1], p[2]) - p[3] << std::endl;
        }
    }
    ifs2.close();
    ofs.close();

    return 0;
}
