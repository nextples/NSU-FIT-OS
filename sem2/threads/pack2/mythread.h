#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <stdlib.h>
#include <setjmp.h>


#define PAGE 4096
#define STACK_SIZE (PAGE * 8)
#define DETACHED 1
#define JOINABLE 0
#define CANCELED -2

typedef void* (*mythread_routine_t)(void*);

typedef struct mythread_t {
    int                      my_tid;
    void*                    stack;
    mythread_routine_t       routine;
    void*                    arg;
    void*                    ret;
    int                      exited;
    int                      detached;
    pid_t                    tid;
    int                      canceled;

    jmp_buf                  state_before_routine;
    // ucontext_t               context_before_routine;
} mythread_t;

void print_mythread(mythread_t* thread);
int mythread_create(mythread_t* thread, mythread_routine_t routine, void* arg, int type);
int mythread_join(mythread_t* thread, void** ret);
int mythread_detach(mythread_t* thread);
int mythread_cancel(mythread_t* thread);
void mythread_exit(mythread_t* thread, void* retval);

#endif // MYTHREAD_H
