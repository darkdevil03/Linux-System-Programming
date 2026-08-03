#include <stdio.h>

void changeInt(int *ptrAlter) {
    *ptrAlter = *ptrAlter * *ptrAlter;
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