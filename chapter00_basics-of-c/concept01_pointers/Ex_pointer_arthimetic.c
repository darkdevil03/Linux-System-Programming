/**
    Pointer Arithmetic
        We can add or subtract from a pointer. Adding 1 to an int pointer doesn't add 1 byte;
        it moves the pointer forward by the size of an int (usually 4 bytes).
 */

#include <stdio.h>

int main() {

    int arr[] = {10, 20, 30};
    int *ptr = arr; // Points to the first element (10)

    printf("%d\n", *ptr);       // Prints 10
    ptr++;                      // Moves to the next integer in memory
    printf("%d\n", *ptr);       // Prints 20
    *ptr++;                     // *ptr++ also moves to next integer in memory; here * treats has unused.
    printf("%d\n", *ptr);

    return 0;
}