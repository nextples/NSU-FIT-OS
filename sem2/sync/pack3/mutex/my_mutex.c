#define _GNU_SOURCE
#include <errno.h>
#include <linux/futex.h>
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>

#include "my_mutex.h"


#define STATUS_LOCK 0
#define STATUS_UNLOCK 1
#define NO_TID -1

int futex(int *uaddr, int futex_op, int val, const struct timespec *timeout, int *uaddr2, int val3) {
    return syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3);
}

int futex_wait(int *futex_addr) {
    int err = futex(futex_addr, FUTEX_WAIT, STATUS_LOCK, NULL, NULL, 0);
    if (err == -1 && errno != EAGAIN) {
        printf("futex_wait [%d %d %d]: FUTEX_WAIT error: %s\n", getpid(), getppid(), gettid(),
               strerror(errno));
        return 1;
    }
    return 0;
}

int futex_wake(int *futex_addr) {
    int futex_rc = futex(futex_addr, FUTEX_WAKE, 1, NULL, NULL, 0);
    if (futex_rc == -1 && errno != EAGAIN) {
        printf("futex_wake [%d %d %d]: FUTEX_WAKE error: %s\n", getpid(), getppid(), gettid(),
               strerror(errno));
        return 1;
    }
    return 0;
}

void my_mutex_init(my_mutex_t *m) {
    m->lock = 1;
    m->tid = NO_TID;
}

void my_mutex_lock(my_mutex_t *m) {
    while (1) {
        int expected = STATUS_UNLOCK;
        if (atomic_compare_exchange_strong(&m->lock, &expected, STATUS_LOCK))
            break;
        futex_wait(m);     // Если не смогли захватить лок, то уходим в слип
    }
    m->tid = gettid();
}

void my_mutex_unlock(my_mutex_t *m) {
    int expected = STATUS_LOCK;
    if (m->tid != gettid()) {
        printf("mutex_unlock [%d %d %d]: not access\n", getpid(), getppid(), gettid());
        exit(1);
    }
    if (atomic_compare_exchange_strong(&m->lock, &expected, STATUS_UNLOCK)) {
        futex_wake(m);      // Будим потоки, спящие на данном мьютексе
    }
}
