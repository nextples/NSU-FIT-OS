#include "my_spinlock.h"

#include <stdbool.h>

#define STATUS_LOCK 0
#define STATUS_UNLOCK 1

void my_spinlock_init(my_spinlock_t* s){
    *s = STATUS_UNLOCK;
}

void my_spinlock_lock(my_spinlock_t* s) {
    while(true) {
        int expected = STATUS_UNLOCK;
        if(atomic_compare_exchange_strong(s, &expected, STATUS_LOCK))
            break;
    }
}

void my_spinlock_unlock(my_spinlock_t *s) {
    int expected = STATUS_LOCK;
    atomic_compare_exchange_strong(s, &expected, STATUS_UNLOCK);
}