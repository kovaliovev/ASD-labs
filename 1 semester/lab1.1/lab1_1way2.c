#include <stdio.h>

int main() {
    float x;
    float y;
    printf("Please, enter x:\n");
    scanf("%f", &x);

    if(x >= 8 && x < 23){                                // x in [8; 23)
        y = -5 * (x * x * x) + 10;
        printf("y = %f", y);
    }
    else{
        if(x < -19 || x > -3 && x <= 0){   // x in (-inf; -19) or (-3; 0]
            y = 2 * (x * x * x) + 8 * (x * x);
            printf("y = %f", y);
        }
        else{                    // x in [-19; -3] or (0; 8) or [23; inf)
            printf("Function doesn't exist for the entered x");
        }
    }
    return 0;
}
