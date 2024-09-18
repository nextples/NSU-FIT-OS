#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define PAGE_SIZE 4096

void writer(int fd) {
    unsigned int val = 0;

    while (1) {
        for (int i = 0; i < PAGE_SIZE / sizeof(unsigned int); i++) {
            write(fd, &val, sizeof(unsigned int));
            val++;
//            sleep(1);
        }
    }
}

void reader(int fd) {
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
//            sleep(1);
        }
    }
}

int main() {
    int pipe_fd[2];

    int err = pipe(pipe_fd);
    if (err == -1) {
        printf("Failure in the creating pipe. %s\n", strerror(errno));
        return -1;
    }

    int pid = fork();

    if (pid == 0) {
        reader(pipe_fd[0]);
    } else if (pid > 0) {
        writer(pipe_fd[1]);
    } else {
        printf("Failure in the creating child proc\n");
        return -1;
    }
}