/**
    Scenario:
        Pass by Reference in Functions
            By default, C uses pass-by-value (functions get copies of variables). By passing a pointer, a function can modify the original variable.
 */

#include <stdio.h>

void changeInt(int *ptrAlter) {
    *ptrAlter = *ptrAlter * *ptrAlter; // Modifies original variable in main() for square of input.
}

int main() {
    int inputNumber;
    printf("Enter the input number for square of it : ");
    scanf("%d",&inputNumber);

    int printValue = inputNumber;

    changeInt(&inputNumber);

    printf("Square of input number %d is %d",printValue, inputNumber);

    return 0;
}