/**
    Scenario:
        What is a Pointer?
            Think of computer memory as a giant street with houses.
            Every house has an address and contains a value.A regular variable stores a value (e.g., int age = 25;).
            A pointer is a special variable that stores a memory address instead of a data value.
 */

#include <stdio.h>

int main() {
    int age = 25;
    /*
        The Two Core Operators:
            & (Address-of operator): Gets the memory address of a variable.
            * (Dereference operator): Accesses or modifies the value stored at the memory address.
     */
    int *ptr = &age; // *ptr holds the address of age at declaration & initialization.

    printf("-------------------------------------------\n");
    printf("| Value of age : %d                       |\n", age);       // 25
    printf("| Value via pointer : %d                  |\n", *ptr);  // 25 (Dereferencing)
    printf("-------------------------------------------\n");
    printf("---------------------------------------------------\n");
    printf("| Address of age via &age : %p        |\n", &age);     // e.g., 0x7ffee4
    printf("| Address of age via pointer : %p     |\n", ptr);     // Address of a age via ptr
    printf("---------------------------------------------------\n");


    return 0;
}