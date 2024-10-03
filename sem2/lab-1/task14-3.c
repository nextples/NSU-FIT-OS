#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

void cleanup(void *arg) {
    printf("clenaup [%d %d %d]: Running\n", getpid(), getppid(), gettid());
    free(arg);
}

void *print_hello_world(void *arg) {
    char *str = (char*) malloc(14);
    strcpy(str, "Hello, World!");

    pthread_cleanup_push(cleanup, str);

    while (true) {
        printf("print_hello_world [%d %d %d %ld]: str = %s\n", getpid(), getppid(), gettid(), pthread_self(), str);
    }

    pthread_cleanup_pop(1);

    return NULL;
}

int main() {
    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

    pthread_t tid;
    pthread_create(&tid, NULL, print_hello_world, NULL);
    printf("main [%d %d %d]: Create thread %ld\n", getpid(), getppid(), gettid(), tid);

    sleep(3);

    pthread_cancel(tid);
    printf("main [%d %d %d]: Cancel print_hello_world\n", getpid(), getppid(), gettid());

    return 0;
}