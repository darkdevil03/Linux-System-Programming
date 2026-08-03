/**
    Scenario:
        Pointers and Arrays
            The name of an array acts as a constant pointer to its first element (&arr[0]).
            we can traverse arrays using pointer arithmetic (ptr + i) or (i + ptr).
 */

#include  <stdio.h>

int i = 10;
char *ptr3 = "hello";

int main() {

    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};  // Stack segment

    int *ptr = arr; //*ptr = &arr[0]; // points to arr[0]

    char *ptr2 = "Hello";


    printf("Address of arr = %p\n", arr);
    printf("Address of arr via ptr = %p\n", ptr);


    // Different syntax for accessing address and value using ptr
    printf("Address of arr[0] = %p ; and it's value = %d\n", (ptr+0), *(ptr+0));  // For starting ptr+0 -> ptr because it's pointing zero th index value.

    printf("Address of arr[1] = %p ; and it's value = %d\n", (1+ptr), *(1+ptr));

    printf("Address of arr[2] = %p ; and it's value = %d\n", (void*)ptr+2,ptr[2]);

    printf("Address of arr[3] = %p ; and it's value = %d\n", (void*)(3+ptr),3[ptr]);

    for (int i = 4; i <= 8; i++) {
        printf("Address of arr[%d] = %p ; and it's value = %d\n", i, (ptr+i),*(ptr+i));
    }


    // Altering the value via ptr
    *(ptr+4) = 8;
    printf("\nptr[4] value changed to 8: \n");
    printf("Address of arr[4] = %p ; and it's value = %d\n", (void*)(4+ptr),*(4+ptr));

    return 0;
}