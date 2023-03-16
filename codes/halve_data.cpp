#include<fstream>
#include<iostream>
#include<string>
#include<cstdlib>

int main()
{
    int exp_date = 310;
    int ana_date = 316;
    char ifs_name[64], ofs_name[64];
    snprintf(ifs_name, 64, "../exp%04d/a%04d/exp%04d.dat", exp_date, ana_date, exp_date);
    snprintf(ofs_name, 64, "../exp%04d/a%04d/exp%04d_halved.dat", exp_date, ana_date, exp_date);
    std::ifstream ifs(ifs_name);
    std::ofstream ofs(ofs_name);

    int row, col, num_rows;
    int tdc, adc1, adc2;
    int ptdc, padc1, padc2; // previous values
    int flag;   // 0: doubled, 1: not doubled

    // input
    num_rows = 0;
    flag = 0;
    while (ifs >> tdc >> adc1 >> adc2)
    {
        num_rows++;
        if (ptdc == tdc && padc1 == adc1 && padc2 == adc2)
        {
            flag = 0;
            continue;
        }

        // output
        ofs << tdc << " " << adc1 << " " << adc2 << std::endl;

        // in case not doubled
        if (flag == 1)
            std::cout << "not doubled no. " << num_rows << std::endl;
        flag = 1;

        // update
        ptdc = tdc;
        padc1 = adc1;
        padc2 = adc2;
    }
    ifs.close();
    ofs.close();

    return 0;
}
