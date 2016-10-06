#include "mergesort.h"
#include <stdlib.h>
#include <stdint.h>

void copy_mem(void * dest, const void *src, size_t size) {
    while (size--) {
        *((uint8_t*)dest++) = *((uint8_t*)src++);
    }
}

void merge(void *a, void *b, void *storage, size_t numa, size_t numb, size_t size, int (*compar)(const void *, const void *)) {
    size_t i = 0, j = 0, k = 0;
    while (i < numa || j < numb) {
        if (j == numb || (i != numa && compar(a + size * i, b + size * j) < 0)) {
            copy_mem(storage + k * size, a + i * size, size);
            i++;
        } else {
            copy_mem(storage + k * size, b + j * size, size);
            j++;
        }
        k++;
    }
}

#define min(a, b) ((a) < (b) ? (a) : (b))

int mergesort(void *base, size_t num, size_t size, int (*compar)(const void *, const void *)) {
    void *storage = malloc(size * num);
    for (size_t k = 1; k < num; k <<= 1) {
        for (size_t i = 0; i + k < num; i += 2 * k) {
            merge(base + i * size, base + i * size + k * size, storage, k, min(k, num - (i + k)), size, compar);
            copy_mem(base + i * size, storage, (k + min(k, num - (i + k))) * size);
        }
    }
    free(storage);
    return 0;
}
