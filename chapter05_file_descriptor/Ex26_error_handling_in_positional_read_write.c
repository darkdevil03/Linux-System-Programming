/**
    Theory: return values of pread and pwrite
     Assume that in file there is data as -> Hello
       a) Positive Number (Success): The function returns and says, "I successfully read 6 bytes."
       b) Returns 0 for pread (EOF): The kernel ask physical location byte point,
                                   but the file is completely empty. There is nothing left to pick up (End of File).
       c) Returns 0 for pwrite: The kernel ask physical location byte point,
                              but return nothing off (usually because you gave them 0 byte data to write into the file).

        d) Returns -1 (Fatal Error): The job completely failed. They give you an error code (errno) explaining why it failed.

            Deep Dive into error cases: What do the last two lines mean?
                "For pread(), any valid read() or lseek() errno value is possible.
                 For pwrite(), any valid write() or lseek() value is possible."

                Because pread and pwrite are "all-in-one" functions,
                they perform two separate jobs in a single, uninterruptible step:
                    1) The Jump: Finding the exact byte location in the file (just like lseek()).
                    2) The Transfer: Moving the data into or out of that location (just like read() or write()).

            The above is pointing out that if pread() fails and returns -1,
            we have to play detective. Did it fail during The Jump, or did it fail during The Transfer?
            Because it does both jobs, it can suffer from the errors of both jobs.

            Examples of "Jump" Errors (The lseek part):
                1) EINVAL (Invalid Argument): We told pread() to read from offset -50.
                                              A file cannot have a negative byte location. The "jump" failed.

                2) ESPIPE (Illegal Seek): We tried to pread() from a live network socket or a terminal window.
                                          We cannot "jump" to byte 500 of a live internet stream.
                                          The "jump" is physically impossible.

            Examples of "Transfer" Errors (The read/write part):
                1) EBADF (Bad File Descriptor): We opened a file as "Read Only",
                                                but we tried to use pwrite() to write to it.
                                                The "jump" succeeded, but the "transfer" was blocked by permissions.

                2) ENOSPC (No Space Left): We tried to pwrite() a massive block of data, but our hard drive is 100% full.

                3) EIO (I/O Error): Our physical hard drive crashed or the USB cable was yanked out right as
                                    the data was transferring.
 */

#define XOPEN_SOURCE 500

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main() {
    int fd;
    char buffer[20];
    ssize_t result;

    printf("--- Demonstrating pread() Return Values ---\n\n");

    // 1. Create a tiny file with just 5 bytes of data
    fd = open("Created_Files/File26_Positional_read_write_test_errors.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Failed to open file! ");
        return EXIT_FAILURE;
    }
    write(fd, "HELLO", 5);

    // ---------------------------------------------------------
    // CASE A: SUCCESS (Returns > 0)
    // ---------------------------------------------------------
    // Read 3 bytes starting at offset 0
    result = pread(fd, buffer, 3, 0);
    if (result > 0) {
        buffer[result] = '\0';
        printf("[CASE A] Success! pread() returned %ld bytes. Data: '%s'\n", result, buffer);
    }

    // ---------------------------------------------------------
    // CASE B: END OF FILE (Returns 0)
    // ---------------------------------------------------------
    // Try to read 10 bytes, but start at offset 100.
    // The file is only 5 bytes long, so offset 100 is pure empty space.
    result = pread(fd, buffer, 10, 100);
    if (result == 0) {
        printf("[CASE B] EOF Reached! pread() returned 0. There is no data at offset 100.\n");
    }

    // ---------------------------------------------------------
    // CASE C: THE "lseek" ERROR (Returns -1)
    // ---------------------------------------------------------
    // Try to read from offset -10. This is physically impossible.
    // This proves the last line of your text: pread can throw lseek errors!
    result = pread(fd, buffer, 5, -10);
    if (result == -1) {
        printf("[CASE C] Error Triggered! pread() returned -1.\n");
        printf("         Error Code (errno): %d - %s\n", errno, strerror(errno));
        printf("         (Notice this is an 'lseek' style error, not a read error!)\n");
    }

    close(fd);
    return EXIT_SUCCESS;
}