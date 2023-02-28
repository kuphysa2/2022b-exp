#include<stdio.h>

#define MAX_ROWS 500000
#define MAX_COLS 3

int main()
{
    FILE *in_file = fopen("../exp0227/a0228/exp0227.dat", "r");
    FILE *out_file = fopen("../exp0227/a0228/exp0227_halved.dat", "w");
    int data[MAX_ROWS][MAX_COLS] = {};
    int row, col, num_rows;

    // input
    num_rows = 0;
    for (row = 0; row < MAX_ROWS; row++)
    {
        if (fscanf(in_file, "%d %d %d", &data[row][0], &data[row][1], &data[row][2]) == 3)
            num_rows++;
        else
            break;
    }

    // output
    int flag;
    int prev_flagged_row = 0;
    for (row = 0; row < num_rows - 1; row++)
    {
        flag = 0;
        for (col = 0; col < MAX_COLS; col++)
        {
            if (data[row][col] != data[row + 1][col])
            {
                flag = 1;
            }
        }
        if (flag == 1)
        {
            if (row == prev_flagged_row + 1)
                printf("not doubled # %d\n", row);
            for (col = 0; col < MAX_COLS; col++)
            {
                fprintf(out_file, "%d ", data[row][col]);
            }
            fprintf(out_file, "\n");
            prev_flagged_row = row;
        }
    }

    fclose(in_file);
    fclose(out_file);

    return 0;
}
