#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>

// #define MAX_ROWS 1000000
// #define MAX_COLS 3

int adc_calibrat(double factor1, double ground1, double factor2, double ground2)
{
    std::ifstream ifs("../exp0227/a0228/exp0227_tcalib.dat");
    std::ofstream ofs("../exp0227/a0228/exp0227_acalib.dat");

    int adc[2];
    double tdc;
    int row = 0;

    while (!ifs.eof())
    {
        ifs >> adc[0] >> adc[1] >> tdc;
        ofs << (double)adc[0] * factor1 + ground1 << " " << (double)adc[1] * factor2 + ground2 << " " << tdc << std::endl;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    double factor1 = 0.7355;
    double ground1 = -245.15;
    double factor2 = 0.6937;
    double ground2 = -162.87;

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
