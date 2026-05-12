#pragma once
#include "board.h"

enum class GamePhase { Placement, Battle, Finished };

class Game {
private:
    Board playerBoard;
    Board opponentBoard;
    bool playerPlacementDone = false;
    bool playerTurn;
    GamePhase phase;

public:
    Game() : playerTurn(true), phase(GamePhase::Placement) {}

    int placeShip(int size, int rowStart, char colStart, int rowEnd, char colEnd);
    void finishPlacement();

    int shoot(int row, char col);

    bool isOver() const;
    bool isPlayerTurn() const;
    GamePhase getPhase() const;
};
