#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>

int main() {

    printf("[SELECT] Starting the process...\n");

    // 1. Create a timeval structure
    struct timeval timeout;

    // 2. Set the sleep duration: 15 second and 500,000 microseconds (0.5 seconds)
    timeout.tv_sec = 15; // Seconds
    timeout.tv_usec = 5; // Microseconds (1,000,000 usec = 1 sec)

    printf("[MAIN-PROGRAM] I am going for sleep for 15.5sec using select()\n");
    // 3. THE HACK: Call select(): Capture the return value of select()
    // - Arg 1: 0 (Highest file descriptor + 1. Since we have none, we pass 0)
    // - Arg 2: NULL (No files to watch for reading)
    // - Arg 3: NULL (No files to watch for writing)
    // - Arg 4: NULL (No files to watch for errors)
    // - Arg 5: &tv (Our 1.5 second timer)
    int ret = select( 0, nullptr, nullptr, nullptr, &timeout);

    // 4. Error Validation
    if (ret == -1) {
        if (errno == EINTR) {
            printf("[WARNING] Sleep was interrupted early by a signal!\n");
        } else {
            perror("[ERROR] select() failed due to an invalid argument");
            return EXIT_FAILURE;
        }
    }
    // 5. Success Validation (Timeout Expired)
    else if (ret == 0) {
        // Because we gave select() zero files to watch, returning 0
        // (meaning 0 files are ready) is the exact successful behavior we want!
        printf("[SUCCESS] Woke up! The 1.5 second timeout expired normally.\n");
    }
    // 6. The "Impossible" Case
    else {
        // Since we didn't give it any files to watch, it should be impossible
        // for select() to find a ready file and return a number greater than 0.
        printf("[UNKNOWN] select returned %d, which shouldn't happen here!\n", ret);
    }

    return EXIT_SUCCESS;
}