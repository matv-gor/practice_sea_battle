#include "board.h"

int Board::createShip(int inputSize, int inputNumberStart, char inputLetterStart,
                      int inputNumberEnd, char inputLetterEnd) {
    int numberInstedLetterStart = inputLetterStart - Constants::kLetterToNumberSymbol;
    int numberInstedLetterEnd = inputLetterEnd - Constants::kLetterToNumberSymbol;
    if (inputNumberStart < 0 || inputNumberStart > 9 ||
        numberInstedLetterStart < 0 || numberInstedLetterStart > 9 ||
        inputNumberEnd < 0 || inputNumberEnd > 9 ||
        numberInstedLetterEnd < 0 || numberInstedLetterEnd > 9) {
        return 1;
    }

    Ship newShip(inputSize);

    if (numberInstedLetterEnd == numberInstedLetterStart) {
        for (int i{0}; i < inputSize; ++i) {
            int row = inputNumberStart + i;
            int col = numberInstedLetterStart;
            if (row > 9 || board[row][col] != 0) {
                return 2;
            }
        }
        for (int i{0}; i < inputSize; ++i) {
            editBoard(inputNumberStart + i, numberInstedLetterStart, 2);
            newShip.addPosition(inputNumberStart + i, numberInstedLetterStart);
        }
        for (int i{0}; i < inputSize + 2; ++i) {
            for (int j{-1}; j <= 1; ++j) {
                editBoard(inputNumberStart + i - 1, numberInstedLetterStart + j, 1);
            }
        }
    } else if (inputNumberStart == inputNumberEnd) {
        for (int i{0}; i < inputSize; ++i) {
            int row = inputNumberStart;
            int col = numberInstedLetterStart + i;
            if (col > 9 || board[row][col] != 0) {
                return 2;
            }
        }
        for (int i{0}; i < inputSize; ++i) {
            editBoard(inputNumberStart, numberInstedLetterStart + i, 2);
            newShip.addPosition(inputNumberStart, numberInstedLetterStart + i);
        }
        for (int i{0}; i <= 1; ++i) {
            for (int j{-1}; j <= inputSize; ++j) {
                editBoard(inputNumberStart + i, numberInstedLetterStart + j, 1);
            }
        }
    }

    ships.push_back(newShip);
    return 0;
}

void Board::editBoard(int inputNumber, int numberInstedLetter, int outputValueCell) {
    if (inputNumber < 0 || inputNumber > 9 ||
        numberInstedLetter < 0 || numberInstedLetter > 9) {
        return;
    }
    if (board[inputNumber][numberInstedLetter] == 2 ||
        board[inputNumber][numberInstedLetter] == 3) {
        return;
    }
    board[inputNumber][numberInstedLetter] = outputValueCell;
}

int Board::shootShip(int inputNumber, char inputLetter) {
    int row = inputNumber;
    int col = inputLetter - Constants::kLetterToNumberSymbol;

    if (row < 0 || row > 9 || col < 0 || col > 9) {
        return -1;
    }

    if (board[row][col] == 0 || board[row][col] == 1) {
        board[row][col] = 4;
        return 0;
    }

    if (board[row][col] == 3 || board[row][col] == 4) {
        return -1;
    }

    if (board[row][col] == 2) {
        board[row][col] = 3;

        for (Ship& ship : ships) {
            if (ship.isHit(row, col)) {
                ship.hit();

                if (ship.isDestroyed()) {
                    for (int i{0}; i < 10; ++i) {
                        for (int j{0}; j < 10; ++j) {
                            if (board[i][j] == 2 && ship.isHit(i, j)) {
                                board[i][j] = 3;
                            }
                        }
                    }
                    for (int i{0}; i < 10; ++i) {
                        for (int j{0}; j < 10; ++j) {
                            if (board[i][j] == 3 && ship.isHit(i, j)) {
                                for (int di{-1}; di <= 1; ++di) {
                                    for (int dj{-1}; dj <= 1; ++dj) {
                                        int ni = i + di;
                                        int nj = j + dj;
                                        if (ni >= 0 && ni < 10 && nj >= 0 && nj < 10) {
                                            if (board[ni][nj] == 0) {
                                                board[ni][nj] = 1;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    return 2;
                }
                return 1;
            }
        }
    }

    return -1;
}

int Board::getCell(int row, int col) const {
    return board[row][col];
}

void Board::clearBoard() {
    for (int i{0}; i < 10; ++i) {
        for (int j{0}; j < 10; ++j) {
            board[i][j] = 0;
        }
    }
    ships.clear();
}

bool Board::isAllSunk() const {
    for (const Ship& ship : ships) {
        if (!ship.isDestroyed()) {
            return false;
        }
    }
    return true;
}
