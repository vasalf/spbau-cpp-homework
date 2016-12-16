#include <Test.h>
#include <iostream>

using std::cout;
using std::endl;

void Test::check(bool expr, const char *func, const char *filename, size_t lineNum) {
    totalNum++;
    failedNum += !expr;
    if (!expr)
        cout << "test failed: " << func << " in " << filename << ":" << lineNum << endl;
}

void Test::showFinalResult() {
    if (failedNum == 0)
        cout << "All tests passed." << endl;
    else
        cout << "Failed " << failedNum << " of " << totalNum << " tests." << endl;
}
