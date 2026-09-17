/**
    1. What is the use of Sparse Files and Holes?
        As the text states, a "hole" is a gap of zero-padded space inside a file.
        When a file contains these holes, it is called a sparse file.

        There are two massive benefits to using them:
            Massive Physical Disk Space:
                The most important feature of a hole is that it does not occupy any physical disk space.
                If we create a 1-Gigabyte hole in a file, the file's logical size becomes 1 GB,
                but it takes up exactly 0 bytes on our physical hard drive.
                The operating system just makes a tiny note in its metadata saying,
                "Pretend there are 1 billion zeros right here."
            Enhancing Performance:
                Because the operating system doesn't actually have to write a billion physical zeros
                to the hard drive, creating this hole is instantaneous.
                It completely skips the slow process of physical Input/Output (I/O).

    2. What does "Zero-Padded" mean?
            In computer science, a "zero" is an actual piece of data.
            It is the binary byte 00000000 (also known as the Null byte, or \0 in C).
            If we open a text file and type a normal word, it takes up bytes.
            If we tell a program to write a "zero byte," it normally takes up exactly 1 byte of space.

            When the book says a hole is "zero-padded," it means that if a C program uses the read()
            function to look inside that massive hole, the Linux kernel will feed the program millions
            of 00000000 bytes to fill the gap.

            The software reading the file thinks, "Wow, someone took the time to write 5 million zeros in a row!"
            The gap is "padded" (filled) with zeros.

    3. How is it represented on the Hard Disk?
            If the zeros aren't on the disk, how does the computer remember the hole is there?
                It uses the file's Inode (the metadata index card we discussed earlier).
                A normal file's Inode contains a map pointing to physical blocks on the hard drive.

                For a normal file, the map looks like this:
                    Block 1: Go to Hard Drive Sector #100
                    Block 2: Go to Hard Drive Sector #101
                    Block 3: Go to Hard Drive Sector #102

                When we create a Sparse File with a hole, the operating system writes a
                special "Unallocated" flag in the Inode's map. It looks like this:
                    Block 1 (The word "START"): Go to Hard Drive Sector #100
                    Blocks 2 through 1,000 (The Hole): [UNALLOCATED SPACE] {this jumps using lseek() function without any write}
                    Block 1,001 (The word "END"): Go to Hard Drive Sector #500

    4. What happens when we read it?
            our C program asks to read() Block 1.
            The OS fetches Sector #100 from the disk and gives we "START"
            our C program asks to read() Block 2.
            The OS checks the Inode map. It sees [UNALLOCATED SPACE].
            The OS does not even talk to the hard drive. It intercepts our request,
            instantly generates a block of 00000000 bytes in the computer's RAM,
            and hands it to our program.
            our program asks to read() Block 1,001.
            The OS fetches Sector #500 from the disk and gives we "END".
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    // 1. Create a new file for writing
    int fd = open("sparse_file.bin", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    printf("--- Creating a Sparse File ---\n");

    // 2. Write data at the very beginning (Byte 0)
    write(fd, "START", 5);
    printf("Wrote 'START' at the beginning of the file.\n");

    // 3. The Magic Jump: Seek 5 MB past the end of the file!
    // 5 MB = 5 * 1024 * 1024 bytes = 5,242,880 bytes
    off_t massive_jump = 5 * 1024 * 1024;

    if (lseek(fd, massive_jump, SEEK_END) == (off_t)-1) {
        perror("Failed to jump");
        close(fd);
        return EXIT_FAILURE;
    }
    printf("Jumped 5 Megabytes into the void...\n");

    // 4. Write data at the new location to "lock in" the hole
    // If we don't write anything, the OS won't stretch the file.
    write(fd, "END", 3);
    printf("Wrote 'END' at the new location.\n");

    close(fd);

    printf("\n[SUCCESS] File 'sparse_file.bin' created.\n");
    printf("To the system, it looks like 5MB, but it takes up almost no physical disk space!\n");

    return EXIT_SUCCESS;
}

/**
    For above program after execution,The below is a perfect terminal output!
    We have just successfully proven the existence of the "File Hole" on our hard drive.

    Here is exactly how to read those terminal outputs, the math behind them,
    and how they map to the Fake Size (Logical Size) versus the True Size (Physical Size).

    1. The du -b Command (Showing the Fake Size)
        Our Output: 5242888 sparse_file.bin

       What it means: The du command stands for "Disk Usage".
       Normally, it shows physical disk space. However, because we used the -b (bytes) flag,
       we explicitly asked the command to show the Apparent Size (the fake size) in exactly
       how many bytes long the file claims to be.

        The Math: Why exactly 5,242,888?
        START = 5 bytes
        The Hole = 5,242,880 bytes
        END = 3 bytes
        Total = 5,242,888 bytes.

        Verdict: This is the Fake Size. If a C program opens this file and reads
        it from start to finish, the OS will feed it exactly 5,242,888 bytes.

    2. The ls -ls --si Command (Showing Both Sizes!)
        Our Output: 8.2k -rw-r--r-- 1 usr usr 5.3M Sep 16 14:37 sparse_file.bin

        This command is the ultimate proof of sparse files because it prints both sizes side-by-side.
        ls = list directory contents.
        -l = long format (shows permissions, owner, size).
        -s = print the allocated size (physical blocks) in the very first column.
        --si = make the numbers human-readable (like K and M).
        Let's look at the two different size numbers in our output:

        A. The 5.3M (The Fake Size)
            This is the standard size column that ls always prints.
            Because we used --si, it rounded our 5.3 million bytes as 5.3 Megabytes.
            To the rest of the operating system, this file is a massive 5.3 MB file.

        B. The 8.2k (The True Physical Size)
            Because we used the -s flag, the very first column (8.2k) reveals
            the True Size physically taken up on our SSD/Hard Drive.

            The Math: Most Linux filesystems format the hard drive
                      into "blocks" that are 4 Kilobytes (4k) in size.
                      The OS needed one block to store the word "START",
                      and one block 100 Megabytes away to store the word "END".

            4k + 4k = 8k. Add a tiny bit of metadata, and we get exactly 8.2 Kilobytes.

     3. Overall Summary of the Magic:
            We told the computer to create a file with 5.3 Megabytes of data in it.
                Fake Size: The OS wrote down in its directory index that the file is 105M.
                True Size: The OS actually only spent 8.2k of physical hard drive space to create it.

        We saved 99.99% of our disk space by using a sparse file hole!
        If we didn't use lseek() to jump the gap and instead wrote 100 million zeros manually using write(),
        both columns in our ls command would have read 105M.

    4. Real-World Usage Scenarios:;
        Virtual Machines (VirtualBox, VMware):
            When we create a 50GB virtual hard drive for a new Virtual Machine,
            the creation is instant, and it only takes up a few megabytes on our host computer.
            Why? Because the software created a massive sparse file with a 50GB hole in it.
            As the VM writes files over time, the holes are slowly filled in with real data.

        BitTorrent / Downloading:
            When we start downloading a 5GB movie, the torrent client instantly creates a 5GB file.
            It does this by lseeking 5GB ahead and writing a single byte. It creates a massive hole,
            reserving the file layout without actually wasting time writing 5 gigabytes of zeros to our disk.
            It then fills the holes as the downloaded pieces arrive.
 */