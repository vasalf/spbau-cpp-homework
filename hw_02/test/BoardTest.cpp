#include <BoardTest.h>
#include <Board.h>

#define DO_CHECK(expr) BoardTest::check((expr), __FUNCTION__, __FILE__, __LINE__)

BoardTest::BoardTest() {}

void BoardTest::testLDiag() {
    Board b;
    b.move(0, 0, 'O');
    b.move(1, 1, 'O');
    b.move(2, 2, 'O');
    b.move(3, 3, 'O');
    b.move(4, 4, 'O');
    DO_CHECK(b.isWin() == Board::GameState::O_WINS);
}

void BoardTest::testRDiag() {
    Board b;
    b.move(9, 0, 'O');
    b.move(8, 1, 'O');
    b.move(7, 2, 'O');
    b.move(6, 3, 'O');
    b.move(5, 4, 'O');
    DO_CHECK(b.isWin() == Board::GameState::O_WINS);
}

void BoardTest::testXWins() {
    Board b;
    b.move(0, 0, 'X');
    b.move(0, 1, 'X');
    b.move(0, 2, 'X');
    b.move(0, 3, 'X');
    b.move(0, 4, 'X');
    DO_CHECK(b.isWin() == Board::GameState::X_WINS);
}

void BoardTest::testCanMove() {
    Board b;
    b.move(0, 0, 'O');
    DO_CHECK(b.canMove(1, 1, 'X'));
}

void BoardTest::testCannotMove1() {
    Board b;
    b.move(1, 1, 'O');
    DO_CHECK(!b.canMove(1, 1, 'X'));
}

void BoardTest::testCannotMove2() {
    Board b;
    b.move(0, 0, 'X');
    b.move(0, 1, 'X');
    b.move(0, 2, 'X');
    b.move(0, 3, 'X');
    b.move(0, 4, 'X');
    DO_CHECK(!b.canMove(9, 9, 'O'));
}

void BoardTest::testCanMoveOutOfBounds() {
    Board b;
    DO_CHECK(!b.canMove(-1, 0, 'O'));
    DO_CHECK(!b.canMove(0, -1, 'O'));
    DO_CHECK(!b.canMove(-1,-1, 'O'));
    DO_CHECK(!b.canMove(0, 10, 'O'));
    DO_CHECK(!b.canMove(10, 0, 'O'));
    DO_CHECK(!b.canMove(100500, 1791791791, 'O'));
}

void BoardTest::testMove1() {
    Board b;
    b.move(0, 0, 'O');
    DO_CHECK(b.getCell(0, 0) == 'O');
}

void BoardTest::testMove2() {
    Board b;
    b.move(0, 0, 'X');
    DO_CHECK(b.getCell(0, 0) == 'X');
}

void BoardTest::testMove3() {
    Board b;
    b.move(1, 1, 'O');
    DO_CHECK(b.getCell(0, 0) == '.');
}

void BoardTest::runAllTests() {
    testLDiag();
    testRDiag();
    testXWins();
    testCanMove();
    testCannotMove1();
    testCannotMove2();
    testCanMoveOutOfBounds();
    testMove1();
    testMove2();
    testMove3();
}

