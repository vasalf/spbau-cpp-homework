#ifndef BOARDVIEW_H_
#define BOARDVIEW_H_

#include <Board.h>

class View {
    Board& _board;
    bool end = false;
    void doTurn(char who);
  public:
    View(Board& board);
    void showBoard() const;
    void doGameCycle();
};

#endif // BOARDVIEW_H_
