#include <fstream>
#include <iostream>
#include <TF1.h>
#include <TFile.h>
#include <TH1D.h>
#include <TROOT.h>
#include <TStyle.h>
#define NAME_LEN 64

void draw_tdcadc_raw()
{
    int exp_date = 320;
    int ana_date = 403;
    int adc_range[] = {0, 4100};
    int tdc_range[] = {0, 4096};
    int adc_nBins = 1000;
    int tdc_nBins = 1000;
    int adc1_height = 1000;
    int adc2_height = 1000;
    int adc_zoom_range[] = {0, 2500};

    TH1S *htdc = new TH1S("h1", "h1", tdc_nBins, tdc_range[0], tdc_range[1]);
    TH1S *hadc0 = new TH1S("h1", "h1", adc_nBins, adc_range[0], adc_range[1]);
    TH1S *hadc1 = new TH1S("h1", "h1", adc_nBins, adc_range[0], adc_range[1]);
    TH1S *hadc0z = new TH1S("h1", "h1", adc_nBins, adc_zoom_range[0], adc_zoom_range[1]);
    TH1S *hadc1z = new TH1S("h1", "h1", adc_nBins, adc_zoom_range[0], adc_zoom_range[1]);

    char ifs_name[NAME_LEN];
    snprintf(ifs_name, NAME_LEN, "../exp%04d/a%04d/exp%04d.dat", exp_date, ana_date, exp_date);
    ifstream data(ifs_name);
    double tdc, adc[] = {0, 0};

    // input
    while (!data.eof())
    {
        data >> adc[0] >> adc[1] >> tdc;
        htdc->Fill(tdc);
        hadc0->Fill(adc[0]);
        hadc1->Fill(adc[1]);
        hadc0z->Fill(adc[0]);
        hadc1z->Fill(adc[1]);
    }

    // graph titles
    htdc->SetTitle("TDC; time [TDC]; count;");
    hadc0->SetTitle("ADC1 raw; energy [ADC]; count;");
    hadc1->SetTitle("ADC2 raw; energy [ADC]; count;");
    hadc0z->SetTitle("ADC1 raw; energy [ADC]; count;");
    hadc1z->SetTitle("ADC2 raw; energy [ADC]; count;");

    TCanvas *canvases[5];
    // drawing TDC histogram
    canvases[0] = new TCanvas(TString::Format("canvas%d", 0), "", 500, 500);
    gPad->SetLogy(1);
    htdc->Draw();
    canvases[0]->Update();
    char tdc_ofs_name[NAME_LEN];
    snprintf(tdc_ofs_name, NAME_LEN, "../exp%04d/a%04d/pdf/tdc_raw.pdf", exp_date, ana_date);
    canvases[0]->Print(tdc_ofs_name);
    snprintf(tdc_ofs_name, NAME_LEN, "../exp%04d/a%04d/img/tdc_raw.png", exp_date, ana_date);
    canvases[0]->Print(tdc_ofs_name);

    // drawing ADC1 histogram
    canvases[1] = new TCanvas(TString::Format("canvas%d", 1), "", 500, 500);
    hadc0->Draw();
    canvases[1]->Update();
    char adc1_ofs_name[NAME_LEN];
    snprintf(adc1_ofs_name, NAME_LEN, "../exp%04d/a%04d/pdf/adc1_raw.pdf", exp_date, ana_date);
    canvases[1]->Print(adc1_ofs_name);
    snprintf(adc1_ofs_name, NAME_LEN, "../exp%04d/a%04d/img/adc1_raw.png", exp_date, ana_date);
    canvases[1]->Print(adc1_ofs_name);
    canvases[3] = new TCanvas(TString::Format("canvas%d", 3), "", 500, 500);
    hadc0z->SetMaximum(adc1_height);
    hadc0z->Draw();
    canvases[3]->Print(Form("../exp%04d/a%04d/pdf/adc1_raw_zoom.pdf", exp_date, ana_date));
    canvases[3]->Print(Form("../exp%04d/a%04d/img/adc1_raw_zoom.png", exp_date, ana_date));

    // drawing ADC2 histogram
    canvases[2] = new TCanvas(TString::Format("canvas%d", 2), "", 500, 500);
    hadc1->Draw();
    canvases[2]->Update();
    char adc2_ofs_name[NAME_LEN];
    snprintf(adc2_ofs_name, NAME_LEN, "../exp%04d/a%04d/pdf/adc2_raw.pdf", exp_date, ana_date);
    canvases[2]->Print(adc2_ofs_name);
    snprintf(adc2_ofs_name, NAME_LEN, "../exp%04d/a%04d/img/adc2_raw.png", exp_date, ana_date);
    canvases[2]->Print(adc2_ofs_name);
    canvases[4] = new TCanvas(TString::Format("canvas%d", 4), "", 500, 500);
    hadc1z->SetMaximum(adc1_height);
    hadc1z->Draw();
    canvases[4]->Print(Form("../exp%04d/a%04d/pdf/adc2_raw_zoom.pdf", exp_date, ana_date));
    canvases[4]->Print(Form("../exp%04d/a%04d/img/adc2_raw_zoom.png", exp_date, ana_date));
}
