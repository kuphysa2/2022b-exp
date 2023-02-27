#include<stdio.h>

#define MAX_ROWS 200000
#define MAX_COLS 4

int main()
{
    FILE *in_file = fopen("../exp0216/a0227/exp0216.dat", "r");
    FILE *out_file = fopen("../exp0216/a0227/exp0216_halved.dat", "w");
    int data[MAX_ROWS][MAX_COLS] = {};
    int row, col, num_rows;

    // input
    num_rows = 0;
    for (row = 0; row < MAX_ROWS; row++)
    {
        if (fscanf(in_file, "%d %d %d %d", &data[row][0], &data[row][1], &data[row][2], &data[row][3]) == 4)
            num_rows++;
        else
            break;
    }

    // output
    for (row = 0; row < num_rows; row+=2)
    {
        for (col = 0; col < MAX_COLS; col++)
            fprintf(out_file, "%d ", data[row][col]);
        fprintf(out_file, "\n");
    }

    fclose(in_file);
    fclose(out_file);

    return 0;
}
