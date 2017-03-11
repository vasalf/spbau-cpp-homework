#include <cassert>
#include "my_array.h"

int main() {
    my_array<bool, 10> arr;
    for (int i = 0; i < 4; i++)
        arr[i] = true;
    for (int i = 4; i < 10; i++)
        arr[i] = false;
    for (int i = 0; i < 4; i++)
        assert(arr[i] == true);
    for (int i = 4; i < 10; i++)
        assert(arr[i] == false);
    arr.fill(true);
    for (int i = 0; i < 10; i++)
        assert(arr[i] == true);
    arr.fill(false);
    for (int i = 0; i < 10; i++)
        assert(arr[i] == false);
    arr.fill((char)2);
    for (int i = 0; i < 10; i++)
        assert(arr[i] == true);
    return 0;
}
