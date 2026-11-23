#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("--------- Implemented if-else statement --------- \n");

    printf("Enter any two numbers: ");
    int num1, num2;
    scanf("%d%d", &num1, &num2);

    if (num1 > num2){
        printf("%d is greater than %d\n ",num1, num2);
    }else{
        printf("%d is greater than %d\n ",num2, num1);
    }

    return EXIT_SUCCESS;
}