#include <Board.h>
#include <algorithm>

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
    if (_board[x][y] != '.')
        return false;
    return true;
}

Board::GameState Board::isWin() const {
    for (int i = 0; i <= 10; i++) {
        for (int j = 0; j + 5 <= 10; j++) {
            bool f = true;
            for (int k = 1; f && k < 5; k++) {
                f &= (_board[i][j + k] == _board[i][j]);
            }
            if (f) {
                if (_board[i][j] == 'X')
                    return GameState::X_WINS;
                else if (_board[i][j] == 'O')
                    return GameState::O_WINS; 
            }
        }
    }
    for (int i = 0; i + 5 <= 10; i++) {
        for (int j = 0; j <= 10; j++) {
            bool f = true;
            for (int k = 1; f && k < 5; k++) {
                f &= (_board[i + k][j] == _board[i][j]);
            }
            if (f) {
                if (_board[i][j] == 'X')
                    return GameState::X_WINS;
                else if (_board[i][j] == 'O')
                    return GameState::O_WINS; 
            }
        }
    }
    for (int i = 0; i + 5 <= 10; i++) {
        for (int j = 0; j + 5 <= 10; j++) {
            bool f = true;
            for (int k = 1; f && k < 5; k++) {
                f &= (_board[i + k][j + k] == _board[i][j]);
            }
            if (f) {
                if (_board[i][j] == 'X')
                    return GameState::X_WINS;
                else if (_board[i][j] == 'O')
                    return GameState::O_WINS; 
            }
        }
    }
    for (int i = 4; i < 10; i++) {
        for (int j = 0; j + 5 <= 10; j++) {
            bool f = true;
            for (int k = 1; f && k < 5; k++) {
                f &= (_board[i - k][j + k] == _board[i][j]);
            }
            if (f) {
                if (_board[i][j] == 'X')
                    return GameState::X_WINS;
                else if (_board[i][j] == 'O')
                    return GameState::O_WINS; 
            }
        }
    }
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
