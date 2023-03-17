#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>

int adc_calibrat(double factor1, double ground1, double factor2, double ground2)
{
    int exp_date = 0;
    int ana_date = 317;
    char ifs_name[64], ofs_name[64];
    snprintf(ifs_name, 64, "../exp%04d/a%04d/exp%04d_tcalib.dat", exp_date, ana_date, exp_date);
    snprintf(ofs_name, 64, "../exp%04d/a%04d/exp%04d_acalib.dat", exp_date, ana_date, exp_date);
    std::ifstream ifs(ifs_name);
    std::ofstream ofs(ofs_name);

    int adc[2];
    double tdc;
    int row = 0;

    while (!ifs.eof())
    {
        // input
        ifs >> adc[0] >> adc[1] >> tdc;
        // output
        ofs << (double)adc[0] * factor1 + ground1 << " " << (double)adc[1] * factor2 + ground2 << " " << tdc << std::endl;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    // default values (cf. README.md)
    double factor1 = 0.7408;
    double ground1 = -164.47;
    double factor2 = 0.755;
    double ground2 = -134.09;

    // changing values in case of input
    if (argc > 1)
        factor1 = atof(argv[1]);
    if (argc > 2)
        ground1 = atof(argv[2]);
    if (argc > 3)
        factor2 = atof(argv[3]);
    if (argc > 4)
        ground2 = atof(argv[4]);

    adc_calibrat(factor1, ground1, factor2, ground2);

    return 0;
}
