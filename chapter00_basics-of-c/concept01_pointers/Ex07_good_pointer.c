#include <stdio.h>
#include <stdlib.h>

static void goodAllocate(int **dPtr) {
    *dPtr = malloc(sizeof(int));
    printf("\nCreated new heap and allocated values to it....\n");
    **dPtr = 20;
}

int main() {

    int *mainPtr = nullptr;
    printf("\nBefore good allocate function called:\n");
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
        printf("\nAfter good allocate function called:\n");
        printf("mainPtr\n");
        printf("   |------> address : %p\n", &mainPtr);
        printf("   |------> value hold (refers to address holding) : %p\n", mainPtr);
        printf("   |------> value of address holding (refers to dereference): %d\n", *mainPtr);

    }

    return 0;
}