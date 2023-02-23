#include <stdio.h>

// обчислення за допомогою циклу

double seriesSum(double x, int n) {
    double number;
    double result = x;
    if(n > 1) {
        number = (x*x)/2.0;
        result += number;
        for (int i = 3; i <= n; i++) {
            number *= -1.0 * x * (i-1) * (i-2)/(i*i-i);
            result += number;
        }
    }
    return result;
}

int main() {
    double x;
    int n;
    scanf("%lf%d", &x, &n);
    double sum = seriesSum(x, n);
    double ln = sum / (1+x);
    printf("x: %.2lf\nn: %d\nsum: %lf\nln(%.2lf) = %lf", x, n, sum, x+1, ln);
    return 0;
}
