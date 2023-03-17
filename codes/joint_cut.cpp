#include<iostream>
#include<fstream>
using namespace std;

int main()
{
    int ana_date = 317;
    char ofs_name[64];
    snprintf(ofs_name, 64, "../exp0000/a%04d/cut/exp0000_cut(<2845).dat", ana_date);

    ofstream ofs(ofs_name);
    // ifstream ifs1("../exp0216/a0313/exp0216_acalib.dat");
    ifstream ifs2("../exp0227/a0310/exp0227.dat");
    ifstream ifs3("../exp0310/a0317/exp0310.dat");

    double adc1, adc2, tdc;

    // while (!ifs1.eof())
    // {
    //     ifs1 >> adc1 >> adc2 >> tdc;
    //     ofs << adc1 << " " << adc2 << " " << tdc << endl;
    // }

    while (!ifs2.eof())
    {
        ifs2 >> adc1 >> adc2 >> tdc;
        if (tdc < 2845.528)
        {
            ofs << adc1 << " " << adc2 << " " << tdc << endl;
        }
    }
    while (!ifs3.eof())
    {
        ifs3 >> adc1 >> adc2 >> tdc;
        if (tdc < 2845.528)
        {
            ofs << adc1 << " " << adc2 << " " << tdc << endl;
        }
    }

    return 0;
}
