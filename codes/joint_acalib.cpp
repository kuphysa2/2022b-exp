#include<iostream>
#include<fstream>
using namespace std;

int main()
{
    int ana_date = 316;
    char ofs_name[64];
    snprintf(ofs_name, 64, "../exp0000/a%04d/exp0000_acalib.dat", ana_date);

    ofstream ofs(ofs_name);
    // ifstream ifs1("../exp0216/a0313/exp0216_acalib.dat");
    ifstream ifs2("../exp0227/a0310/exp0227_acalib.dat");
    ifstream ifs3("../exp0310/a0314/exp0310_acalib.dat");

    double adc1, adc2, tdc;

    // while (!ifs1.eof())
    // {
    //     ifs1 >> adc1 >> adc2 >> tdc;
    //     ofs << adc1 << " " << adc2 << " " << tdc << endl;
    // }
    while (!ifs2.eof())
    {
        ifs2 >> adc1 >> adc2 >> tdc;
        ofs << adc1 << " " << adc2 << " " << tdc << endl;
    }
    while (!ifs3.eof())
    {
        ifs3 >> adc1 >> adc2 >> tdc;
        ofs << adc1 << " " << adc2 << " " << tdc << endl;
    }

    return 0;
}
