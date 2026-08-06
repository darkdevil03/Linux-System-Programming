/**
    Dynamic Memory Allocation (malloc, free)
        Instead of fixed-size arrays, you can request exact amounts of memory from the "heap" during runtime.
        Always free() what you allocate to prevent memory leaks.
 */

#include <stdio.h>
#include <stdlib.h>

int main() {
    const int size = 5;
    // Request memory for 5 integers
    int *dynamic_arr = malloc(size * sizeof(int));

    if (dynamic_arr == nullptr) {
        return 1; // Memory allocation failed
    }

    dynamic_arr[0] = 100; // Use it just like an array
    dynamic_arr[1] = 200;
    dynamic_arr[2] = 300;  // For remaining, dynamic_arr[3,4] garbage value will be allocated by compiler of malloc function

    for (int i = 0; i < size; i++) {
        printf("dynamic_arr[%d] = %d\n", i, dynamic_arr[i]);
    }

    free(dynamic_arr); // Release the memory back to the system
    dynamic_arr = nullptr; // Good practice
    return 0;
}