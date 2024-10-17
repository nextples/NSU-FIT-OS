#include <cerrno>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void *thread_routine(void *arg) {
    printf("int_routine [%d %d %d %ld]: Working\n", getpid(), getppid(), gettid(), pthread_self());
    // pthread_detach(pthread_self());
    return NULL;
}

int main() {
    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_t tid;
    // while (1) {
    //     pthread_create(&tid, &attr, thread_routine, NULL);
    //     pthread_join(tid, NULL);
    // }

    pthread_create(&tid, &attr, thread_routine, NULL);

    int res = pthread_join(tid, NULL);
    if (res == EINVAL) {
        printf("hui\n");
    }
    sleep(1);
    pthread_attr_destroy(&attr);
    return 0;
}