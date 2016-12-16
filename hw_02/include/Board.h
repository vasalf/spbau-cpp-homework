#ifndef BOARD_H_
#define BOARD_H_

#include <array>

class Board {
    std::array<std::array<char, 10>, 10> _board;
  public:
    Board();
    enum class GameState {
        GAME_IN_PROGRESS,
        X_WINS,
        O_WINS,
        DRAW
    };
    void move(int x, int y, int sign); // By the way, why int?
    bool canMove(int x, int y, char sign) const; 
    GameState isWin() const;
    char getCell(int x, int y) const;
};

#endif // BOARD_H_
