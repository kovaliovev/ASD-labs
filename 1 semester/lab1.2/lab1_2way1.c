#include <stdio.h>
#include <math.h>

int main() {
    int counter = 0;
    int n;
    scanf("%d", &n);
    double result = 1;
    counter += 2; // присвоєння n через scanf, присвоєння result
    counter += 1; // присвоєння i
    for(int i = 1; i <= n; i++){
        counter += 3; // порівняння i та n, інкремент i, нова ітерація
        double sum = 0;
        counter += 1; // присвоєння sum
        counter += 1; // присвоєння j
        for(int j = 1; j <= i; j++){
            counter += 3; // порівняння j та i, інкремент j, нова ітерація
            sum += sin(j) + 1;
            counter += 3; // сінус, додавання, присвоєння sum
        }
        result *= (i * (i + 1)) / sum;
        counter += 4; // додавання, множення, ділення, присвоєння result
    };
    printf("%.7lf\n", result);
    printf("Number of operations: %d", counter);
    return 0;
}
