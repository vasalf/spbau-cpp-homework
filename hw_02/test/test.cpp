#include <BoardTest.h>

int Test::failedNum = 0;
int Test::totalNum = 0;

int main() {
    BoardTest bt;
    bt.runAllTests();
    BoardTest::showFinalResult();
    return 0;
}
