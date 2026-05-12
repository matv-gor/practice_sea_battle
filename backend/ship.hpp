#pragma once
#include "Coordinate.h"
#include <vector>
class Ship{
private:
    int size;
    int health;
    std::vector<Coordinate> positions;
public:
    Ship();
    Ship(int size):size(size),health(size){}
    void addPosition(int row, int col);
    bool isDestroyed() const;
    bool isHit(int row, int col) const;
    void hit();
};
