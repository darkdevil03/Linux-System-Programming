/*
What is Process ?
        A process is a program in execution. When we write source code in C or C++ and compile it, the compiler generates an executable binary file.
        This executable file is called a program. When the program is loaded into memory and executed, it becomes a process.

        A process is an 'active' entity instead of a program, which is considered a 'passive' entity.
        A single program can create many processes when run multiple times;
        for example, when we open a .exe or binary file multiple times, multiple instances begin (multiple processes are created).

    Memory Layout of a Process :
        A process in memory is divided into several distinct sections, each serving a different purpose.
        Here's how a process typically looks in memory.


            Text Section: A text or code segment contains executable instructions. It is typically a read only section
            Stack: The stack contains temporary data, such as function parameters, returns addresses, and local variables.
            Data Section: Contains the global variable.
            Heap Section: Dynamically memory allocated to process during its run time.
 */


#include <stdio.h>
#include <stdlib.h>


// --- BSS Segment (Uninitialized Data) ---
int global_uninit;

// --- Data Segment (Initialized Data) ---
int global_init = 10;

// --- Text Segment (Code) ---
void dummy_function() {
    // Just a placeholder function to get a code address
}

int main() {
    // --- BSS Segment ---
    static int static_uninit;

    // --- Data Segment ---
    static int static_init = 20;

    // --- Stack Segment ---
    int local_var = 30;

    // --- Heap Segment ---
    int *heap_var = (int*)malloc(sizeof(int));

    printf("Memory Layout Demonstration in C\n");
    printf("================================\n\n");

    printf("1. Text/Code Segment:\n");
    printf("   Address of function:       %p\n\n", (void*)dummy_function);

    printf("2. Data Segment (Initialized):\n");
    printf("   Address of global_init:    %p\n", (void*)&global_init);
    printf("   Address of static_init:    %p\n\n", (void*)&static_init);

    printf("3. BSS Segment (Uninitialized):\n");
    printf("   Address of global_uninit:  %p\n", (void*)&global_uninit);
    printf("   Address of static_uninit:  %p\n\n", (void*)&static_uninit);

    printf("4. Heap Segment (Dynamic):\n");
    printf("   Address of heap_var:       %p\n\n", (void*)heap_var);

    printf("5. Stack Segment (Local):\n");
    printf("   Address of local_var:      %p\n\n", (void*)&local_var);

    free(heap_var);
    return 0;
}