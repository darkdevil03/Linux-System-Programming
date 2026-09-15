#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    char write_buf[] = "Data entry";
    char read_buf[100];

    printf("\t--- Synchronization Flags Demonstration ---\n");

    // 1. O_SYNC (Data + Metadata on Write)
    // Every time we write, the program pauses until the data AND
    // the file size/modified time are physically on the disk.
    printf("\n---------------------------------------------------------\n");
    printf(" 1. O_SYNC: The Financial Log (Data + Metadata on Write)\n");
    printf("---------------------------------------------------------\n");
    int fd_finance = open("File18_financial_log.txt", O_CREAT | O_WRONLY | O_APPEND | O_SYNC, 0644);
    if (fd_finance != -1) {
        write(fd_finance, write_buf, sizeof(write_buf));
        printf("[SUCCESS] Financial log safely written to physical disk.\n");
        close(fd_finance);
    }

    // ---------------------------------------------------------
    // 2. O_DSYNC (Data Only on Write)
    // ---------------------------------------------------------
    // We only care about the sensor numbers. We allow the OS to delay
    // saving the file's metadata to speed things up.
    // (Note: On Linux, this will just behave like O_SYNC anyway).
    printf("\n---------------------------------------------------------\n");
    printf(" 2. O_DSYNC: The Sensor Log (Data Only on Write)\n");
    printf("---------------------------------------------------------\n");
    int fd_sensor = open("File18_sensor_log.txt", O_CREAT | O_WRONLY | O_APPEND | O_DSYNC, 0644);
    if (fd_sensor != -1) {
        write(fd_sensor, write_buf, sizeof(write_buf));
        printf("[SUCCESS] Sensor data safely written to physical disk.\n");
        close(fd_sensor);
    }

    // ---------------------------------------------------------
    // 3. O_RSYNC (Metadata on Read)
    // ---------------------------------------------------------
    // O_RSYNC must be paired with O_SYNC (or O_DSYNC).
    // When we read this file, the program will pause until the new
    // "Access Time" (atime) is physically saved to the hard drive.
    printf("\n---------------------------------------------------------\n");
    printf(" 3. O_RSYNC: The Top Secret File (Metadata on Read)\n");
    printf("---------------------------------------------------------\n");
    int fd_secret = open("File18_top_secret.txt", O_CREAT | O_RDWR | O_APPEND | O_SYNC | O_RSYNC, 0644);
    if (fd_secret != -1) {
        // We write some initial data to it first
        write(fd_secret, "Classified launch codes.", 24);

        // Reset our cursor to the beginning of the file to read it
        lseek(fd_secret, 0, SEEK_SET);

        // THE READ OPERATION
        // Because of O_RSYNC, this read() function will not return until
        // the OS writes the new Access Time (atime) to the physical disk.
        read(fd_secret, read_buf, 24);
        printf("[SUCCESS] Secret file read. Access Time (atime) permanently secured.\n");

        close(fd_secret);
    }

    return EXIT_SUCCESS;
}