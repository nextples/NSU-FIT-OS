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

    int fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        printf("Failure in opening named pipe for reading. %s\n", strerror(errno));
        return -1;
    }

    unsigned int prev_val = -1;
    unsigned int cur_val = 0;

    while (true) {
        for (int i = 0; i < PAGE_SIZE / sizeof(unsigned int); i++) {
            read(fd, &cur_val, sizeof(unsigned int));
            if (cur_val - prev_val != 1) {
                printf("\nFailure in the sequence between %u and %u\n", prev_val, cur_val);
            }
            printf("%u ", cur_val);
            fflush(stdout);
            prev_val = cur_val;
            sleep(1);
        }
    }

    return 0;
}
