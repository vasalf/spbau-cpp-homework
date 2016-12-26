#include <Board.h>
#include <algorithm>
#include <cassert>

using std::min;
using std::max;

Board::Board() {
    for (int i = 0; i < 10; i++)
        std::fill(_board[i].begin(), _board[i].end(), '.');
}

void Board::move(int x, int y, int sign) {
    _board[x][y] = sign;
}

bool Board::canMove(int x, int y, __attribute__((unused)) char sign) const {
    if (isWin() != GameState::GAME_IN_PROGRESS)
        return false;
    if (x < 0 || x >= 10 || y < 0 || y >= 10)
        return false;
    if (_board[x][y] != '.')
        return false;
    return true;
}

Board::GameState Board::isWin() const {
    #define CHECK_WIN(dx, dy)                                                       \
    for (int i = -min(0, dx); i < 10 - max(0, dx) * 4; i++) {                       \
        for (int j = -min(0, dy) * 4; j < 10 - max(0, dy) * 4 ; j++) {              \
            bool f = true;                                                          \
            for (int k = 1; f && k < 5; k++) {                                      \
                f &= (_board[i + dx * k][j + dy * k] == _board[i][j]);              \
            }                                                                       \
            if (f && _board[i][j] != '.')                                           \
                return _board[i][j] == 'X' ? GameState::X_WINS : GameState::O_WINS; \
        }                                                                           \
    }

    CHECK_WIN(0, 1)
    CHECK_WIN(1, 0)
    CHECK_WIN(1, 1)
    CHECK_WIN(1, -1)

   for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++)
            if (_board[i][j] == '.')
                return GameState::GAME_IN_PROGRESS; 
    }
    return GameState::DRAW;
}

char Board::getCell(int x, int y) const {
    return _board[x][y];
}
