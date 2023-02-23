#include <stdio.h>
#define N 9

int main() {
    int matrix[N][N] = {
            { 23, 41, 25, 11, 89, 90, 13, 43, 66 },
            { 12, 44, 56, 85, 99, 21, 49, 34, 34 },
            { 10, 77, 41, 42, 62, 83, 64, 69, 90 },
            { 53, 75, 19, 36, 98, 40, 90, 32, 11 },
            { 88, 21, 72, 93, 11, 66, 28, 20, 94 },
            { 41, 71, 99, 83, 81, 70, 29, 91, 90 },
            { 14, 83, 12, 70, 30, 27, 79, 10, 39 },
            { 53, 65, 29, 52, 62, 33, 18, 20, 25 },
            { 34, 59, 26, 95, 72, 79, 20, 30, 44 },
    };
    int flag = 1;
    int current;
    int next;
    int tmp;
    printf("Matrix before sorting:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    while (flag) {
        flag = 0;
        for (int row = N - 1, col = 0; row > 0; row--, col++) {
            current = matrix[row][col];
            next = matrix[row - 1][col + 1];
            tmp = current;
            if (next > current) {
                matrix[row][col] = next;
                matrix[row - 1][col + 1] = tmp;
                flag = 1;
            }
        }
    }

    printf("Matrix after sorting:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    return 0;
}
