/**
    Our code doesn't work (and will actually generate compiler warnings or errors) because of two major issues:
        a type mismatch and overwriting the local variable.

    Here is exactly what happens when we try to run the below code:

        1. The Type Mismatch
            In main, your variable ptr is an int * (a single pointer). When you use the address-of operator (&ptr),
            we are getting the address of a pointer, which makes it an int ** (a double pointer).

            However, your function is defined as void badAllocate(int *p). It is only expecting a single pointer.
            We are trying to force a double pointer into a single pointer slot. The C compiler will flag this as an incompatible pointer type.

        2. The Connection is Severed Immediately
            Let's pretend you forced the compiler to ignore the warning. Here is what happens line-by-line inside the function:

                -> badAllocate(&ptr); passes the memory address of ptr to the function. So, temporarily, p holds the location of ptr.

                -> p = malloc(sizeof(int)); happens. This is the fatal flaw. Instead of using the address of ptr to modify it,
                   we immediately overwrite the local variable p with the brand new heap address returned by malloc.

                -> The address of ptr you passed in is completely erased and forgotten.

                -> *p = 42; puts the number 42 into the new heap memory.

                -> The function ends, the heap memory is leaked, and ptr back in main was never touched, so it remains NULL.
 */

#include <stdio.h>
#include <stdlib.h>

static void badAllocate(int *p) {
    p =  malloc(sizeof(int)); // Only changes the local copy 'p'
    printf("\nCreated new heap and allocated values to it....\n");
    *p = 42;
}

int main() {
    /*
        To avoid the Null Pointer (Safety)
            Always initialize pointers. If we don't have an address to assign yet,
            set it to NULL to prevent it from pointing to a random, potentially protected memory location
            (a "wild pointer").
    */
    int *mainPtr = nullptr; // similar NULL
    printf("\nBefore badAllocate function called:\n");
    printf("mainPtr\n");
    printf("   |------> address : %p\n", &mainPtr);
    printf("   |------> value hold (refers to address holding) : %p\n", mainPtr); // pointing to nil address of nullptr
    printf("   |------> value of address holding (refers to dereference): NULL\n"); /* if put *mainPtr we get segmentation error due to nil address doesn't have physical location in RAM,
                                                                                             to avoid that, here we just printing NULL (for our understanding!).
                                                                                            */
    badAllocate(mainPtr); // Why &mainPtr not passed to hold address this pointer to function single pointer refer Ex05_double_pointer.c
    // CRASH: ptr is still NULL here because badAllocate received a copy of ptr.

    if (mainPtr == nullptr) { // similar to ptr == NULL
        printf("\nAfter badAllocate function called:\n");
        printf("mainPtr\n");
        printf("   |------> address : %p\n", &mainPtr);
        printf("   |------> value hold (refers to address holding) : %p\n", mainPtr); // pointing to nil address of nullptr
        printf("   |------> value of address holding (refers to dereference): NULL\n");
        printf("mainPtr still pointing to nil, not to heap memory created at badAllocate function!!!\n");
        printf("It defines that badAllocate function is a pass by value approach!!!!\n");
    }else {
        printf("value of ptr: %d\n", *mainPtr); // Only dereference if it's safe!
    }

    return 0;
}
