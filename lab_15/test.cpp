#include "linq.h"
#include <cassert>

void test() {
    const int v[] = {1};
    auto vec = from(v, v + 1)
        .where_neq(1)
        .to_vector();
    assert(vec == std::vector<int>());
}

int main() {
    test();
    return 0;
}
