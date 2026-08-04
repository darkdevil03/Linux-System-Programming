#include <stdio.h>
#include <stdlib.h>

void badAllocate(int *p) {
    p =  malloc(sizeof(int)); // Only changes the local copy 'p'
    *p = 42;
}

int main() {
    /*
        To avoid the Null Pointer (Safety)
            Always initialize pointers. If we don't have an address to assign yet,
            set it to NULL to prevent it from pointing to a random, potentially protected memory location
            (a "wild pointer").
    */
    int *ptr = NULL;
    badAllocate(ptr);
    // CRASH: ptr is still NULL here because badAllocate received a copy of ptr

    if (ptr == NULL) {
        printf("Pointer pointing to null!!");
    }else {
        printf("value of ptr: %d\n", *ptr); // Only dereference if it's safe!
    }

    return 0;
}
