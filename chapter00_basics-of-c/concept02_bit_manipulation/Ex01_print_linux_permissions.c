#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

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
    if(octal_value < 0) {
        printf("[ERROR] Invalid octal value!!\n");
        return false;
    }
    if (octal_value >> 4 == 0) {
        return true;
    }
    printf("[ERROR] Incorrect octal value!! It should be 0000 to 0777.\n");
    return false;
}


int main() {
    unsigned int option;
    printf("Options:\n"
           "1 : For  Octal (Numeric) Representation (like 0777)\n"
           "2 : For Exit \n");
    printf("Enter the option: ");
    scanf("%d", &option);

    while(option >= 1 && option < 3) {
        switch(option) {
            case 1: {
                int octal_value;
                printf("Enter the octal value: ");
                scanf("%d", &octal_value);
                if (!octal_count_checker(octal_value)) {
                    break;
                }
                printf(" Symbolic Representation: ");
                print_linux_permissions(octal_value);


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
           "1 : For  Octal (Numeric) Representation (like 0777)\n"
           "2 : For Exit \n");
        printf("Enter the option: ");
        scanf("%d", &option);
    }
    //printf("Enter the linux permission number: \n"); // 0755
   // print_linux_permissions(0755); // rwxr-xr-x

    return EXIT_SUCCESS;
}