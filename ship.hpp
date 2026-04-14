#pragma once

class Board{
private:
    int board[10][10]{0};
    Board();

public:
    int createShip(int,int,char,int,char);//добавлено построение поля с кораблями, но пока без учета других кораблей вокруг
    int shootShip(int,char);
    void editBoard(int,int,int);
    //добавить функцию создания пустого поля
};

namespace Constants{
    const char kLetterToNumberSymbol = 'A';
}