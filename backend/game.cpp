#include "game.h"

int Game::placeShip(int size, int rowStart, char colStart, int rowEnd, char colEnd) {
    if (phase != GamePhase::Placement) return -1;
    Board& target = playerPlacementDone ? opponentBoard : playerBoard;
    return target.createShip(size, rowStart, colStart, rowEnd, colEnd);
}

void Game::finishPlacement() {
    if (!playerPlacementDone) {
        playerPlacementDone = true;
    } else {
        phase = GamePhase::Battle;
    }
}

int Game::shoot(int row, char col) {
    if (phase != GamePhase::Battle) return -1;
    Board& target = playerTurn ? opponentBoard : playerBoard;

    int result = target.shootShip(row, col);

    if (result == 0) {
        playerTurn = !playerTurn;
    }

    if (target.isAllSunk()) {
        phase = GamePhase::Finished;
    }

    return result;
}

bool Game::isOver() const {
    return phase == GamePhase::Finished;
}

GamePhase Game::getPhase() const {
    return phase;
}

bool Game::isPlayerTurn() const {
    return playerTurn;
}
