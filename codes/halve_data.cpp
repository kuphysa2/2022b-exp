#include<fstream>
#include<iostream>
#include<string>

int main()
{
    std::ifstream ifs("../exp0227/a0310/exp0227.dat");
    std::ofstream ofs("../exp0227/a0310/exp0227_halved.dat");

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
