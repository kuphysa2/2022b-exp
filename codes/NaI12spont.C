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

const int MAX_ROWS = 5000000;
const int MAX_COLS = 3;

void NaI12spont()
{
    int channel = 2; // 1 or 2
    int exp_date = 320;
    int ana_date = 323;
    double zMax = 20;
    double zMin = 1;
    double xMax = 1500;
    double xMin = -20;
    double yMax = 1500;
    double yMin = -20;
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
        binX = h1->GetXaxis()->FindBin(data[i][0]);
        binY = h1->GetXaxis()->FindBin(data[i][1]);
        h1->Fill(data[i][0], data[i][1]);
    }
    h1->SetMinimum(zMin);
    h1->SetMaximum(zMax);
    h1->SetOption("colz");
    h1->SetOption("logz");
    gStyle->SetOptLogz();
    char title[NAME_LEN];
    snprintf(title, NAME_LEN, "ADC1 ADC2; ADC1 energy[keV]; ADC2 energy[keV];");
    h1->SetTitle(title);
    canvases[0] = new TCanvas("c1", "c1", 600, 600);
    h1->Draw("colz");
    canvases[0]->Update();
    snprintf(out1_name, NAME_LEN, "../exp%04d/a%04d/pdf/adc1adc2.pdf", exp_date, ana_date);
    canvases[0]->Print(out1_name);
    snprintf(out1_name, NAME_LEN, "../exp%04d/a%04d/img/adc1adc2.png", exp_date, ana_date);
    canvases[0]->Print(out1_name);

    return;
}
