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
#define MAX_ROW 100000

void TQcorrection()
{
    Double_t tdc, adc[2];
    int row;
    int flag;
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

    // TEST: making histograms (before inputting data)
    TH1F *histograms[MAX_SEC];
    for (i = 0; i < MAX_SEC; i++)
    {
        histograms[i] = new TH1F(Form("histogram%d", i), Form("ADC1 Distrib. (E=%f)", E[i]), 100, E[i] - E_width, E[i] + E_width);
    }
    // // making histograms (before inputting data)
    // TH1F *histograms[MAX_SEC][2];
    // for (i = 0; i < MAX_SEC; i++)
    // {
    //     for (j = 0; j < 2; j++)
    //     {
    //         histograms[i][j] = new TH1F(Form("histogram%d", i), Form("ADC%d Distribution (E=%f)", j + 1, E[i]), 100, E[i] - E_width, E[i] + E_width);
    //     }
    // }

    // TEST: input data
    row = 0;
    std::ifstream ifs("../exp0227/a0228/exp0227_acalib.dat");
    while (!ifs.eof())
    {
        ifs >> adc[0] >> adc[1] >> tdc;
        row++;
        flag = 0;

        for (i = 0; i < MAX_SEC; i++)
        {
            for (j = 0; j < 2; j++)
            {
                if (adc[j] > E[i] - E_width && adc[j] < E[i] + E_width)
                {
                    histograms[i]->Fill(tdc);
                    // flag++;
                }
            }
            // if (flag == 2) // when both histograms are filled
            //     break;
        }
    }
    ifs.close();

    // // input data
    // row = 0;
    // std::ifstream ifs("../exp0227/a0228/exp0227_acalib.dat");
    // while (!ifs.eof())
    // {
    //     ifs >> adc[0] >> adc[1] >> tdc;
    //     row++;
    //     flag = 0;

    //     for (i = 0; i < MAX_SEC; i++)
    //     {
    //         for (j = 0; j < 2; j++)
    //         {
    //             if (adc[j] > E[i] - E_width && adc[j] < E[i] + E_width)
    //             {
    //                 histograms[i][j]->Fill(tdc);
    //                 flag++;
    //             }
    //         }
    //         if (flag == 2) // when both histograms are filled
    //             break;
    //     }

    //     if (row >= MAX_ROW)
    //     {
    //         std::cerr << "Error: too many rows in data file." << std::endl;
    //         return 1;
    //     }
    // }
    // ifs.close();

    // TEST: output
    TCanvas *c1 = new TCanvas("canvas", "test", 800, 600);
    histograms[6]->Draw();

    // // output
    // TCanvas *canvases[2];
    // canvases[0] = new TCanvas("canvas", "ADC1 Distributions", 800, 600);
    // canvases[1] = new TCanvas("canvas", "ADC2 Distributions", 800, 600);
    // canvases[0]->Divide(4, 4);
    // canvases[1]->Divide(4, 4);
    // for (int i = 0; i < MAX_SEC; i++)
    // {
    //     // drawing ADC1 E[i] histogram
    //     canvases[0]->cd(i + 1);
    //     histograms[i][0]->Draw();
    //     // drawing ADC2 E[i] histogram
    //     canvases[1]->cd(i + 1);
    //     histograms[i][1]->Draw();
    // }

    return;
}
