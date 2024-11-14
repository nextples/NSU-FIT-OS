#ifndef MY_MUTEX_H
#define MY_MUTEX_H

#include <stdatomic.h>

typedef struct _mutex {
    int lock;
    int tid;
} my_mutex_t;

void my_mutex_init(my_mutex_t *m);
void my_mutex_lock(my_mutex_t *m);
void my_mutex_unlock(my_mutex_t *m);

#endif //MY_MUTEX_H
