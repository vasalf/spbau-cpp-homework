#ifndef BOARD_TEST_H_
#define BOARD_TEST_H_

#include <Test.h>

class BoardTest : public Test {
  public:
    BoardTest();
  
    void testLDiag();
    void testRDiag();
    void testXWins();

    void testCanMove();
    void testCannotMove1();
    void testCannotMove2();
    void testCanMoveOutOfBounds();

    void testMove1();
    void testMove2();
    void testMove3();

    void runAllTests();
};

#endif //BOARD_TEST_H_
