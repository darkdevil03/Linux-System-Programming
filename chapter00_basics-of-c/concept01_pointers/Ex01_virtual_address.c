#include  <stdio.h>

int main() {
    int intVariable;
    // Printing the virtual address which consist of { Page + offset } ; which commonly calls as address of variable.
    printf("Address of intVariable = %p\n", &intVariable);

    return 0;
}