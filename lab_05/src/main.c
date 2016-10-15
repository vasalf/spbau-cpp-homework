#include "position.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// Operations with files in text format

void loadtext(intrusive_list *l, const char *filename) {
    FILE *f = fopen(filename, "rt");
    int a, b;
    while (fscanf(f, "%d %d", &a, &b) == 2) {
        add_position(l, a, b);
    }
    fclose(f);
}

void applyable_show(intrusive_node *node, void *data) {
    FILE *f = data;
    position_node *pnode = get_position(node);
    fprintf(f, "%d %d\n", pnode->x, pnode->y);
}

void savetext(intrusive_list *l, const char *filename) {
    FILE *f = fopen(filename, "wt");
    assert(f);
    apply(l, applyable_show, f);
    fclose(f);    
}

// Operations with files in binary format

/*
 * That makes storage of ints in binary format really easy.
 */
union binary_int {
    int x;
    uint8_t data[4];
};

void normalize(union binary_int * bi) {
    if ((((bi->x) >> 23) & 1) == 0)
        bi->data[3] = 0x00;
    else
        bi->data[3] = 0xff;
}

void loadbin(intrusive_list *l, const char *filename) {
    FILE *f = fopen(filename, "rb");
    union binary_int a, b;
    while (fread(a.data, 1, 3, f) == 3) {
        fread(b.data, 1, 3, f);
        normalize(&a);
        normalize(&b);
        add_position(l, a.x, b.x); 
    }
    fclose(f);
}

void applyable_showbin(intrusive_node *nd, void *data) {
    FILE *f = data;
    position_node *pnode = get_position(nd);
    union binary_int bix, biy;
    bix.x = pnode->x;
    biy.x = pnode->y;
    fwrite(bix.data, 3, 1, f);
    fwrite(biy.data, 3, 1, f);
}

void savebin(intrusive_list *l, const char *filename) {
    FILE *f = fopen(filename, "wb");
    apply(l, applyable_showbin, f);
    fclose(f);
}

// print operation

void applyable_print(intrusive_node *nd, void *data) {
    const char *format = data;
    position_node *node = get_position(nd);
    printf(format, node->x, node->y);
}

void print(intrusive_list *l, char *fmt) {
    apply(l, applyable_print, fmt);
}

// count operation

void applyable_count(__attribute__((unused))intrusive_node *nd, void *data) {
    int *x = data;
    (*x)++;
}

void count(intrusive_list *l) {
    int t = 0;
    apply(l, applyable_count, &t);
    printf("%d\n", t);
}

int main(__attribute__((unused)) int argc, char **argv) {
    intrusive_list list;
    intrusive_list *l = &list;
    init_list(l);

    if (strcmp(argv[1], "loadtext") == 0)
        loadtext(l, argv[2]);
    else
        loadbin(l, argv[2]);

    if (strcmp(argv[3], "savetext") == 0) 
        savetext(l, argv[4]);
    else if (strcmp(argv[3], "savebin") == 0)
        savebin(l, argv[4]);
    else if (strcmp(argv[3], "print") == 0)
        print(l, argv[4]);
    else
        count(l);

    remove_all(l); 

    return 0;
}
