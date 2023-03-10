#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <math.h>

int TQcor(double p0, double p1, double p2, double p3, double q0, double q1, double q2, double q3)
{
    std::ifstream ifs("../exp0227/a0310/exp0227_acalib.dat");
    std::ofstream ofs("../exp0227/a0310/exp0227_TQcor.dat");

    double a1, a2, t;

    while (!ifs.eof())
    {
        ifs >> a1 >> a2 >> t;
        if(a1 > 20)
        {
            ofs << a1 << " " << a2 << " " << t - p0 / pow((a1 - p1), p2) - p3 << std::endl;
        }
        else if(a2 > 20)
        {
            ofs << a1 << " " << a2 << " " << t - q0 / pow((a2 - q1), q2) - q3 << std::endl;
        }
    }
    return 0;
}

int main()
{
    TQcor(214.3, 123.5, 0.03451, -56.74, 236.9, 128.8, 0.02601, -84.61);
}
