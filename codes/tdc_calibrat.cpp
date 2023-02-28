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
    int row = 0;

    while (!ifs.eof())
    {
        ifs >> adc[0] >> adc[1] >> tdc;
        ofs << adc[0] << " " << adc[1] << " " << (double)tdc * factor + ground << std::endl;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    double factor = 0.2126;
    double ground = 11.313;

    if (argc > 1)
        factor = atof(argv[1]);
    if (argc > 2)
        ground = atof(argv[2]);

    tdc_calibrat(factor, ground);

    return 0;
}
