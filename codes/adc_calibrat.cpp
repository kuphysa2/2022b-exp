#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>

int adc_calibrat(double factor1, double ground1, double factor2, double ground2)
{
    std::ifstream ifs("../exp0227/a0310/exp0227_tcalib.dat");
    std::ofstream ofs("../exp0227/a0310/exp0227_acalib.dat");

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
    double factor1 = 0.7472;
    double ground1 = -113.58;
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
