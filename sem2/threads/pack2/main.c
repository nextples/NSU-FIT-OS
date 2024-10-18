#include <pthread.h>
#include <stdio.h>
#include "mythread.h"

void* example_routine1(void* arg) {
    printf("First example routine...\n");
    return NULL;
}

void* example_routine2(void* arg) {
    printf("Second example routine...\n");
    sleep(3);
    return NULL;
}

int main() {
    mythread_t my_kernel_thread1;
    mythread_t my_kernel_thread2;


    int err = mythread_create(&my_kernel_thread1, example_routine1, NULL, JOINABLE);
    if (err == -1) {
        perror("Fail to create mythread");
        return -1;
    }
    printf("main: create thread %d\n", my_kernel_thread1.tid);


    err = mythread_create(&my_kernel_thread2, example_routine2, NULL, DETACHED);
    if (err == -1) {
        perror("Fail to create mythread");
        return -1;
    }
    printf("main: create thread %d\n", my_kernel_thread2.tid);


    err = mythread_join(&my_kernel_thread1, NULL);
    if (err == -1) {
        printf("Fail to join mythread");
        return -1;
    }
    printf("main: join thread %d\n", my_kernel_thread2.tid);

    pthread_exit(NULL);
}
