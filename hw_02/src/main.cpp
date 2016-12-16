#include <Board.h>
#include <BoardView.h>

int main() {
    Board board;
    View view(board);
    view.doGameCycle();
    return 0;
}
