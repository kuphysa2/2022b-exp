#include <iostream>
#include <fstream>
#include "TH2.h"
#include "TH2F.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TColor.h"
#include "TMath.h"

const int MAX_ROWS = 1000000;
const int MAX_COLS = 3;

void time_energy()
{
    int channel = 1;    // 0 or 1
    channel--;
    int exp_date = 310;
    int ana_date = 313;
    char ifs_name[64];
    snprintf(ifs_name, 64, "../exp%04d/a%04d/exp%04d_acalib.dat", exp_date, ana_date, exp_date);
    Double_t data[MAX_ROWS][MAX_COLS];
    std::ifstream file(ifs_name);
    int row = 0;

    // input
    while (file >> data[row][0] >> data[row][1] >> data[row][2])
    {
        row++;
        if (row >= MAX_ROWS)
        {
            std::cerr << "Error: too many rows in data file." << std::endl;
            return 1;
        }
    }
    file.close();

    int nBins = 200;
    double xMin = -100;
    double xMax = 2000;
    double yMin = 0;
    double yMax = 1000;
    double zMin = 1;
    double zMax = 1e3;
    int binX, binY;
    gStyle->SetOptStat(0);

    TCanvas *canvases[4];

    // making 2D histogram of ADC1-TDC
    TH2F *h1 = new TH2F("h1", "h1", nBins, xMin, xMax, nBins, yMin, yMax);
    char out1_name[64];
    snprintf(out1_name, 64, "../exp%04d/a%04d/time_energy%d.pdf", exp_date, ana_date, channel + 1);
    for (int i = 0; i < row; i++)
    {
        binX = h1->GetXaxis()->FindBin(data[i][channel]);
        binY = h1->GetXaxis()->FindBin(data[i][2]);
        h1->Fill(data[i][channel], data[i][2]);
    }
    h1->SetMinimum(zMin);
    h1->SetMaximum(zMax);
    h1->SetOption("colz");
    h1->SetOption("logz");
    char title[64];
    snprintf(title, 64, "Time-Energy ADC%d; energy[keV]; time[ns];", channel);
    h1->SetTitle(title);
    canvases[0] = new TCanvas("c1", "c1", 600, 600);
    h1->Draw("colz");
    canvases[0]->Update();
    canvases[0]->Print(out1_name);

    // making 2D histogram of ADC2-TDC
    // TH2F *h2 = new TH2F("h2", "h2", nBins, xMin, xMax, nBins, yMin, yMax);
    // for (int i=0; i < row; i += 2)
    // {
    //     binX = h1->GetXaxis()->FindBin(data[i][1]);
    //     binY = h1->GetXaxis()->FindBin(data[i][2]);
    //     h1->Fill(data[i][1], data[i][2]);
    // }
    // h2->SetMaximum(zMax);
    // h2->SetMinimum(zMin);
    // h2->SetOption("colz");
    // h2->SetOption("logz");
    // h2->SetTitle("Time-Energy ADC2; energy [keV]; time [ns];");
    // canvases[1] = new TCanvas("c2", "c2", 600, 600);
    // canvases[1]->Update();
    // canvases[1]->Print("../exp0227/a0228/time_energy2.pdf");
    // h2->Draw("colz");

    return;
}
