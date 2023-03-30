#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>
#define NAME_LEN 64

int adc_calibrat(double factor1, double ground1, double factor2, double ground2)
{
    int exp_date = 320;
    int ana_date = 330;
    char ifs_name[NAME_LEN], ofs_name[NAME_LEN];
    snprintf(ifs_name, NAME_LEN, "../exp%04d/a%04d/exp%04d_tcalib.dat", exp_date, ana_date, exp_date);
    snprintf(ofs_name, NAME_LEN, "../exp%04d/a%04d/exp%04d_acalib.dat", exp_date, ana_date, exp_date);
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
    double factor1 = 0.73742;
    double ground1 = -112.825;
    double factor2 = 0.761194;
    double ground2 = -137.776;

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
