#define _GNU_SOURCE
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>

#include "list.h"

#define THREAD_COUNT 6
#define STORAGE_SIZE 100000

void set_cpu(int n) {
    int err;
    cpu_set_t cpuset;
    pthread_t tid = pthread_self();

    CPU_ZERO(&cpuset);
    CPU_SET(n, &cpuset);

    err = pthread_setaffinity_np(tid, sizeof(cpu_set_t), &cpuset);
    if (err) {
        printf("set_cpu: pthread_setaffinity failed for cpu %d\n", n);
        return;
    }

    printf("set_cpu: set cpu %d\n", n);
}

int ascending_count = 0;
int ascending_iter = 0;

void *ascending_routine(void *args) {
    set_cpu(4);
    list_t *list = (list_t *) args;

    while (true) {
        pthread_rwlock_rdlock(&list->first->sync);
        node_t *prev = list->first, *cur;

        while (prev->next != NULL) {
            cur = prev->next;
            int prev_size = strlen(prev->val);
            pthread_rwlock_rdlock(&cur->sync);
            pthread_rwlock_unlock(&prev->sync);

            if (prev_size < strlen(cur->val)) {
                ascending_count++;
            }
            prev = cur;
        }

        pthread_rwlock_unlock(&prev->sync);
        ascending_iter++;
    }
}


int descending_count = 0;
int descending_iter = 0;

void *descending_routine(void *args) {
    set_cpu(5);
    list_t *list = (list_t *) args;

    while (true) {
        pthread_rwlock_rdlock(&list->first->sync);
        node_t *prev = list->first, *cur;
        
        while (prev->next != NULL) {
            cur = prev->next;
            int prev_size = strlen(prev->val);
            pthread_rwlock_rdlock(&cur->sync);
            pthread_rwlock_unlock(&prev->sync);

            if (prev_size > strlen(cur->val)) {
                descending_count++;
            }
            prev = cur;
        }

        pthread_rwlock_unlock(&prev->sync);
        descending_iter++;
    }
}


int equal_count = 0;
int equal_iter = 0;

void *equal_routine(void *args) {
    set_cpu(6);
    list_t *list = (list_t *) args;

    while (true) {
        pthread_rwlock_rdlock(&list->first->sync);
        node_t *prev = list->first, *cur;

        while (prev->next != NULL) {
            cur = prev->next;
            int prev_size = strlen(prev->val);
            pthread_rwlock_rdlock(&cur->sync);
            pthread_rwlock_unlock(&prev->sync);

            if (prev_size == strlen(cur->val)) {
                equal_count++;
            }
            prev = cur;
        }
        pthread_rwlock_unlock(&prev->sync);
        equal_iter++;
    }
}

pthread_mutex_t swap_mutex;
int swap_count = 0;
int swap_iter = 0;

void *swap_routine(void *args) {
    set_cpu(7);
    list_t *list = (list_t *) args;

    while (true) {
        pthread_rwlock_wrlock(&list->first->sync);
        node_t *prev = list->first, *cur, *next;

        while (prev->next != NULL) {
            if (rand() % 10 != 0) {    // swap chance = 10%
                cur = prev->next;
                pthread_rwlock_wrlock(&cur->sync);
                pthread_rwlock_unlock(&prev->sync);
                prev = cur;
                continue;
            }

            cur = prev->next;
            pthread_rwlock_wrlock(&cur->sync);

            next = cur->next;
            if (next == NULL) {
                pthread_rwlock_unlock(&cur->sync);
                break;
            }

            pthread_rwlock_wrlock(&next->sync);
            prev->next = next;
            pthread_rwlock_unlock(&prev->sync);
            cur->next = next->next;
            pthread_rwlock_unlock(&cur->sync);
            next->next = cur;
            swap_count++;
            prev = next;
        }
        pthread_rwlock_unlock(&prev->sync);

        pthread_mutex_lock(&swap_mutex);
        swap_iter++;
        pthread_mutex_unlock(&swap_mutex);
    }
}


void *print_routine(void *args) {
    while (true) {
        sleep(1);
        printf("asc: %d/%d,\t\tdesc: %d/%d,\t\teq: %d/%d,\t\tswap: %d/%d\n", ascending_count, ascending_iter, descending_count, descending_iter,
               equal_count, equal_iter, swap_count, swap_iter);

    }
}


int main() {
    printf("RWLOCK, SIZE = %d\n", STORAGE_SIZE);

    list_t *list = malloc(sizeof(list_t));

    srand(time(NULL));
    char val[MAX_STRING_LENGTH];
    memset(val, 0, MAX_STRING_LENGTH);
    memset(val, '0', rand() % MAX_STRING_LENGTH);
    list->first = create_node(val);

    node_t *last = list->first;
    for (int i = 1; i < STORAGE_SIZE; ++i) {
        memset(val, 0, MAX_STRING_LENGTH);
        memset(val, '0', rand() % MAX_STRING_LENGTH);
        node_t *new_node = create_node(val);
        last->next = new_node;
        last = new_node;
    }

    pthread_t tid[THREAD_COUNT];

    int err = pthread_create(&tid[0], NULL, ascending_routine, list);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }
    err = pthread_create(&tid[1], NULL, descending_routine, list);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }
    err = pthread_create(&tid[2], NULL, equal_routine, list);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }
    err = pthread_create(&tid[3], NULL, print_routine, NULL);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }

    pthread_mutex_init(&swap_mutex, NULL);
    for (int i = 0; i < 3; ++i) {
        err = pthread_create(&tid[4 + i], NULL, swap_routine, list);
        if (err) {
            printf("main: pthread_create() failed: %s\n", strerror(err));
            return -1;
        }
    }

    sleep(100);

    for (int i = 0; i < THREAD_COUNT; ++i) {
        pthread_cancel(tid[i]);
    }

    linked_list_destroy(list);
    free(list);
}