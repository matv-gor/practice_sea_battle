#pragma once

#include "ui_types.h"

#include <optional>
#include <vector>

namespace frontend {

bool contains(const sf::FloatRect &rect, sf::Vector2f point);
std::optional<sf::Vector2i> cellAt(sf::Vector2f point, sf::Vector2f origin);
std::vector<sf::Vector2i> shipCells(sf::Vector2i start, int size,
                                    Orientation orientation);
bool canPlace(const PlayerUiState &player,
              const std::vector<sf::Vector2i> &cells);
void addShip(PlayerUiState &player, const std::vector<sf::Vector2i> &cells);
bool fleetReady(const PlayerUiState &player);
void applyShot(PlayerUiState &defender, sf::Vector2i cell, int result);
void resetPlayer(PlayerUiState &player);

} // namespace frontend
