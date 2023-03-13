#include<iostream>
#include<fstream>
using namespace std;
int main()
{
    int ana_date = 313;
    char ofs_name[64];
    snprintf(ofs_name, 64, "../exp0216/a%04d/exp0216.dat", ana_date);
    ofstream ofs(ofs_name);
    ifstream ifs("../exp0216/a0227/exp0216.dat");

    int adc1, adc2, adc3, tdc;
    while (!ifs.eof())
    {
        ifs >> adc1 >> adc2 >> adc3 >> tdc;
        ofs << adc1 << " " << adc2 << " " << tdc << endl;
    }
    return 0;
}
