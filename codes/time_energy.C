#include <iostream>
#include <fstream>
#include "TH2.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TColor.h"
#include "TMath.h"

const int MAX_ROWS = 100000;
const int MAX_COLS = 4;

int main()
{
    // データを読み込む
    Double_t data[MAX_ROWS][MAX_COLS];
    std::ifstream file("../exp0216/a0220/exp0216_halved.dat");
    int row = 0;
    while (file >> data[row][0] >> data[row][1] >> data[row][2] >> data[row][3])
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
    double xMin = 600;
    double xMax = 800;
    double yMin = 0;
    double yMax = 2400;
    double zMin = 1;
    double zMax = 1e6;
    gStyle->SetOptStat(0);

    // hist[0], hist[1], hist[2] それぞれのカラープロットを作成する
    for (int k = 0; k < 3; k++)
    {
        TH2F *hist = new TH2F(Form("hist%d", k), Form("hist%d", k), nBins, xMin, xMax, nBins, yMin, yMax);
        for (int i = 0; i < row; i += 2)
        {
            int binX = hist->GetXaxis()->FindBin(data[i][k]);
            int binY = hist->GetYaxis()->FindBin(data[i][3]);
            hist->Fill(data[i][k], data[i][3]);
        }
        hist->SetMinimum(zMin);
        hist->SetMaximum(zMax);
        hist->SetStats(false);
        hist->GetXaxis()->SetTitle(Form("Column %d", k));
        hist->GetYaxis()->SetTitle("Column 3");
        hist->GetZaxis()->SetTitle("Events");
        hist->GetZaxis()->SetLabelSize(0.025);
        hist->GetZaxis()->SetLabelOffset(0.005);
        hist->GetZaxis()->SetLabelFont(42);
        hist->GetZaxis()->SetTitleSize(0.03);
        hist->GetZaxis()->SetTitleOffset(1.5);
        hist->GetZaxis()->SetTitleFont(42);
        hist->SetOption("colz");
        hist->SetContour(100);
        hist->SetLogz();
        TCanvas *canvas = new TCanvas(Form("canvas%d", k), Form("canvas%d", k), 800, 600);
        canvas->SetRightMargin(0.15);
        hist->Draw("colz");
    }

    return 0;
}
