#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

struct test_t {
    int num;
    char* str;
};

void* thread_routine(void* arg) {
    struct test_t* test_arg = (struct test_t*) arg;

    printf("Thread routine\n");
    printf("thread_routine [%d %d %d %ld]: Data: [num=%d; str=%s]\n",
                getpid(), getppid(), gettid(), pthread_self(), test_arg->num, test_arg->str);
    return NULL;
}

int main() {
    pthread_t thread;
    pthread_attr_t attr;

    struct test_t* test_arg = (struct test_t*) malloc(sizeof(struct test_t));
    test_arg->num = 111;
    test_arg->str = strdup("Hello World!");;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&thread, &attr, thread_routine, (void *) test_arg);

    sleep(5);
    free(test_arg);
    pthread_attr_destroy(&attr);
    return 0;
}
