#include <stdio.h>

int main() {

    int age = 18;

    int *singlePointer = &age;

    int **doublePointer = &singlePointer;

    printf("Level 0 (Variable)              Level 1 (Single Pointer)            Level 2 (Double Pointer)\n");
    printf("+---------------+               +-------------------+               +-------------------+\n");
    printf("|  int x = %d;  | <------------ |   int *ptr = &x;  | <------------ | int **dptr = &ptr;|\n", age);
    printf("+---------------+               +-------------------+               +-------------------+\n");
    printf("Address: %p         Address: %p             Address: %p\n",&age, &singlePointer, &doublePointer);
    printf("Value: %d                       Value: %p               Value: %p\n",*singlePointer, *doublePointer, doublePointer);

    
    return 0;
}