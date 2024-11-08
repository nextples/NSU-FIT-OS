#ifndef LIST_H
#define LIST_H

#include <pthread.h>

#define MAX_STRING_LENGTH 100

typedef struct node_t {
    char val[MAX_STRING_LENGTH];
    struct node_t *next;
    pthread_spinlock_t sync;
} node_t;

typedef struct linked_list_t {
    node_t *first;
} list_t;

node_t *create_node(char *val);

void linked_list_destroy(list_t *list);

#endif //LIST_H