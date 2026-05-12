#include "ship.hpp"

Ship::Ship() : size(0), health(0) {}

void Ship::addPosition(int row, int col) {
    positions.push_back({row, col});
}

bool Ship::isHit(int row, int col) const {
    for (const auto& pos : positions) {
        if (pos.getRow() == row && pos.getCol() == col) {
            return true;
        }
    }
    return false;
}

bool Ship::isDestroyed() const {
    return health == 0;
}

void Ship::hit() {
    if (health > 0) {
        health--;
    }
}
