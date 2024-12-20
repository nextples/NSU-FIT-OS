#define _GNU_SOURCE
#include "mythread.h"
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <threads.h>
#include <pthread.h>
#include <ucontext.h>

thread_local mythread_t* current_thread;
thread_local ucontext_t new_context;

void free_detached_thread() {
    if (munmap(current_thread->stack, STACK_SIZE) == -1) {
        perror("Fail to unmap stack\n");
        exit(-1);
    }
    printf("Resources of detached thread were free\n");
    free(new_context.uc_stack.ss_sp);
}

void print_mythread(mythread_t *thread) {
    printf(
        "mythread_t {\n\tmy_tid: %d,\n\tstack: %p,\n\troutine: %p,\n\targ: %p,\n\tret: %p,\n\texited: %d,\n\tdetached: %d,\n\ttid: %d,\n\tcanceled: %d\n}\n",
        thread->my_tid, thread->stack, thread->routine, thread->arg, thread->ret, thread->exited, thread->detached,
        thread->tid, thread->canceled);
}

void mythread_handle_cancel(int sig) {
    printf("mythread_handle_cancel: thread [%d] get signal\n", gettid());

    mythread_t *thread = current_thread;
    if (thread) {
        printf("thread [%d] received cancel signal\n", thread->tid);
        longjmp(thread->state_before_routine, 1);
    }
}

int mythread_startup(void *arg) {
    mythread_t *thread = (mythread_t *) arg;
    current_thread = thread;
    thread->tid = gettid();

    struct sigaction act;
    act.sa_handler = mythread_handle_cancel;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, NULL);

    if (setjmp(thread->state_before_routine) == 0) {
        thread->ret = thread->routine(thread->arg);
    } else {
        printf("thread [%d] canceled before completion\n", thread->tid);
        thread->exited = 1;
        thread->ret = (void*)CANCELED;
    }

    if (thread->detached == DETACHED) {
        getcontext(&new_context);

        char* new_stack = (char*) malloc(PAGE * sizeof(char));

        new_context.uc_stack.ss_sp = new_stack;
        new_context.uc_stack.ss_size = PAGE;
        new_context.uc_stack.ss_flags = 0;
        new_context.uc_link = NULL;

        makecontext(&new_context, free_detached_thread, 0);
        setcontext(&new_context);
    }
    return 0;
}

int mythread_create(mythread_t *thread, mythread_routine_t routine, void *arg, int type) {
    static int my_tid = 1;

    thread->stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (thread->stack == MAP_FAILED) {
        perror("Fail to map stack");
        return -1;
    }

    thread->my_tid = my_tid++;
    thread->routine = routine;
    thread->arg = arg;
    thread->exited = 0;
    thread->detached = type;
    thread->canceled = 0;

    mythread_t *new_thread = thread->stack + STACK_SIZE - sizeof(mythread_t);
    *new_thread = *thread;

    thread->tid = clone(mythread_startup, thread->stack + STACK_SIZE - sizeof(mythread_t),
                        CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD, (void *) new_thread);

    if (thread->tid == -1) {
        perror("Fail to clone thread");
        if (munmap(thread->stack, STACK_SIZE) == -1) {
            perror("Fail to unmap stack");
        }
        return -1;
    }

    return 0;
}

int mythread_join(mythread_t *thread, void **ret) {
    if (thread->detached) {
        printf("Join cannot be expected for a detached thread\n");
        return -1;
    }

    mythread_t *new_thread = thread->stack + STACK_SIZE - sizeof(mythread_t);

    while (!new_thread->exited) {
        usleep(200000); // 0.2 sec
        printf("mythread_join [%d]: wait..\n", new_thread->my_tid);
    }

    *thread = *new_thread;
    if (ret != NULL) {
        *ret = thread->ret;
    }

    if (munmap(thread->stack, STACK_SIZE) == -1) {
        perror("Fail to unmap stack\n");
        return -1;
    }
    printf("unmap\n");

    return 0;
}

int mythread_detach(mythread_t *thread) {
    if (thread->detached == DETACHED) {
        perror("Thread is detached already\n");
        return -1;
    }
    thread->detached = DETACHED;
    return 0;
}

int mythread_cancel(mythread_t *thread) {
    if (thread->exited) {
        printf("Thread [%d] already exited\n", thread->my_tid);
        return -1;
    }

    thread->canceled = 1;
    thread->exited = 1;
    if (kill(thread->tid, SIGUSR1) == -1) {
        perror("Failed to send cancel signal");
        return -1;
    }

    return 0;
}
