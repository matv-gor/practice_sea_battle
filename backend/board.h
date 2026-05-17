#pragma once
#include <vector>
#include "constants.h"
#include "ship.hpp"

class Board {
private:
    int board[10][10]{0};
    std::vector<Ship> ships;

public:
    Board()=default;

    int createShip(int inputSize, int inputNumberStart, char inputLetterStart,
                   int inputNumberEnd, char inputLetterEnd);
    int shootShip(int inputNumber, char inputLetter);
    void editBoard(int inputNumber, int numberInstedLetter, int outputValueCell);
    bool isAllSunk() const;
    void clearBoard();
    int getCell(int row, int col) const;
};


/*
0 - пустая клетка(ставить можно)
1 - вокруг корабля(ставить нельзя)
2 - "живая" клетка корабля
3 - "мертвая клетка корабля"
4 - промах
*/
