/**
    Scenario:
        Why double pointer ?
            In C, a single pointer can physically store the address of another pointer because all memory addresses on a given system are the exact same size (e.g., 8 bytes on a 64-bit system). 
            However, we must use a double pointer because C enforces strict data type safety, type compatibility, and correct pointer arithmetic compilation.

        The Core Reason: Type Compatibility
            A pointer in C does not just store a raw memory address;
            it also dictates the data type of the value residing at that address.

        Single Pointer (int *ptr):
            This type tells the compiler: "I store the memory address of an integer variable."
        Double Pointer (int **dptr):
            This type tells the compiler: "I store the memory address of an int * variable (a single pointer)."

        If we attempt to force the address of a single pointer into another single pointer (e.g., type *ptr1 = &ptr),
        the compiler will flag a type mismatch warning or error.
        It cannot safely determine how many times to look up (dereference) the memory to find the final underlying data
 */

#include <stdio.h>

int main() {

    int age = 18;

    int *singlePointer = &age;

    int **doublePointer = &singlePointer;

    /*
        How Memory Levels Look Under the Hood:
            The different levels of pointers tell the compiler exactly how many "steps" or "hops" of
            indirection it needs to take through memory to reach the actual value.
     */

    printf("Level 0 (Variable)              Level 1 (Single Pointer)            Level 2 (Double Pointer)\n");
    printf("+---------------+               +-------------------+               +-------------------+\n");
    printf("|  int x = %d;  | <------------ |   int *ptr = &x;  | <------------ | int **dptr = &ptr;|\n", age);
    printf("+---------------+               +-------------------+               +-------------------+\n");
    printf("Address: %p         Address: %p             Address: %p\n",&age, &singlePointer, &doublePointer);
    printf("Value: %d                       Value: %p               Value: %p\n",**doublePointer, *doublePointer, doublePointer);

    /**
     After running, we got below as output:
        Level 0 (Variable)              Level 1 (Single Pointer)            Level 2 (Double Pointer)
        +---------------+               +-------------------+               +-------------------+
        |  int x = 18;  | <------------ |   int *ptr = &x;  | <------------ | int **dptr = &ptr;|
        +---------------+               +-------------------+               +-------------------+
        Address: 0x7ffc99b6a31         Address: 0x7ffc99b6a38             Address: 0x7ffc99b6a39
        Value: 18                       Value: 0x7ffc99b6a31               Value: 0x7ffc99b6a38


        Why the Compiler Requires This Separation:

            1. Proper Dereferencing (* operator)
                The compiler reads the number of asterisks to figure out what data it retrieves during a dereference operation:
                    -> *dptr goes to address 0x2000 and reads the value 0x1000 (an int * address).
                    -> **dptr jumps two levels to address 0x1000 and reads the final integer value 10.
                    -> If we used a single pointer for everything, the compiler would not know whether a dereference should yield another address or the actual integer data.

            2. Accurate Pointer Arithmetic
                When you increment a pointer (ptr++), the compiler advances the memory address based on the byte size of the type it points to.
                    -> On a 64-bit machine, an int takes 4 bytes, but an int * address takes 8 bytes.
                    -> If we increment an int *, it moves forward by 4 bytes.
                    -> If we increment an int **, it moves forward by 8 bytes.
                    -> Using the wrong pointer tier will corrupt your memory navigation because the step size will be calculated incorrectly.
     */

    return 0;
}