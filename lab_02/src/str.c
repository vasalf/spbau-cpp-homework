/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org>
 **/

#include <str.h>
#include <stdio.h>

char *strcpy(char *destination, const char *source) {
    char *ret = destination;
    while (*source != 0)
        *(destination++) = *(source++);
    *destination = 0;
    return ret;
}

size_t strlen(const char *str) {
    size_t ret = 0;

    while (*str != 0) {
        str++;
        ret++;
    }

    return ret;
}

char *strcat(char *destination, const char *source) {
    return strcpy(destination + strlen(destination), source);
}

int strcmp(const char *str1, const char *str2) {
    while (*str1 != 0 || *str2 != 0) {        
        if (*str1 != *str2) {
            if (*str1 < *str2)
                return -1;
            else
                return 1;
        }
        str1++; str2++;
    }
    if (*str1 < *str2)
        return -1;
    else if (*str1 == *str2)
        return 0;
    else
        return 1;
}
