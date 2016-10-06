#ifndef MERGESORT_H_
#define MERGESORT_H_

#include <stddef.h>

int mergesort(void *base, size_t num, size_t size, int (*compar)(const void *, const void *));

#endif //MERGESORT_H_
