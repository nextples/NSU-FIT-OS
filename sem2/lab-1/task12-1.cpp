#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void *int_routine(void *arg) {
    // Вариант 1 с ошибкой
    // int res = 42;
    // printf("int_routine [%d %d %d %ld]: Return %d\n", getpid(), getppid(), gettid(), pthread_self(), res);
    // return (void *) res;

    // Вариант 2 с динамическим выделением памяти
    int *res = (int *) malloc(sizeof(int));
    *res = 42;
    printf("int_routine [%d %d %d %ld]: Return %d\n", getpid(), getppid(), gettid(), pthread_self(), *res);
    return res;
}

void *string_routine(void *arg) {
    // char* str = "Hello, World";
    char *str = (char *)malloc(14);
    strcpy(str, "Hello, World!");

    printf("string_routine [%d %d %d %ld]: Return %s\n", getpid(), getppid(), gettid(), pthread_self(), str);

    return str;
}

int main() {
    pthread_t tid;
    // Вариант 1.2.b
    // pthread_create(&tid, NULL, int_routine, NULL);

    // Вариант 1.2.c
    pthread_create(&tid, NULL, string_routine, NULL);

    printf("main [%d %d %d]: create thread %ld\n", getpid(), getppid(), gettid(), tid);

    // Дождаться завершения потока
    void* res;
    pthread_join(tid, &res);

    // Вариант 1.2.b
    // printf("main [%d %d %d]: int_routine return %d\n", getpid(), getppid(), gettid(), *(int*)res);

    // Вариант 1.2.c
    printf("main [%d %d %d]: string_routine return %s\n", getpid(), getppid(), gettid(), (char*)res);

    free(res);

    return 0;
}