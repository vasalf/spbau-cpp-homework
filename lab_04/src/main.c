#include "mergesort.h"
#include <stdio.h>
#include <string.h>

int intcmp(const void* _a, const void* _b) {
    int a = *((int*)_a), b = *((int*)_b);
    if (a < b)
        return -1;
    else if (a == b)
        return 0;
    else
        return 1;
}

int charcmp(const void *_a, const void *_b) {
    char a = *((char*)_a), b = *((char*)_b);
    if (a < b)
        return -1;
    else if (a == b)
        return 0;
    else
        return 1;
}

int _strcmp(const void *_a, const void *_b) {
    return strcmp(*(char**)_a, *(char**)_b);
}

int main(int argc, char **argv) {
    if (strcmp(argv[1], "int") == 0) {
        int a[argc - 2];
        for (int i = 2; i < argc; i++) {
            sscanf(argv[i], "%d", a + i - 2);
        }
        mergesort(a, argc - 2, sizeof(int), intcmp);
        for (int i = 0; i < argc - 2; i++)
            printf("%d ", a[i]);
        printf("\n");
    } else if (strcmp(argv[1], "char") == 0) {
        char a[argc - 2];
        for (int i = 2; i < argc; i++) {
            a[i - 2] = argv[i][0];
        }
        mergesort(a, argc - 2, sizeof(char), charcmp);
        for (int i = 0; i < argc - 2; i++)
            printf("%c ", a[i]);
        printf("\n");
    } else {
        mergesort(argv + 2, argc - 2, sizeof(char*), _strcmp);
        for (int i = 0; i < argc - 2; i++)
            printf("%s ", argv[i + 2]);
        printf("\n");
    }
    return 0;
}
