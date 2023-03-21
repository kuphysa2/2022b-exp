#include <iostream>
#include <fstream>
using namespace std;
#define NAME_LEN 64

int main()
{
    int ana_date = 320;
    char ofs_name[NAME_LEN];
    snprintf(ofs_name, NAME_LEN, "../exp0000/a%04d/exp0000.dat", ana_date);

    ofstream ofs(ofs_name);
    ifstream ifs2("../exp0227/a0310/exp0227.dat");
    ifstream ifs3("../exp0310/a0320/exp0310.dat");

    double adc1, adc2, tdc;
    int entry = 0;

    while (!ifs2.eof())
    {
        ifs2 >> adc1 >> adc2 >> tdc;
        ofs << adc1 << " " << adc2 << " " << tdc << endl;
        entry++;
    }
    while (!ifs3.eof())
    {
        ifs3 >> adc1 >> adc2 >> tdc;
        ofs << adc1 << " " << adc2 << " " << tdc << endl;
        entry++;
    }

    cout << entry << endl;

    return 0;
}
