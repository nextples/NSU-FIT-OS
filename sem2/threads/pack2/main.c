#include "syscall.h"

#include "mythread.h"

void* example_routine1(void* arg) {
    printf("First example routine...\n");
    return NULL;
}

void* example_routine2(void* arg) {
    printf("Second example routine...\n");
    return NULL;
}

void*  example_routine3(void* arg) {
    for (int i = 0; i < 100; i++) {
        printf("%d\n", i);
        sleep(1);
    }
    return NULL;
}

int main() {
    // mythread_t my_kernel_thread1;
    // mythread_t my_kernel_thread2;


    // int err = mythread_create(&my_kernel_thread1, example_routine1, NULL, JOINABLE);
    // if (err == -1) {
        // perror("Fail to create mythread");
        // return -1;
    // }
    // printf("main: create thread %d\n", my_kernel_thread1.tid);


    // err = mythread_create(&my_kernel_thread2, example_routine2, NULL, DETACHED);
    // if (err == -1) {
        // perror("Fail to create mythread");
        // return -1;
    // }
    // printf("main: create thread %d\n", my_kernel_thread2.tid);


    // err = mythread_join(&my_kernel_thread1, NULL);
    // if (err == -1) {
        // printf("Fail to join mythread");
        // return -1;
    // }
    // printf("main: join thread %d\n", my_kernel_thread1.tid);

    mythread_t my_kernel_thread3;
    int err = mythread_create(&my_kernel_thread3, example_routine3, NULL, JOINABLE);
    if (err == -1) {
        perror("Fail to create mythread");
    }

    sleep(5);

    mythread_cancel(&my_kernel_thread3);
    sleep(5);
    print_mythread(&my_kernel_thread3);

    syscall(SYS_exit, 0);
}
