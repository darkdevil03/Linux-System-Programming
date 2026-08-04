/**
    Double Pointer (Pass-by-Reference)
        To fix bad pointer allocation of pass by value, we need to pass the address of ptr to the function.
        so the function knows exactly where ptr lives in memory and can overwrite its contents.

    The Flow:
        -> goodAllocate is called with &ptr. The local variable p now holds 0x1000 (the address of ptr).

        -> malloc allocates memory at 0x5000.

        -> *p = 0x5000. By using the * operator on p, the code goes to address 0x1000 and overwrites it with 0x5000. This successfully updates ptr in main.

        -> **p = 42. The code follows p to 0x1000, finds 0x5000 there, follows that to the heap, and writes 42.

        -> The function finishes. ptr in main now safely points to the allocated memory.
 */

#include <stdio.h>
#include <stdlib.h>

static void goodAllocate(int **dPtr) {
    *dPtr = malloc(sizeof(int));
    printf("\nCreated new heap and allocated values to it....\n");
    **dPtr = 20;
}

int main() {

    int *mainPtr = nullptr;
    printf("\nBefore goodAllocate function called:\n");
    printf("mainPtr\n");
    printf("   |------> address : %p\n", &mainPtr);
    printf("   |------> value hold (refers to address holding) : %p\n", mainPtr); // pointing to nil address of nullptr
    printf("   |------> value of address holding (refers to dereference): NULL\n"); /* if put *mainPtr we get segmentation error due to nil address doesn't have physical location in RAM,
                                                                                             to avoid that, here we just printing NULL (for our understanding!).
                                                                                            */

    goodAllocate(&mainPtr);

    if (mainPtr == nullptr) {
        printf("mainPtr is still pointing to nullptr!!");
    }else {
        printf("\nAfter goodAllocate function called:\n");
        printf("mainPtr\n");
        printf("   |------> address : %p\n", &mainPtr);
        printf("   |------> value hold (refers to address holding) : %p\n", mainPtr);
        printf("   |------> value of address holding (refers to dereference): %d\n", *mainPtr);

    }

    return 0;
}