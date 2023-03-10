#include<stdio.h>
#include<fstream>

#define MAX_ROWS 700000
#define MAX_COLS 3

int main()
{
    std::ifstream ifs("../exp0227/a0310/exp0227.dat");
    std::ofstream ofs("../exp0227/a0310/exp0227_halved.dat");

    // FILE *in_file = fopen("../exp0227/a0310/exp0227.dat", "r");
    // FILE *out_file = fopen("../exp0227/a0310/exp0227_halved.dat", "w");
    // int data[MAX_ROWS][MAX_COLS] = {};
    int row, col, num_rows;
    int tdc, adc1, adc2;
    int ptdc, padc1, padc2;

    // input
    num_rows = 0;
    while (ifs >> tdc >> adc1 >> adc2)
    {
        num_rows++;
        if (ptdc == tdc && padc1 == adc1 && padc2 == adc2)
            continue;
        ofs << tdc << " " << adc1 << " " << adc2 << std::endl;
        ptdc = tdc;
        padc1 = adc1;
        padc2 = adc2;
    }
    ifs.close();
    ofs.close();

    // // input
    // num_rows = 0;
    // for (row = 0; row < MAX_ROWS; row++)
    // {
    //     if (fscanf(in_file, "%d %d %d", &data[row][0], &data[row][1], &data[row][2]) == 3)
    //         num_rows++;
    //     else
    //         break;
    // }

    // // output
    // int flag;   // for judging the row is same with the previous or not
    // int prev_flagged_row = 0;
    // for (row = 0; row < num_rows - 1; row++)
    // {
    //     flag = 0;
    //     for (col = 0; col < MAX_COLS; col++)
    //     {
    //         if (data[row][col] != data[row + 1][col])
    //         {
    //             flag = 1;
    //         }
    //     }
    //     if (flag == 1)  // when the row is not the previous one
    //     {
    //         if (row == prev_flagged_row + 1)
    //             printf("not doubled # %d\n", row);
    //         for (col = 0; col < MAX_COLS; col++)
    //         {
    //             fprintf(out_file, "%d ", data[row][col]);
    //         }
    //         fprintf(out_file, "\n");
    //         prev_flagged_row = row;
    //     }
    // }

    // fclose(in_file);
    // fclose(out_file);

    return 0;
}
