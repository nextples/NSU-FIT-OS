#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define log_error() fprintf(stderr, "%s: unexpected error.\nLine: %d\nError: %s\n", __FILE__, __LINE__, strerror(errno))

int my_global_var = 3;

int main() {
    int my_local_var = 5;

    printf("pid = %d\n", getpid());
    printf("my_local_var:\taddress = %p, value = %d\n", &my_local_var, my_local_var);
    printf("my_global_var:\taddress = %p, value = %d\n\n\n", &my_global_var, my_global_var);

    pid_t child_pid = fork();
    if (child_pid < 0) {
        log_error();
        return EXIT_FAILURE;
    }

    else if (child_pid == 0) {
          exit(5);
    }

    else if (child_pid > 0) {
        printf("zombie child process created. pid = %d\n", child_pid);
        sleep(30);

    }
}