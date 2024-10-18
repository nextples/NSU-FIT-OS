#define _GNU_SOURCE
#include "mythread.h"
#include <errno.h>
#include <sys/wait.h>
#include <string.h>

void print_mythread(mythread_t* thread) {
     printf("mythread_t {\n\ttid: %d,\n\tstack: %p,\n\troutine: %p,\n\targ: %p,\n\tret: %p,\n\texited: %d,\n\tjoined: %d\n\tdetached: %d\n}\n",
         thread->tid, thread->stack, thread->routine, thread->arg, thread->ret, thread->exited, thread->joined, thread->detached);
}

// Запуск функции потока
int mythread_startup(void* arg) {
    mythread_t* thread = (mythread_t*)arg;

    printf("mythread_startup [%d]: starting thread func...\n", thread->tid);
    // print_mythread(thread);

    // Выполнение основной функции потока
    thread->ret = thread->routine(thread->arg);
    thread->exited = 1;

    printf("mythread_startup [%d]: terminating thread func...\n", thread->tid);
    // print_mythread(thread);

    if (thread->detached == DETACHED) {
        printf("detached free\n");
        munmap(thread->stack, STACK_SIZE);
        munmap(thread, sizeof(mythread_t));
    }

    // print_mythread(thread);
    while (!thread->joined) {
        printf("sleep....\n");
        sleep(1);
    }
    print_mythread(thread);
    printf("mythread_startup [%d]: thread joined\n", thread->tid);

    return 0;
}

int mythread_create(mythread_t* thread, mythread_routine_t routine, void* arg, int type) {
    static int my_tid = 1;

    thread->stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (thread->stack == MAP_FAILED) {
        perror("Fail to map stack");
        return -1;
    }

    thread->tid = my_tid++;
    thread->routine = routine;
    thread->arg = arg;
    thread->exited = 0;
    thread->detached = type;
    thread->joined = 0;

    mythread_t* new_thread = thread->stack + STACK_SIZE - sizeof(mythread_t);
    *new_thread = *thread;

    int child_pid = clone(mythread_startup, thread->stack + STACK_SIZE - sizeof(mythread_t),
                          CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD, (void*)new_thread);

    if (child_pid == -1) {
        perror("Fail to clone thread");
        munmap(thread->stack, STACK_SIZE);
        return -1;
    }

    return 0;
}

int mythread_join(mythread_t* thread, void** ret) {
    if (thread->detached) {
        printf("Join cannot be expected for a detached thread\n");
        return -1;
    }

    mythread_t* new_thread = (thread->stack + STACK_SIZE - sizeof(mythread_t));

    while (!new_thread->exited) {
        usleep(200000);     // 0.2 sec
        printf("mythread_join [%d]: wait..\n", new_thread->tid);
    }

    *thread = *new_thread;
    if (ret != NULL) {
        *ret = thread->ret;
    }

    thread->joined = 1;

    printf("joinable free\n");
    if (munmap(thread->stack, STACK_SIZE) == -1) {
        perror("Fail to unmap stack\n");
        return -1;
    }


    printf("CHECK\n");
    return 0;
}

int mythread_detach(mythread_t* thread) {
    if (thread->detached == DETACHED) {
        perror("Thread is detached already\n");
        return -1;
    }
    thread->detached = DETACHED;
    return 0;
}
