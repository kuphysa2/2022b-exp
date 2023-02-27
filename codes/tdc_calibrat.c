// #include<fstream>
// #include<iostream>
// #include<cstdlib>
// #include<string>
#include<stdio.h>

#define MAX_ROWS 1000000
#define MAX_COLS 3

int tdc_calibrat()
{
    FILE *input = fopen("../exp0227/a0227/exp0227_halved.dat", "r");
    FILE *output = fopen("../exp0227/a0227/exp0227_tcalib.dat", "w");

    int tdc, adc[] = {0, 0};
    int row = 0;

    for ( row = 0; row < MAX_ROWS; row++)
    {
        if (fscanf(input, "%d %d %d", &adc[0], &adc[1], &tdc) == 3)
        {
            fprintf(output, "%d %d %lf\n", adc[0], adc[1], ((double)tdc - 92.033) / 4.0675);
        }
        else
            break;
    }

    return 0;
}
