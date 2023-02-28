#include <iostream>
#include <fstream>
#include "TH2.h"
#include "TH2F.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TColor.h"
#include "TMath.h"

#define trial 16
#define MAX_ROW 100000

void TQcorrection()
{
    Double_t tdc[MAX_ROW];
    Double_t adc[MAX_ROW][2];
    int counter = 0;
    int row;
    int flag;
    double dE = 20;
    double E[trial] = {}, dt[2][trial] = {};
    // initialize E
    for (int i = 0; i < trial; i++)
    {
        E[i] = 150 + i * dE;
    }

    // make histograms
    TH1F *histograms[trial][2];
    for (int i = 0; i < trial; i++)
    {
        histograms[i][0] = new TH1F(Form("histogram%d", i), Form("ADC1 Distribution (E=%f)", E[i]), 100, E[i] - 15, E[i] + 15);
        histograms[i][1] = new TH1F(Form("histogram%d", i), Form("ADC2 Distribution (E=%f)", E[i]), 100, E[i] - 15, E[i] + 15);
    }

    // input data
    row = 0;
    counter = 0;
    std::ifstream ifs("../exp0227/a0228/exp0227_acalib.dat");
    while (!ifs.eof())
    {
        ifs >> adc[row][0] >> adc[row][1] >> tdc[row];
        row++;
        flag = 0;

        for (int i = 0; i < trial; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                if (adc[row][j] > E[i] - 5 && adc[row][j] < E[i] + 5)
                {
                    histograms[i][j]->Fill(tdc[i]);
                    flag++;
                }
            }
            if (flag == 2)
                break;
        }

        if (row >= MAX_ROW)
        {
            std::cerr << "Error: too many rows in data file." << std::endl;
            return 1;
        }
    }
    ifs.close();

    TCanvas *c1 = new TCanvas("canvas", "test", 800, 600);
    histograms[0][1]->Draw();

    std::cout << row << std::endl;

    // output
    // TCanvas *canvases[2];
    // canvases[0] = new TCanvas("canvas", "ADC1 Distributions", 800, 600);
    // canvases[1] = new TCanvas("canvas", "ADC2 Distributions", 800, 600);
    // canvases[0]->Divide(4, 4);
    // canvases[1]->Divide(4, 4);
    // for (int i = 0; i < trial; i++)
    // {
    //     canvases[0]->cd(i + 1);
    //     histograms[i][0]->Draw();
    //     canvases[1]->cd(i + 1);
    //     histograms[i][1]->Draw();
    // }

    return;
}
