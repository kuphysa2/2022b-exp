#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>

// #define MAX_ROWS 1000000
// #define MAX_COLS 3

int tdc_calibrat(double factor = 0.246, double ground = -22.618)
{
    int exp_date = 310;
    int ana_date = 317;
    char ifs_name[64], ofs_name[64];
    snprintf(ifs_name, 64, "../exp%04d/a%04d/exp%04d_halved.dat", exp_date, ana_date, exp_date);
    snprintf(ofs_name, 64, "../exp%04d/a%04d/exp%04d_tcalib.dat", exp_date, ana_date, exp_date);
    std::ifstream ifs(ifs_name);
    std::ofstream ofs(ofs_name);

    int tdc, adc[2];
    double origin = 850;    // delay on TDC is 850 ns
    int row = 0;

    while (!ifs.eof())
    {
        // input
        ifs >> adc[0] >> adc[1] >> tdc;
        // output
        ofs << adc[0] << " " << adc[1] << " " << origin - ((double)tdc * factor + ground) << std::endl;
    }

        return 0;
}

int main(int argc, char *argv[])
{
    // default values (cf. README.md)
    double factor = 0.246;
    double ground = -22.618;

    // changing values in case of input
    if (argc > 1)
        factor = atof(argv[1]);
    if (argc > 2)
        ground = atof(argv[2]);

    tdc_calibrat(factor, ground);

    return 0;
}
