#include "clist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define container_of(ptr, type, member) ((type*)((char*)(ptr) - offsetof(type, member)))

struct position_node {
    int x, y;
    struct intrusive_node node;
};

void remove_position(struct intrusive_list *list, int x, int y) { // removes all (x, y) pairs
    struct intrusive_node nn;
    for (struct intrusive_node *n = list->head; n != NULL; n = n->next) {
        if (container_of(n, struct position_node, node)->x == x
            && container_of(n, struct position_node, node)->y == y) {
            nn.next = n->next;
            remove_node(list, n);
            free(container_of(n, struct position_node, node));
            n = &nn;
        }
    }
}

void add_position(struct intrusive_list *list, int x, int y) {
    struct position_node *node = malloc(sizeof(struct position_node));
    node->x = x;
    node->y = y;
    add_node(list, &node->node);
}

void show_all_positions(struct intrusive_list *list) {
    for (struct intrusive_node *n = list->head; n != NULL; n = n->next) {
        printf("(%d %d) ", container_of(n, struct position_node, node)-> x,
               container_of(n, struct position_node, node)->y);
    }
    printf("\n");
}

void remove_all_positions(struct intrusive_list *list) {
    struct intrusive_node nn;
    for (struct intrusive_node *n = list->head; n != NULL; n = n->next) {
        nn.next = n->next;
        remove_node(list, n);
        free(container_of(n, struct position_node, node));
        n = &nn;
    }
}

int main() {
    /* usage

    struct intrusive_list_t l;
    init_list(&l);

    add_positon(&l, 10, 10);
    add_positon(&l, 20, 20);

    show_all_positions(&l);

    return 0;
    */

    struct intrusive_list list;

    init_list(&list);

    char buf[10];
    for (;;) {
        if (scanf("%s", buf) != 1) {
            printf("Unknown command\n");
            continue;
        }

        if (strcmp(buf, "add") == 0) {
            int x, y;

            if (scanf("%d %d", &x, &y) != 2) {
                printf("Unknown command\n");
                continue;
            }

            add_position(&list, x, y);
        } else if (strcmp(buf, "rm") == 0) {
            int x, y;

            if (scanf("%d %d", &x, &y) != 2) {
                printf("Unknown command\n");
                continue;
            }
        
            remove_position(&list, x, y);
        } else if (strcmp(buf, "len") == 0) {
            printf("%d\n", get_length(list));
        } else if (strcmp(buf, "print") == 0) {
            show_all_positions(&list);
        } else if (strcmp(buf, "rma") == 0) {
            remove_all_positions(&list);
        } else if (strcmp(buf, "exit") == 0) {
            remove_all_positions(&list);
            return 0;
        } else {
            printf("Unknown command\n");
        }
    }

    /* unreachable point */
    return 0;
}
