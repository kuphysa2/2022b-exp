#include <iostream>
#include <fstream>
#include "TH2.h"
#include "TH2F.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TColor.h"
#include "TMath.h"
#define NAME_LEN 64
#define TIME_FLOOR 200

const int MAX_ROWS = 5000000;
const int MAX_COLS = 3;

void time_energy()
{
    int channel = 1; // 1 or 2
    int exp_date = 0;
    int ana_date = 320;
    double zMax = 500;
    double zMin = 1;
    double xMax = 2000;
    double xMin = 20;
    double yMax = 1000;
    double yMin = 0;
    int nBins = 200;
    channel--;

    char ifs_name[NAME_LEN];
    snprintf(ifs_name, NAME_LEN, "../exp%04d/a%04d/exp%04d_acalib.dat", exp_date, ana_date, exp_date);
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

    int binX, binY;
    gStyle->SetOptStat(0);

    TCanvas *canvases[4];

    // making 2D histogram of ADC1-TDC
    TH2F *h1 = new TH2F("h1", "h1", nBins, xMin, xMax, nBins, yMin, yMax);
    char out1_name[NAME_LEN];
    for (int i = 0; i < row; i++)
    {
        if (data[i][2] > TIME_FLOOR)
        {
            binX = h1->GetXaxis()->FindBin(data[i][channel]);
            binY = h1->GetXaxis()->FindBin(data[i][2]);
            h1->Fill(data[i][channel], data[i][2]);
        }
    }
    h1->SetMinimum(zMin);
    h1->SetMaximum(zMax);
    h1->SetOption("colz");
    h1->SetOption("logz");
    gStyle->SetOptLogz();
    char title[NAME_LEN];
    snprintf(title, NAME_LEN, "Time-Energy ADC%d (t>%d); energy[keV]; time[ns];", channel + 1, TIME_FLOOR);
    h1->SetTitle(title);
    canvases[0] = new TCanvas("c1", "c1", 600, 600);
    h1->Draw("colz");
    canvases[0]->Update();
    snprintf(out1_name, NAME_LEN, "../exp%04d/a%04d/pdf/t_cut/time_energy%d_Tover%d.pdf", exp_date, ana_date, channel + 1, TIME_FLOOR);
    canvases[0]->Print(out1_name);

    return;
}
