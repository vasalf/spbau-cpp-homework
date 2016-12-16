#ifndef TEST_H_
#define TEST_H_

#include <cstddef>

class Test {
  protected:
    static int failedNum;
    static int totalNum;
  public:
    static void check(bool expr, const char *func, const char *filename, size_t lineNum);
    static void showFinalResult();
    virtual void runAllTests() = 0;
};

#endif // TEST_H_
