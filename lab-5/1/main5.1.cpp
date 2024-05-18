#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#define log_error() fprintf(stderr, "%s: unexpected error.\nLine: %d\nError: %s\n", __FILE__, __LINE__, strerror(errno))


int my_global_var = 3;

int main() {
    int my_local_var = 5;

    printf("pid = %d\n", getpid());

//    sleep(15);

    printf("my_local_var:\taddress = %p, value = %d\n", &my_local_var, my_local_var);
    printf("my_global_var:\taddress = %p, value = %d\n\n\n", &my_global_var, my_global_var);

    pid_t child_pid = fork();
    if (child_pid < 0) {
        log_error();
    } else if (child_pid == 0) {
        printf("child: my_local_var:\taddress = %p, value = %d\n", &my_local_var, my_local_var);
        printf("child: my_global_var:\taddress = %p, value = %d\n", &my_global_var, my_global_var);
        printf("child: pid = %d, parent pid = %d\n", getpid(), getppid());

        printf("child: vars were changed\n");
        my_local_var = 123;
        my_global_var = 777;

        printf("child: my_local_var:\taddress = %p, value = %d\n", &my_local_var, my_local_var);
        printf("child: my_global_var:\taddress = %p, value = %d\n", &my_global_var, my_global_var);
        printf("child: pid = %d, parent pid = %d\n", getpid(), getppid());

        exit(5);

//        printf("\n");
    } else if (child_pid > 0){
        sleep(1);
        printf("parent: my_local_var:\taddress = %p, value = %d\n", &my_local_var, my_local_var);
        printf("parent: my_global_var:\taddress = %p, value = %d\n", &my_global_var, my_global_var);
        printf("parent: pid = %d, parent pid = %d\n", getpid(), getppid());

        sleep(30);
        int status;
        wait(&status);
        printf("parent: exit status of child proc = %d\n", WEXITSTATUS(status));

    }
//    printf("new pid = %d, parent pid = %d\n", getpid(), getppid());

}
