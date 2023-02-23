#include <stdio.h>

// обчислення на рекурсивному підйомі

double sum = 0;

double seriesSum(double x, int n) {
    double number;
    if (n == 1) {
        number = x;
    }
    else if (n == 2) {
        sum += x;
        number = (x*x)/2.0;
    }
    else {
        number = -seriesSum(x, n-1) * x * (n-1) * (n-2)/(n*n-n);
    }

    sum += number;
    return number;
}

int main() {
    double x;
    int n;
    scanf("%lf%d", &x, &n);
    seriesSum(x, n);
    double ln = sum / (1+x);
    printf("x: %.2lf\nn: %d\nsum: %lf\nln(%.2lf) = %lf", x, n, sum, x+1, ln);
    return 0;
}

