#include <stdio.h>

int main() {
    float x;
    float y;
    printf("Please, enter x:\n");
    scanf("%f", &x);
    if(x < 23){                                          // x in (-inf; 23)
        if(x >= 8){                                      // x in [8; 23)
            y = -5 * (x * x * x) + 10;
            printf("y = %f", y);
        }
        else{                                            // x in (-inf; 8)
            if(x > -3){                                  // x in (-3; 8)
                if(x <= 0){                              // x in (-3; 0]
                    y = 2 * (x * x * x) + 8 * (x * x);
                    printf("y = %f", y);
                }
                else{                                    // x in (0; 8)
                    printf("Function doesn't exist for the entered x");
                }
            }
            else{                                        // x in (-inf; -3]
                if(x < -19){                             // x in (-inf; -19)
                    y = 2 * (x * x * x) + 8 * (x * x);
                    printf("y = %f", y);
                }
                else{                                    // x in [-19; -3]
                    printf("Function doesn't exist for the entered x");
                }
            }
        }
    }
    else{                                                // x in [23; inf)
        printf("Function doesn't exist for the entered x");
    }
    return 0;
}
