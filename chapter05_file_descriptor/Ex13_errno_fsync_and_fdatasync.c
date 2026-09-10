#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main() {
    // ---------------------------------------------------------
    // 1. Triggering EBADF (Bad File Descriptor)
    // ---------------------------------------------------------
    /*      Simple: Here when we are asking the system to save changes,
                    but the ID you provided either doesn't exist or belongs
                    to a file you are only allowed to read, not change.
            In-Depth: The operating system checks the process's File Descriptor table.
                      If the integer fd does not point to an open file object,
                      or if the file was opened with read-only permissions (O_RDONLY),
                      the kernel rejects the fsync or fdatasync call.
                      It prevents you from attempting to flush write buffers
                      for a file we have no permission to write to.
     */

    printf("--- Testing EBADF ---\n");

    // 1. The Read-Only Scenario (This will SUCCEED)
    int fd_readonly = open("File15_Fsync_With_Create_Flag.txt", O_RDONLY);
    if (fd_readonly != -1) {
        // Here, we read some data from file path.
        // In case fd is opened in read: if case will true
        printf("[CASE-1] : ");
        if (fsync(fd_readonly) == 0) {
            printf("[SUCCESS] fsync() worked on a read-only file descriptor!\n");
        }else {
            // In case fd malfunctioned/closed then else part will execute
            if (fsync(fd_readonly) == -1 && errno == EBADF) {
                printf("[FAILED] Cannot sync the file!!!\n");
            }
        }
        // Close it so we can use it for the next test
        close(fd_readonly);
    }

    // 2. The Closed FD Scenario (This will trigger EBADF)
    // fd_readonly is now closed and invalid.
    printf("[CASE-2] : ");
    if (fsync(fd_readonly) == -1) {
        if (errno == EBADF) {
            printf("[EBADF CAUGHT] Cannot sync because the file descriptor is closed/invalid!\n");
        }
    }

    // 3. The Negative FD Scenario (This will also trigger EBADF)
    printf("[CASE-3] : ");
    if (fsync(-1) == -1) {
        if (errno == EBADF) {
            printf("[EBADF CAUGHT] Cannot sync because -1 is never a valid fd!\n");
        }
    }


     // ----------------------------------------------------------------
     //   2. Triggering EINVAL (Invalid Argument) -> Unsupported Object
     // ----------------------------------------------------------------
     /*      Simple: When we gave a valid ID, but it points to something that cannot physically be saved
                     to a hard drive (like a network connection, your keyboard, or a pipe).
             In-Depth: The file descriptor is valid, but the underlying object in the Virtual File System (VFS)
                       lacks a synchronization mechanism.
             For example:
                a Unix pipe or a TCP socket exists entirely in RAM.
                Because there is no physical disk block to flush data to, the kernel returns EINVAL.
                Additionally, if a specific filesystem does not implement fsync, it will return this error.
    */

    printf("\n--- Testing EINVAL ---\n");

    int pipe_fds[2];

    // Create a pipe. We use a pipe because it lives in RAM
    // and naturally rejects synchronization attempts with EINVAL.
    if (pipe(pipe_fds) == -1) {
        perror("[ERROR] Failed to create pipe!!\n");
        return EXIT_FAILURE;
    }

    printf("[STEP-1] : Attempting fsync() on an object that doesn't support it...\n");

    // The "Paranoid" Fallback Strategy
    if (fsync(pipe_fds[1]) == -1) {  // Attempting to sync a pipe makes no sense to the OS

        if (errno == EINVAL) {
            printf("[CAUGHT EINVAL] fsync() is not supported by this object.\n");
            printf("[STEP-2] : Executing fallback to fdatasync()...\n");

            // Fallback Strategy: The text notes that sometimes fsync() is missing
            // but fdatasync() works. Paranoid apps should try this fallback.
            // which refers only data available but doesn't have metadata
            if (fdatasync(pipe_fds[1]) == -1) {
                // Note: In this specific test using a pipe, fdatasync will also fail.
                // On the rare filesystems the book mentions, this might succeed!
                printf("[FALLBACK FAILED] fdatasync() also rejected the operation!!\n");
            } else {
                printf("[FALLBACK SUCCESS] fdatasync() worked where fsync() failed.\n");
            }

        } else {
            // It failed for some other reason (EBADF, EIO)
            perror("fsync failed");
        }
    } else {
        printf("[SUCCESS] fsync for file was succeeded.\n");
    }

    // Cleanup
    close(pipe_fds[0]);
    close(pipe_fds[1]);


    // ---------------------------------------------------------
    // 3. EIO (Hardware I/O Error)
    // ---------------------------------------------------------
    /*      Simple: A severe hardware failure occurred while trying to save our data to the physical disk.
            In-Depth: This is a critical failure indicating a "low-level I/O error" during the actual physical write.
                      The kernel attempted to flush dirty pages from the Page Cache via the disk controller,
                      but the hardware reported a failure. This happens if a disk has a bad sector,
                      a network drive disconnects, or a user yanks out a USB thumb drive while fsync is running.

     */
    printf("\n--- Testing EIO ---\n");
    printf("[EIO NOTE] EIO cannot be reliably triggered in software without fault injection.\n");
    printf("[EIO NOTE] It requires a real hardware failure (e.g., unplugging a USB drive during a write).\n");

    return EXIT_SUCCESS;
}