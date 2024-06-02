#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PAGE_SIZE 4096
#define FIFO_NAME "my_fifo"

int main() {
    int err = mkfifo(FIFO_NAME, 0666);
    if (err == -1 && errno != EEXIST) {
        printf("Failure in creating named pipe. %s\n", strerror(errno));
        return -1;
    }

    int fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        printf("Failure in opening named pipe for writing. %s\n", strerror(errno));
        return -1;
    }

    unsigned int val = 0;

    while (1) {
        for (int i = 0; i < PAGE_SIZE / sizeof(unsigned int); i++) {
            write(fd, &val, sizeof(unsigned int));
            val++;
//            sleep(1);
        }
    }

    return 0;
}
