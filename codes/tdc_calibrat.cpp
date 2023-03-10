#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>

// #define MAX_ROWS 1000000
// #define MAX_COLS 3

int tdc_calibrat(double factor = 0.2126, double ground = -6.4172)
{
    std::ifstream ifs("../exp0227/a0228/exp0227_halved.dat");
    std::ofstream ofs("../exp0227/a0228/exp0227_tcalib.dat");

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
    double factor = 0.2126;
    double ground = -6.4172;

    // changing values in case of input
    if (argc > 1)
        factor = atof(argv[1]);
    if (argc > 2)
        ground = atof(argv[2]);

    tdc_calibrat(factor, ground);

    return 0;
}
