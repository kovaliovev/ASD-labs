#include <stdio.h>
#include <math.h>

int main() {
    int counter = 0;
    int n;
    int j = 1;
    double denominator = 0;
    double result = 1;
    scanf("%d", &n);
    counter += 4; // присвоєння j, присвоєння denominator, присвоєння result, присвоєння n через scanf
    counter += 1; // присвоєння i
    for(int i = 1; i <= n; i++, j++){
        counter += 4; // порівняння i та n, інкремент i, інкремент j, наступна ітерація
        denominator += sin(j) + 1;
        counter += 3; // сінус, додавання, присвоєння
        result *= (i * (i + 1)) / denominator;
        counter += 4; // додавання, множення, ділення, присвоєння counter
    }
    printf("%.7lf\n", result);
    printf("Number of operations: %d", counter);
    return 0;
}
