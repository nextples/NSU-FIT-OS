#include "list.h"

#include <stdlib.h>
#include <string.h>

node_t *create_node(char *val) {
    node_t *node = malloc(sizeof(node_t));

    strncpy(node->val, val, MAX_STRING_LENGTH);
    node->next = NULL;
    pthread_rwlock_init(&node->sync, NULL);

    return node;
}

void linked_list_destroy(list_t *list) {
    node_t *cur = list->first;
    while (cur->next != NULL) {
        node_t *tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    free(cur);
}