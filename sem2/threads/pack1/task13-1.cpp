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

    printf("thread_routine [%d %d %d %ld]: Data: [num=%d; str=%s]\n",
            getpid(), getppid(), gettid(), pthread_self(), test_arg->num, test_arg->str);
    return NULL;
}

int main() {
    pthread_t thread;

    struct test_t test_arg;
    test_arg.num = 111;
    test_arg.str = strdup("Hello World!");

    pthread_create(&thread, NULL, thread_routine, (void*) &test_arg);

    pthread_join(thread, NULL);

    return 0;
}
