#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int fd = open("File20_data.txt", O_RDONLY);
    char buffer[15]; // 14 bytes for data + 1 for null-terminator
    buffer[14] = '\0'; // Ensure string is safely terminated for printing
    ssize_t bytes_read = 0;

    if (fd == -1) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    printf("\n\t\t\t--- Demonstrating lseek() ---\n\n");

    printf("Initially, When The File Opened : Seek/Point To %ldth Byte Location!!\n", lseek(fd, 0, SEEK_CUR));
    printf("Total Bytes in file including data and nextline character (refers to enter for nextline) : %ld\n\n", lseek(fd, 0, SEEK_END));

    // ---------------------------------------------------------
    // 1. SEEK_SET: Jump exactly to Byte 11 (Start of Line 2)
    // ---------------------------------------------------------
    lseek(fd, 11, SEEK_SET);
    bytes_read = read(fd, buffer, 10);
    buffer[bytes_read] = '\0'; // Safely terminate exactly where the read stopped
    printf("1. SEEK_SET (Pos +11): %s\n", buffer); // Prints: Line 02...

    // Note: After reading 10 bytes, our cursor is now sitting at Byte 20.

    // ---------------------------------------------------------
    // 2. SEEK_CUR: Skip forward 14 bytes from our current spot
    // ---------------------------------------------------------
    // We are at Byte 20. We want to skip Line 3.
    // 20 + 14 = Byte 34 (Start of Line 4)
    lseek(fd, 14, SEEK_CUR);
    bytes_read = read(fd, buffer, 11); // Line 4: contains 11 bytes excludes nextline character (enter or '\n').
    buffer[bytes_read] = '\0'; // Safely terminate exactly where the read stopped
    printf("2. SEEK_CUR (Pos +14): %s\n", buffer); // Prints: Line 04...

    // ---------------------------------------------------------
    // 3. SEEK_END: Jump backwards from the very end of the file
    // ---------------------------------------------------------
    // The file is 119 bytes long.
    // We want to read the 9th line, which starts at Byte 80.
    // End (119) - 23 = Byte 96.
    lseek(fd,-23 , SEEK_END);

    bytes_read = read(fd, buffer, 11);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0'; // Safely terminate exactly where the read stopped
        printf("3. SEEK_END (Pos -23): %s\n", buffer); // Prints: Line 09...
    } else if (bytes_read == 0) {
        printf("3. SEEK_END: [EOF - No data left to read]\n");
    } else {
        printf("[ERROR] Unable to read the file!!\n");
    }


    // Bonus Trick: Finding out where you are!
    // As the book states, passing a pos of 0 with SEEK_CUR returns your current position.
    off_t current_spot = lseek(fd, 0, SEEK_CUR);
    printf("\nCurrently sitting at byte: %ld\n", current_spot); // Prints: 90

    if (close(fd) == -1) {
        perror("Failed to close file!!");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/**
Line Text,Visible Characters,Invisible Newline (\n),Total Bytes per Line
    |-----------------|---------------------|--------------------------|---------------|
    |       TEXT      | Visible Char Count  |  Invisible Newline Char  |   Total Bytes |
    |-----------------|---------------------|--------------------------|---------------|
    |  Line-1:ONE     |           10        |             1            |    11         |
    |-----------------|---------------------|--------------------------|---------------|
    |  Line-2:TWO     |           10        |             1            |    11         |
    |-----------------|---------------------|--------------------------|---------------|
    |  Line-3:THREE   |           10        |             1            |    11         |
    |-----------------|---------------------|--------------------------|---------------|
    |  Line-4:FOUR    |           10        |             1            |    11         |
    |-----------------|---------------------|--------------------------|---------------|
    |  Line-5:FIVE    |           10        |             1            |    11         |
    |-----------------|---------------------|--------------------------|---------------|
    |  Line-6:SIX     |           10        |             1            |    11         |
    |-----------------|---------------------|--------------------------|---------------|
    |  Line-7:SEVEN   |           10        |             1            |    11         |
    |-----------------|---------------------|--------------------------|---------------|
    |  Line-8:EIGHT   |           10        |             1            |    11         |
    |-----------------|---------------------|--------------------------|---------------|
    |  Line-9:NINE    |           10        |             1            |    11         |
    |-----------------|---------------------|--------------------------|---------------|
    |  Line-10:TEN    |           10        |             1            |    11         |
    |-----------------|---------------------|--------------------------|---------------|

    For LINE-10:TEN case of 11,0 (See note below),11


    The Core Truth: EOF is a Condition, Not a Character
        There is absolutely no physical EOF byte sitting after the N in TEN.
        If our file has 119 bytes of text, the physical file size on the hard drive is exactly 119 bytes.

        1)  Old Computers (The Past): In the 1980s (like MS-DOS or CP/M),
             computers did put a physical character (usually Ctrl+Z) at the end of a file to mark where it stopped.

        2)  Modern Linux (The Present): Linux does not do this.
             Instead, it relies entirely on the file's Metadata (the Inode).

    The Flow of the above code for:
      When your program calls read(), the Linux kernel checks the file's metadata (Inode).
        1.  The metadata says: File Size = 119 bytes.
        2.  The kernel tracks your cursor position.
        3.  We read characters until your cursor reaches byte number 119 (the letter N).
        4.  We call read() one more time.
        5.  The kernel sees our cursor is at 119, which equals the total file size.
        6.  Instead of returning a character, the read() function simply returns 0.
      That 0 returned by the read() function is the "End of File" indicator.
      It is just a status code sent by the operating system, not a piece of data pulled from the text file.

    The Final Math
        Total Visible Characters: 110 bytes
        Total Newline Characters (Lines 1-9): 9 bytes
        Total File Size: 119 bytes

    However, as we discussed, there is no EOF character.
    All 119 bytes are purely our text and our newline formatting.

    (Note: If we pressed the "Enter" key on our keyboard after typing TEN,
    our text editor will place a 10th invisible \n character at the very end of the file,
    making the total file size exactly 120 bytes).

*/