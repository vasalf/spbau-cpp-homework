#include "clist.h"

#include <stddef.h>

void init_list(struct intrusive_list *a) {
    a->head = NULL;
}

void add_node(struct intrusive_list *list, struct intrusive_node *node) {
    node->prev = NULL;
    node->next = list->head;
    if (list->head)
        list->head->prev = node;
    list->head = node;
}

void remove_node(struct intrusive_list *list, struct intrusive_node *node) {
    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;
    if (node->next)
        node->next->prev = node->prev;
}

int get_length(struct intrusive_list list) {
    int length = 0;
    for (struct intrusive_node *n = list.head; n != NULL; n = n->next)
        length++;
    return length;
}
