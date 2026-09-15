#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <asm-generic/errno-base.h>

int main() {
    int fd = open("/media/my_usb_drive/photo_backup.jpg", O_WRONLY | O_CREAT, 0644);
    if (fd == -1) return EXIT_FAILURE;

    // 1. We write the data. It returns success because it just goes into RAM.
    write(fd, "Fake Photo Data...", 18);

    // ... IN THE BACKGROUND, THE USER UNPLUGS THE USB DRIVE ...

    // 2. We MUST check the close() function!
    if (close(fd) == -1) {
        // Because the drive was yanked, the background save failed.
        // close() returns -1 and sets errno to EIO (Input/Output Error).
        perror("[CRITICAL ERROR] Failed to save data. The drive may have been removed!");

        // Notice we DO NOT try to close(fd) again. The OS already freed it.
        // Although POSIX allows it, close( ) will never return EINTR. The Linux kernel developers know
        // better—such an implementation is not smart.
        return EXIT_FAILURE;
    }

    printf("[SUCCESS] Photo safely backed up.\n");
    return EXIT_SUCCESS;
}
