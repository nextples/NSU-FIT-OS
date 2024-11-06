#include <stdio.h>
#include <syscall.h>
#include <unistd.h>

#include "mythread.h"

void*  example_routine(void* arg) {
    for (int i = 0; i < 3; i++) {
        printf("%d\n", i);
        sleep(1);
    }
    return NULL;
}

int main() {
    mythread_t my_thread;

    int err = mythread_create(&my_thread, example_routine, NULL, DETACHED);
    if (err == -1) {
        perror("Fail to create mythread");
    }
    syscall(SYS_exit, 0);
}
