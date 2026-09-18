/**
    To test this on our 64-bit Ubuntu system, we must force the GNU C Compiler (GCC) to compile our program as a strict 32-bit binary.
    This artificially shrinks the off_t variable back down to its 2-Gigabyte limit, allowing we to observe the actual EOVERFLOW crash.

    Here is the step-by-step process to simulate a 32-bit environment and trigger the exact error described in the book.

    1. Install 32-Bit Compiler Support
        Because we are on a 64-bit Linux installation, GCC needs the 32-bit standard C libraries to compile 32-bit code.
        Run this in our terminal:
            sudo apt update
            sudo apt install gcc-multilib

    2. Create a "Fake" 3GB File
        Instead of trying to do math inside the C code (which often just results in an EINVAL "Invalid Argument" error
        because the C compiler corrupts the math into a negative number before it even reaches the kernel),
        we will create a file that is already 3 Gigabytes.

        Use the terminal to create a 3GB Sparse File (this takes up 0 bytes on our physical disk):
        Note: use below command at the program directory folder or paste the absolute path file proper after executed below command
            truncate -s 3G massive.bin

    3. The 32-Bit C Code Demonstration
        Save the following code as Ex23_limitation_seeking_based_arch.c.

        Notice that we must pass a special Linux flag O_LARGEFILE just to get the 32-bit program to open the file.
        Without it, a 32-bit system will throw an EOVERFLOW panic at the open() stage before lseek() even gets a chance to run.

    4. Compile and Run as 32-Bit
        Compile the program using the -m32 flag. This flag commands GCC to ignore our 64-bit CPU and compile a strict 32-bit executable.

        Case 1: For stimulating that our system 32-bit to generate overflow case scenario in 32-bit system
            gcc -m32 Ex23_limitation_seeking_based_arch.c -o test_overflow
            ./test_overflow

        Case 2: For stimulating that our system 32-bit with a magic fix for reading 3GB file iin 32-bit system case.

                The "Magic Fix" (Large File Support)
                    If we are forced to program on a 32-bit machine (like a Raspberry Pi or an older embedded device)
                    but we need to read files larger than 2GB, Linux has a workaround called Large File Support (LFS).

                    If we add this exact line to the absolute very top of our C file (before any #include statements):
                        #define _FILE_OFFSET_BITS 64 { we already added to our below code at line 53 just uncomment it and do below steps.}

                Recompile AND Rerun with the same commands:
                    gcc -m32 Ex23_limitation_seeking_based_arch.c -o test_overflow
                    ./test_overflow

        Note: If we compile it without the -m32 flag, the program will easily report the file size as 3221225472 bytes,
        because our 64-bit off_t variable has a maximum limit of 9 Exabytes.
 */

#define _GNU_SOURCE // Required to unlock O_LARGEFILE
//#define _FILE_OFFSET_BITS 64 // Uncomment this line for Case 2: re-compile and re-run with -m32 flag

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main() {
    printf("--- 32-Bit File Size Limit Demonstration ---\n\n");

    // 1. Open the 3GB file.
    // We MUST use O_LARGEFILE. Otherwise, standard 32-bit open()
    // detects the file is >2GB and blocks access entirely.
    int fd = open("Created_Files/massive.bin", O_RDONLY | O_LARGEFILE);
    if (fd == -1) {
        perror("[ERROR] Failed to open file");
        return EXIT_FAILURE;
    }
    printf("[1] Successfully opened massive.bin using O_LARGEFILE.\n");

    // 2. Ask the kernel to jump to the end of the file and return the size.
    printf("[2] Attempting to find file size using a 32-bit lseek()...\n");
    off_t size = lseek(fd, 0, SEEK_END);

    // 3. Check for the EOVERFLOW panic
    if (size == (off_t)-1) {
        if (errno == EOVERFLOW) {
            printf("\n[SUCCESS] EOVERFLOW Triggered!\n");
            printf("[CASE-1]The kernel found the end of the 3GB file, but realized the resulting\n");
            printf("byte number (3,000,000,000) is physically too huge to fit into our\n");
            printf("program's tiny 32-bit 'off_t' variable.\n");
        } else {
            perror("lseek failed with a different error");
        }
    } else {
        #if _FILE_OFFSET_BITS != 64
            // Case 1: Standard 32-bit mode (off_t remains a standard 32-bit integer)
            printf("Seek succeeded! Size: %ld.\n", (long)size);
            printf("([Warning] If you see this, you forgot to compile with the -m32 flag!)\n");
        #else
            // Case 2: Large File Support is enabled (off_t is upgraded to 64-bit)
            printf("Seek succeeded! Size: %lld.\n", (long long)size);
            printf("[Case-2] You have uncommented the #define _FILE_OFFSET_BITS 64 at line 53,");
            printf("which upgrades of 32-bit offest to 64-bit offset range!!\n");
        #endif
    }

    close(fd);
    return EXIT_SUCCESS;
}