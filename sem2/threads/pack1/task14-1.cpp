#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

void *thread_routine(void *arg) {
    while (true) {
        printf("thread_routine [%d %d %d %ld]: Running\n", getpid(), getppid(), gettid(), pthread_self());
    }

    return NULL;
}

int main() {
    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

    pthread_t tid;
    pthread_create(&tid, NULL, thread_routine, NULL);
    printf("main [%d %d %d]: Create thread %ld\n", getpid(), getppid(), gettid(), tid);

    sleep(3);

    pthread_cancel(tid);
    printf("main [%d %d %d]: Cancel thread_routine\n", getpid(), getppid(), gettid());

    void* res;
    pthread_join(tid, &res);
    printf("main [%d %d %d]: Join thread_routine\n", getpid(), getppid(), gettid());
    if (res == PTHREAD_CANCELED) {
        printf("main [%d %d %d]: Thread was cancelled\n", getpid(), getppid(), gettid());
    }

    return 0;
}