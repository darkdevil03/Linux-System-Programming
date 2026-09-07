/**
Scenario:
    In this program, it reaches the read() function and completely freezes.
    It cannot do any other work until we type something and press Enter.
Details:
    To test standard terminal input (our keyboard), we read from STDIN_FILENO (File Descriptor 0).
    Here are this program demonstrating action of reading from the keyboard.

 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char buf[21];
    ssize_t bytes_read = 0;

    printf("--- BLOCKING MODE ---\n");
    printf("The program is now frozen. Type something and press Enter: ");
    fflush(stdout); // Ensure the text prints before we freeze

    // We read directly from STDIN_FILENO (0)
    bytes_read = read(STDIN_FILENO, buf, 20);

    if (bytes_read == -1) {
        if (errno == EINTR) {
            printf("\nInterrupted by signal, but we could try again.\n");
        } else {
            perror("Error reading happened!!"); // This statement invoke cases written from line 49 to 92.
        }
        return EXIT_FAILURE;
    }
    if (bytes_read == 0) {
        // This specifically catches Ctrl+D (End of File)
        printf("\n\n[EOF] You pressed Ctrl+D! The input stream was closed.\n");
        return EXIT_SUCCESS;
    }

    // Cap the string and print it
    buf[bytes_read] = '\0';
    printf("Number of data read: %zd\n", bytes_read);
    printf("Data received: %s", buf);

    return 0;
}

/**
When the perror("\n[ERROR] Error reading") invoked?
    The block gets invoked when the read() function encounters a genuine,
    critical failure—meaning it returns -1, but the error is not EINTR (an interruption).
    When reading from the standard terminal keyboard (STDIN_FILENO), real hardware errors are extremely rare.
    Usually, if perror triggers here, it means the programmer made a mistake in the code, or the environment running the program is broken.

    Here are the specific scenarios that would cause your program to bypass EINTR and invoke perror:

        Case 1: The File Descriptor was closed (EBADF - Bad File Descriptor)
                    If we (or another part of a larger program) accidentally closed the standard input before trying to read from it,
                    the kernel will instantly reject the read request.

            How to force it in your code:
                Add close(STDIN_FILENO); right before our read() call.

            What perror will print:
            [ERROR] Error reading: Bad file descriptor

        Case 2: We passed a bad memory address (EFAULT - Bad Address)
                    The read() function needs a valid place in memory (your buf array) to store the data.
                    If we accidentally pass a NULL pointer or an uninitialized pointer,
                    the kernel detects that you are trying to write to invalid memory and stops us to prevent a crash.

            How to force it in your code:
                Change bytes_read = read(STDIN_FILENO, buf, 20); to bytes_read = read(STDIN_FILENO, NULL, 20);

            What perror will print:
                [ERROR] Error reading: Bad address

        Case 3: Redirecting a Directory into your Program (EISDIR - Is a Directory)
                    In Linux, we can use the command line to feed files into a program instead of typing on the keyboard (e.g., ./my_program < input.txt).
                    But if you accidentally feed a folder into the program, read() will refuse to read it.

            How to force it in the terminal:
                Compile our program, then run it like this: ./your_program < /etc/

            What perror will print:
                [ERROR] Error reading: Is a directory

        Case 4: Hardware / Terminal Driver I/O Error (EIO - Input/Output Error)
            This is a true system failure. It happens if the physical terminal or SSH connection you are using suddenly crashes,
            disconnects, or loses power at the exact microsecond the kernel is trying to fetch the keystroke.
            We cannot easily trigger this in code, as it relies on a physical hardware or driver failure.
 */