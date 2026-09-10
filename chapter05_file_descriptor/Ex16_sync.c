/**
    The sync() system call is a broad, "old-school" command that forces the operating system to
    flush every single unsaved data buffer (for all files, across the entire system) from temporary RAM to the physical disk.
    Unlike other synchronization methods that target specific files, sync() acts as a global save command for the entire computer.
    When sync() is called, "global scope" means every single program running on the entire computer,
    not just the C program that executed the command.
    It forces the operating system to flush every unsaved buffer currently sitting in RAM, regardless of who or what created it.

    Here is exactly what that scope includes:
        Our C Program: It will save all data from every file you have opened using open(),
        regardless of the file extension (e.g., .txt, .csv, .bin).
        Other Programs (System-Wide): It will simultaneously save pending data for every other application
        running on the machine at that moment. This includes your web browser, a database running in the background,
        a Python script downloading a file, or system logging services.

        All File Types: The Linux kernel does not care about file extensions or what language a program was written in.
        To the operating system, all open files are just raw data blocks sitting in the Page Cache.
        sync() pushes every dirty block down to the physical disk.

    Why it works this way:
        Notice that the sync() function takes absolutely no parameters sync(void).
        We do not pass it a file descriptor (fd). Because you cannot specify which file to save,
        the command simply tells the Linux kernel to dump the entire system's temporary memory cache to the physical hard drive.
        This total system freeze is why sync() can take several minutes on a busy system,
        and why regular applications should use fsync(fd) to save only their specific documents.
*/

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    // 1. The fsync() way (Targeted, Fast, Recommended)
    int fd = open("File16_sync.txt", O_CREAT | O_WRONLY, 0644);
    if (fd != -1) {
        write(fd, "Important data", 14);

        // Saves ONLY "File16_sync.tx" to disk.
        if (fsync(fd) == -1) {
            perror("fsync failed");
        }
        close(fd);
    }

    // 2. The sync() way (Global, Slow, System-wide)
    printf("Initiating system-wide sync...\n");

    // sync() takes no parameters and returns nothing (void).
    // On Linux, this will block and wait until ALL system buffers
    // (from every program) are securely written to the disk.
    sync();

    printf("All system data is now safe on the disk.\n");

    return EXIT_SUCCESS;
}