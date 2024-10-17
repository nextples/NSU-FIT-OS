#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sched.h>

#define PAGE 4096
#define STACK_SIZE (PAGE * 8)
#define DETACHED 1
#define JOINABLE 0

typedef void* (*mythread_routine_t)(void*);

typedef struct mythread_t {
    int                      tid;        // Идентификатор потока
    void*                    stack;      // Указатель на стек
    mythread_routine_t       routine;    // Функция потока
    void*                    arg;        // Аргумент для функции
    void*                    ret;        // Возвращаемое значение функции
    int                      exited;     // Флаг завершения потока
    int                      detached;   // Флаг для detached потока
} mythread_t;

int mythread_create(mythread_t* thread, mythread_routine_t routine, void* arg, int type);
int mythread_join(mythread_t* thread, void** ret);
int mythread_detach(mythread_t* thread);

#endif // MYTHREAD_H
