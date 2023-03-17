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

using namespace std;

#define MAX_SEC 13 // ranges of t; t = t_0, t_1, ..., t_(MAX_SEC)
#define MAX_ROW 600000

void pickoff()
{
    int adc_channel = 1; // 1 or 2
    int exp_date = 0;
    int ana_date = 317;

    adc_channel--;
    int row;
    Double_t tdc, adc[2];
    Double_t t[MAX_SEC + 1] = {}; // center of time ranges
    Double_t yt[MAX_SEC + 1] = {}, St[MAX_SEC + 1] = {};
    Double_t ft[MAX_SEC + 1];
    Double_t p[3];
    int Erange4y[] = {650, 750};
    int Erange4S[] = {10, 450};
    int NBins4y = 20;
    int NBins4S = 200;
    int NBins4TDC = 200;
    double gFitRange[] = {50, 300};
    double t0 = 100;     // start point of t
    double dt = 50;      // t is set as t0, t0 + dt, ...
    double t_width = 25; // drawing histograms in range of t-t_width < t < t+t_width
    int i;               // for time range
    int j;  // for others

    // initialize t; setting means of time ranges
    t[0] = 0;
    for (i = 1; i < MAX_SEC + 1; i++)
    {
        t[i] = t0 + i * dt;
    }

    // making histograms (before inputting data)
    TH1F *histogramsY[MAX_SEC + 1];
    TH1F *histogramsS[MAX_SEC + 1];
    TH1S *histogramTDC = new TH1S("h1", "h1", NBins4TDC, 0, 1000);
    for (i = 0; i < MAX_SEC + 1; i++)
    {
        histogramsY[i] = new TH1F(Form("histogramY%d", i), Form("ADC%d distrib. at E~511 keV (t=%f)", adc_channel + 1, t[i]), NBins4y, Erange4y[0], Erange4y[1]);
        histogramsS[i] = new TH1F(Form("histogramS%d", i), Form("ADC%d distrib. at %d<E<%d keV (t=%f)", adc_channel + 1, Erange4S[0], Erange4S[1], t[i]), NBins4S, Erange4S[0], Erange4S[1]);
    }

    // input data
    row = 0;
    char ifs_name[64];
    snprintf(ifs_name, 64, "../exp%04d/a%04d/exp%04d_TQcor%d.dat", exp_date, ana_date, exp_date, adc_channel + 1);
    cout << ifs_name << endl;
    ifstream ifs(ifs_name);
    while (!ifs.eof())
    {
        ifs >> adc[0] >> adc[1] >> tdc;
        row++;
        if ((adc[0] > 20 && adc[0] < 750) || (adc[1] > 20 && adc[1] < 750))
            histogramTDC->Fill(tdc);
        for (i = 0; i < MAX_SEC + 1; i++)
        {
            if (tdc > t[i] - t_width && tdc < t[i] + t_width)
            {
                if (adc[adc_channel] >= Erange4y[0] && adc[adc_channel] <= Erange4y[1])
                {
                    histogramsY[i]->Fill(adc[adc_channel]);
                }
                else if (adc[adc_channel] >= Erange4S[0] && adc[adc_channel] <= Erange4S[1])
                {
                    histogramsS[i]->Fill(adc[adc_channel]);
                }
            }
        }
        // error
        if (row >= MAX_ROW)
        {
            cerr << "Error: too many rows in data file." << std::endl;
            return 1;
        }
    }
    ifs.close();

    // recording y(t)
    char ofs_yt_name[64];
    snprintf(ofs_yt_name, 64, "../exp%04d/a%04d/yt%d.dat", exp_date, ana_date, adc_channel + 1);
    ofstream ofs_yt(ofs_yt_name);
    for (i = 0; i < MAX_SEC + 1; i++)
    {
        yt[i] = histogramsY[i]->GetEntries();
        St[i] = histogramsS[i]->GetEntries();
        if (i != 0)
        {
            ft[i] = St[0] * yt[i] / (St[i] * yt[0] - St[0] * yt[i]);
        }
        ofs_yt << t[i] << " " << yt[i] << " " << St[i] << " " << ft[i] << endl;
    }

    // histograms output
    TCanvas *canvases[4];
    canvases[0] = new TCanvas("canvas0", Form("ADC%d Distributions at E~511 keV", adc_channel + 1), 1000, 800);
    canvases[1] = new TCanvas("canvas1", Form("ADC%d Distrib. at %d<E<%d keV", adc_channel + 1, Erange4S[0], Erange4S[1]), 1000, 800);
    canvases[0]->Divide(4, 4);
    canvases[1]->Divide(4, 4);
    for (i = 0; i < MAX_SEC + 1; i++)
    {
        // drawing ADC t[i] histogram
        canvases[0]->cd(i + 1);
        histogramsY[i]->Draw();
        canvases[1]->cd(i + 1);
        histogramsS[i]->Draw();
    }
    canvases[0]->Update();
    canvases[0]->Print(Form("../exp%04d/a%04d/PickOff4yDistrib%d.pdf", exp_date, ana_date, adc_channel + 1));
    canvases[1]->Update();
    canvases[1]->Print(Form("../exp%04d/a%04d/PickOff4SDistrib%d.pdf", exp_date, ana_date, adc_channel + 1));

    // t-f(t) fitting
    canvases[2] = new TCanvas("canvas2", "f-t; t; f(t);");
    canvases[2]->Divide(1, 1);
    gStyle->SetOptFit();
    TF1 fFit("fFit", "[0] * exp(-x / [1]) + [2]");
    fFit.SetParameters(1, 511, 0);
    fFit.SetParLimits(0, 0, 100000);
    fFit.SetParLimits(2, 0, 100000);
    TGraph *graph_f = new TGraph(MAX_SEC + 1, t, ft);
    graph_f->SetMarkerStyle(8);
    graph_f->Fit("fFit");
    graph_f->Draw("AP");
    canvases[2]->Update();
    canvases[2]->Print(Form("../exp%04d/a%04d/ft%d.pdf", exp_date, ana_date, adc_channel + 1));
    for (j = 0; j < 3; j++)
    {
        p[j] = fFit.GetParameter(j);
    }

    // g(t) fitting
    canvases[3] = new TCanvas("canvas3", "g-t; t; g(t);", 600, 600);
    canvases[3]->Divide(1, 1);
    gStyle->SetOptFit();
    gPad->SetLogy(1);
    TF1 *gFit = new TF1("gFit", Form("[0] * (%f * exp(-x / %f) + %f + 1) * exp(-1 / [1] * (-%f * %f * exp(-x / %f) + (1 + %f) * x)) + [2]", p[0], p[1], p[2], p[0], p[1], p[1], p[2]));
    gFit->SetParameters(1, 100, 100);
    gFit->SetParNames("q0", "q1", "q2");
    histogramTDC->Fit(gFit, "", "", gFitRange[0], gFitRange[1]);
    histogramTDC->Draw();
    canvases[3]->Update();
    canvases[3]->Print(Form("../exp%04d/a%04d/tdcPick%d.pdf", exp_date, ana_date, adc_channel + 1));

    return;
}
