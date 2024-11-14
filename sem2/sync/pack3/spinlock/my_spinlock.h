#ifndef MY_XPINLOCK_H
#define MY_XPINLOCK_H

#include <stdatomic.h>

typedef volatile int my_spinlock_t;

void my_spinlock_init(my_spinlock_t* s);
void my_spinlock_lock(my_spinlock_t* s);
void my_spinlock_unlock(my_spinlock_t *s);


#endif //MY_XPINLOCK_H
