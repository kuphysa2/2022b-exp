#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>

void TQcor(double a0, double a1, double a2, double a3, double b0, double b1, double b2, double b3)
{
    std::ifstream ifs("../exp0227/a0303/exp0227_acalib.dat");
    std::ofstream("../exp0227/a0303/exp0227_TQcor.dat");

    double a1,a2,t;

    while (!ifs.eof())
    {
        ifs >> a1 >> a2 >> t;
        if(a1 > 20)
        {
            ofs << 
        }
    }
}
