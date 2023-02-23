#include <stdio.h>

// обчислення на рекурсивному спуску

double seriesSum(double x, int i, int n, double last_result, double sum) {
    double result;
    if (i == 1) result = x;
    else if (i == 2) result = (x*x)/2.0;
    else result = -last_result * x * (i-1) * (i-2) / (i*i-i);

    if (i == n) sum += result;
    else {
        sum += result;
        sum = seriesSum(x, i+1, n, result, sum);
    }

    return sum;
}

int main() {
    double x;
    int n;
    scanf("%lf%d", &x, &n);
    double sum = seriesSum(x, 1, n, 0, 0);
    double ln = sum / (1+x);
    printf("x: %.2lf\nn: %d\nsum: %lf\nln(%.2lf) = %lf", x, n, sum, x+1, ln);
    return 0;
}
