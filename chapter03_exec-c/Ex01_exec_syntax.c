/**
    Exec family():
        The exec family of system calls erases only the user space of the main program.
        It preserves the kernel space (the process's identity and system-level resources).
        Here is the easiest way to think about it: Imagine a process is a taxi cab.
            -> Kernel Space (The Taxi): The physical car, its license plate (Process ID / PID), and the trunk (open files).
               This is managed by the Operating System.
            -> User Space (The , and memory currently running.
        When you call exec(), the OS kicks out the current passengers (your old code) and puts new passengers inside (the new program).
        However, it is still the exact same taxi with the exact same license plate.

        The Breakdown:
            What gets ERASED (User Space replaced):
                -> Text Segment: Your old program's compiled C code is wiped and replaced with the new program's code.
                -> Data/BSS Segments: Global and static variables from the old program are gone.
                -> Heap: Dynamically allocated memory (malloc) from the old program is freed/replaced.
                -> Stack: Local variables and function call history are wiped clean.
            What is PRESERVED (Kernel Space kept)
                -> Process ID (PID): The new program runs under the exact same PID.
                -> Parent Process ID (PPID): The parent remains the same.
                -> File Descriptors: Any files the old program opened (like text files or network sockets) stay open
                and can be used by the new program (unless explicitly told to close).

    Scenario:
        The best way to prove that the kernel space (the process identity) is not destroyed is to check the Process ID (PID) before and after calling exec().
        To do this, we need two separate C files.

        1. The New Program (Ex00_new_program.c)
            This is the program we want to switch to. It just prints its PID.
            ( Compile this first: gcc new_program.c -o new_program
                                                or
             use IDE binary file name { I have used the IDE generated binary file after compiling} )

*/


#include <stdio.h>
#include <unistd.h>

int main() {
    printf("[MAIN PROGRAM] I am starting up.\n");
    printf("[MAIN PROGRAM] My Process ID (PID) is: %d\n", getpid());
    printf("[MAIN PROGRAM] My Parent Process ID (PID) is: %d\n", getppid());

    printf("[MAIN PROGRAM] Calling exec() now. My user space segments will be erased, goodbye!!\n");
    printf("--------------------------------------------------\n");

    // execl replaces this program's user space with "./new_program"
    execl("./Ex00_new_program", "new program via exec family", NULL);

    // If exec() is successful, this line will NEVER print,
    // because the user space holding this code was erased!
    printf("This will never print.\n");

    return 0;
}