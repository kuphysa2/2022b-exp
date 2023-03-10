#include <iostream>
#include <fstream>
#include "TH2.h"
#include "TH2F.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TColor.h"
#include "TMath.h"

#define MAX_SEC 16 // ranges of E; E = E_0, E_1, ..., E_(MAX_SEC)
#define MAX_ROW 600000

int TQcorrection(int adc_channel = 1)
{
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
    Double_t E[MAX_SEC] = {};   // center of energy ranges
    Double_t dt[2][MAX_SEC] = {};
    double E0 = 150;     // start point of E
    double dE = 20;      // E is set as E0, E0+dE, E0+2dE, ...
    double E_width = 5; // drawing histograms in range of E-E_width < energy < E+E_width
    int i;               // for energy range
    int j;               // for selection of ADC1/ADC2

    // initialize E; setting means of energy ranges
    for (i = 0; i < MAX_SEC; i++)
    {
        E[i] = E0 + i * dE;
    }

    // making histograms (before inputting data)
    TH1F *histograms[MAX_SEC];
    for (i = 0; i < MAX_SEC; i++)
    {
        histograms[i] = new TH1F(Form("histogram%d", i), Form("ADC%d Distribution (E=%f)", adc_channel + 1, E[i]), 75, 50, 200);
    }

    // input data
    row = 0;
    std::ifstream ifs("../exp0227/a0310/exp0227_acalib.dat");
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

    // Fittings
    TF1 *f[MAX_SEC];
    for (i = 0; i < MAX_SEC; i++)
    {
        // fitting
        f[i] = new TF1(Form("fit%d%d", adc_channel, i), "gaus");
        histograms[i]->Fit(f[i], "", "", 0, 500);
    }


    // output
    TCanvas *canvases[2];
    TCanvas *canvas = new TCanvas("canvas", Form("ADC%d Distributions", adc_channel + 1), 1000, 800);
    canvas->Divide(4, 4);
    for (int i = 0; i < MAX_SEC; i++)
    {
        // drawing ADC E[i] histogram
        canvas->cd(i + 1);
        histograms[i]->Draw();
    }
    canvas->Update();
    canvas->Print(Form("../exp0227/a0310/TQ_Tdistrib%d.pdf", adc_channel + 1));

    return 0;
}

int main(int argc, char *argv[])
{
    // default value
    int adc_channel = 1;    // 1 or 2

    // changing value in case of input
    if (argc > 1)
    {
        adc_channel = atof(argv[1]);
    }

    TQcorrection(adc_channel);

    return 0;
}
