/**
 *
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

// Forward declarations
bool octal_count_checker(int octal_value);

static void print_linux_permissions(mode_t mode) {
    printf("Owner: %c%c%c\n",
    (mode & S_IRUSR) ? 'r' : '-',
    (mode & S_IWUSR) ? 'w' : '-',
    (mode & S_IXUSR) ? 'x' : '-');
    printf("Group: %c%c%c\n",
    (mode & S_IRGRP) ? 'r' : '-',
    (mode & S_IWGRP) ? 'w' : '-',
    (mode & S_IXGRP) ? 'x' : '-');
    printf("Other: %c%c%c\n",
    (mode & S_IROTH) ? 'r' : '-',
    (mode & S_IWOTH) ? 'w' : '-',
    (mode & S_IXOTH) ? 'x' : '-');
}

bool octal_count_checker(int octal_value) {
    // Check for negative values
    if (octal_value < 0) {
        printf("[ERROR] Invalid octal value: negative number is not allowed.\n");
        return false;
    }
    // Check range: must be 0000 to 0777 (0 to 511 in decimal)
    // Since scanf("%o") already parses input as octal, any value in [0, 0777]
    // is guaranteed to have valid octal digits (0-7).
    if (octal_value > 0777) {
        printf("[ERROR] Incorrect octal value: %04o exceeds 0777.\n", octal_value);
        return false;
    }
    return true;
}

int main() {
    unsigned int option;
    int fail_count = 0;           // consecutive failures in case 1
    int threshold_exceeded_count = 0;  // how many times threshold was exceeded
    const int THRESHOLD_LIMIT = 3;

    printf("Options:\n"
           "1 : For Octal (Numeric) Representation (like 0777)\n"
           "2 : For Exit \n");
    printf("Enter the option: ");
    scanf("%d", &option);

    while(option >= 1 && option < 3) {
        switch(option) {
            case 1: {
                int octal_value;
                printf("Enter the octal value (e.g. 755): ");
                if (scanf("%o", &octal_value) != 1) {
                    printf("[ERROR] Invalid input. Please enter a valid octal number.\n");
                    while (getchar() != '\n'); // clear input buffer
                    fail_count++;
                    if (fail_count >= THRESHOLD_LIMIT) {
                        threshold_exceeded_count++;
                        if (threshold_exceeded_count >= 2) {
                            printf("[FATAL] Threshold exceeded again. Exiting automatically.\n");
                            return EXIT_FAILURE;
                        }
                        printf("[WARNING] Threshold exceeded (%d failed attempts). Returning to menu.\n", THRESHOLD_LIMIT);
                        fail_count = 0;
                    }
                    break;
                }
                if (!octal_count_checker(octal_value)) {
                    fail_count++;
                    if (fail_count >= THRESHOLD_LIMIT) {
                        threshold_exceeded_count++;
                        if (threshold_exceeded_count >= 2) {
                            printf("[FATAL] Threshold exceeded again. Exiting automatically.\n");
                            return EXIT_FAILURE;
                        }
                        printf("[WARNING] Threshold exceeded (%d failed attempts). Returning to menu.\n", THRESHOLD_LIMIT);
                        fail_count = 0;
                    }
                    break;
                }
                // Success: reset failure counter
                fail_count = 0;
                printf(" Symbolic Representation: ");
                print_linux_permissions((mode_t)octal_value);
                break;
            }
            case 2: {
                printf("Exiting...\n");
                option = 3;
                break;
            }
            default: printf("Issue Occurred: Switch Logical Failed!!\n");
        }
        printf("Options:\n"
           "1 : For Octal (Numeric) Representation (like 0777)\n"
           "2 : For Exit \n");
        printf("Enter the option: ");
        scanf("%d", &option);
    }

    return EXIT_SUCCESS;
}