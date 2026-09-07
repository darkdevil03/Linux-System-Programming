/**
 Scenario:
    In this program, we tell the kernel not to freeze the program. The while loop acts like a video game engine.
    If read() finds no keyboard input, it instantly returns EAGAIN. The program uses that opportunity to do "other work"
    (in this case, printing dots) and checks the keyboard again half a second later.
 Usage:
    How to test the Non-Blocking version:
        1. Compile and run this program.
        2. Do not type anything for a few seconds.
        3. We will see ..... appearing continuously on your screen.
           The program is actively looping, receiving EAGAIN thousands of times,
           and doing "work" (printing dots) instead of waiting for us.
        4. Type Hello and press Enter. The loop will instantly capture the text, break, and print it.
 */

#include <errno.h>
#include <fcntl.h>   // Required for fcntl()
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char buf[255];
    ssize_t bytes_read = 0;

    /**
        To test standard terminal input (your keyboard), we read from STDIN_FILENO (File Descriptor 0).
        Because the terminal is already opened by the operating system when our program starts, we do not use open().
        To make it non-blocking, we use a new function called fcntl() (File Control) to
        flip the O_NONBLOCK switch on the already-open terminal.
    */

    // 1. Get the current settings of the standard input
    int current_fd_stdin_setting = fcntl(STDIN_FILENO, F_GETFL, 0);
    
    // 2. Add the O_NONBLOCK flag to those settings
    fcntl(STDIN_FILENO, F_SETFL, current_fd_stdin_setting | O_NONBLOCK);

    printf("--- NON-BLOCKING MODE ---\n");
    printf("Type something and press Enter. Notice how the program keeps running!\n");

    // 3. The "Game Loop"
    while (1) {
        bytes_read = read(STDIN_FILENO, buf, 255);

        if (bytes_read == -1) {
            if (errno == EAGAIN) {
                // The keyboard is empty! 
                // We did NOT freeze. We can do other work here.
                printf(".");
                fflush(stdout);
                
                // Sleep for 1.8 seconds to simulate doing other work
                usleep(1800000);
                continue; // Loop back around and check the keyboard again
            }
            if (errno == EINTR) {
                printf("\n[EINTR] Interrupted by signal");
                continue;
            }
            perror("Critical read error");
            break;
        }
        // We successfully captured keyboard data (such as typed some characters and press enter)
        // then Break the loop via below statement.
            break;
    }

    buf[bytes_read] = '\0';
    printf("\nNumber of bytes read from keyboard that you typed : %zd \n",bytes_read-1);
    printf("[Success] You typed data : %s", buf);

    // 4. Cleanup: Restore the terminal back to its normal blocking state
    fcntl(STDIN_FILENO, F_SETFL, current_fd_stdin_setting);

    return EXIT_SUCCESS;
}