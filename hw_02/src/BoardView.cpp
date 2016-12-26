#include <BoardView.h>
#include <iostream>
#include <algorithm>

using std::cin;
using std::cout;
using std::endl;

View::View(Board& board) : _board(board) {}

void View::showBoard() const {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++)
            cout << _board.getCell(i, j);
        cout << endl;
    }
}

void View::doTurn(char who) {
    showBoard();
    while (true) {
        cout << who << " move: ";
        int x, y;
        cin >> x >> y;
        if (x == -1 && y == -1) {
            end = true;
            break;
        }
        if (_board.canMove(x, y, who)) {
            _board.move(x, y, who);
            break;
        }
        cout << "Bad move!" << endl;
    }
    cout << endl;
}

void View::doGameCycle() {
    char cur_turn = 'O', next_turn = 'X';
    while (_board.isWin() == Board::GameState::GAME_IN_PROGRESS) {
        doTurn(cur_turn);
        if (end)
            return;
        std::swap(cur_turn, next_turn);
    }
    showBoard();
    if (_board.isWin() == Board::GameState::X_WINS)
        cout << "X wins!" << endl;
    else if (_board.isWin() == Board::GameState::O_WINS)
        cout << "O wins!" << endl;
    else
        cout << "Draw." << endl;
}
