#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

static void print_permissions(mode_t mode) {
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

int main() {
    print_permissions(0755); // rwxr-xr-x
    print_permissions(0644); // rw-r--r--

    return EXIT_SUCCESS;
}