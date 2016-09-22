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
#include <test_str.h>

#define PERFORM_TEST(cond) \
    if (!(cond)) \
        return 1;

int test_strcmp(void) {
    PERFORM_TEST(strcmp("179", "179") == 0);
    PERFORM_TEST(strcmp("179", "178") == 1);
    PERFORM_TEST(strcmp("179", "180") == -1);
    PERFORM_TEST(strcmp("179", "18") == -1);
    PERFORM_TEST(strcmp("", "0") == -1);
    PERFORM_TEST(strcmp("0", "") == 1);

    char s[256], t[256];
    *s = 'a'; *(s+1) = 'b'; *(s+2) = 0;
    *t = 'a'; *(t+1) = 'b'; *(t+2) = 0;
    PERFORM_TEST(strcmp(s, t) == 0);
    t[0] = 'b';
    PERFORM_TEST(strcmp(s, t) == -1);

    return 0;
}

int test_strlen(void) {
    char s[256];
    for (int i = 0; i < 256; i++)
        s[i] = '1';
    
    s[2] = 0;
    PERFORM_TEST(strlen(s) == 2);
    s[2] = '1';

    s[179] = 0;
    PERFORM_TEST(strlen(s) == 179);
    s[179] = '1';

    s[255] = 0;
    PERFORM_TEST(strlen(s) == 255);
    s[255] = '1';

    return 0;
}

int test_strcpy(void) {
    char s[256], t[256], q[256];

    strcpy(t, "179");
    strcpy(s, t);
    PERFORM_TEST(strcmp(s, t) == 0);

    strcpy(q, "178");
    strcpy(s, q);
    PERFORM_TEST(strcmp(s, q) == 0);
    PERFORM_TEST(strcmp(s, t) == -1);

    strcpy(s, t);
    PERFORM_TEST(strcmp(s, t) == 0);
    return 0;
}

int test_strcat(void) {
    char s[256];
    s[0] = 0;

    for (int i = 0; i < 10; i++)
        strcat(s, "179");
    PERFORM_TEST(strlen(s) == 30);
    PERFORM_TEST(strcmp(s + 27, "179") == 0);

    return 0;
}
