#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#define ROWS_NUM 7
#define COLS_NUM 10
int main() {
    bool flag = false;  // x was found
    int matrix[ROWS_NUM][COLS_NUM] = {
            {-11, 12, 20, 20, 21, 21, 22, 25, 30, 30},
            {-23, -23, -19, -18, -18, -18, -15, -13, -12, -12},
            {-2, -1, -1, -1, 0, 0, 0, 0, 1, 1},
            {2, 3, 4, 4, 7, 8, 8, 9, 9, 9},
            {52, 52, 53, 55, 66, 66, 69, 78, 100, 10101},
            {34, 35, 39, 40, 40, 40, 40, 40, 40, 44},
            {-10, -9, -9, -8, -5, -5, -5, -5, -5, -5}
    };
    int left_limit;
    int right_limit;
    int middle;
    int x;
    scanf("%d", &x);

    printf("Matrix is:\n");
    for (int i = 0; i < ROWS_NUM; i++) {
        for (int j = 0; j < COLS_NUM; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    };
    for (int row = 0; row < ROWS_NUM && !flag; row++) {
        left_limit = 0;
        right_limit = COLS_NUM - 1;
        middle = floor((left_limit + right_limit) / 2.0);
        while (left_limit < right_limit) {
            if (matrix[row][middle] < x) left_limit = middle + 1;
            else right_limit = middle;
            middle = floor((left_limit + right_limit) / 2.0);
        };
        if (matrix[row][middle] == x) {
            printf("X was found at [%d][%d]\n", row, middle);
            flag = true;
        }
    };
    if (!flag) printf("X doesn't exist in this matrix\n");
    return 0;
};
