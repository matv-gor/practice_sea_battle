#include "ui_logic.h"

#include <cstddef>

namespace frontend {

bool contains(const sf::FloatRect &rect, sf::Vector2f point) {
  return rect.contains(point);
}

std::optional<sf::Vector2i> cellAt(sf::Vector2f point, sf::Vector2f origin) {
  const float boardPixels =
      kBoardSize * kCellSize + (kBoardSize - 1) * kCellGap;
  if (point.x < origin.x || point.y < origin.y ||
      point.x > origin.x + boardPixels || point.y > origin.y + boardPixels) {
    return std::nullopt;
  }

  const int col = static_cast<int>((point.x - origin.x) / kBoardStep);
  const int row = static_cast<int>((point.y - origin.y) / kBoardStep);
  const float localX =
      point.x - origin.x - static_cast<float>(col) * kBoardStep;
  const float localY =
      point.y - origin.y - static_cast<float>(row) * kBoardStep;

  if (row < 0 || row >= kBoardSize || col < 0 || col >= kBoardSize ||
      localX > kCellSize || localY > kCellSize) {
    return std::nullopt;
  }

  return sf::Vector2i(col, row);
}

std::vector<sf::Vector2i> shipCells(sf::Vector2i start, int size,
                                    Orientation orientation) {
  std::vector<sf::Vector2i> cells;
  cells.reserve(static_cast<std::size_t>(size));
  for (int index = 0; index < size; ++index) {
    cells.push_back(
        {start.x + (orientation == Orientation::Horizontal ? index : 0),
         start.y + (orientation == Orientation::Vertical ? index : 0)});
  }
  return cells;
}

bool canPlace(const PlayerUiState &player,
              const std::vector<sf::Vector2i> &cells) {
  for (const sf::Vector2i cell : cells) {
    if (cell.x < 0 || cell.x >= kBoardSize || cell.y < 0 ||
        cell.y >= kBoardSize) {
      return false;
    }
    if (player.ships[static_cast<std::size_t>(cell.y)]
                    [static_cast<std::size_t>(cell.x)] ||
        player.halo[static_cast<std::size_t>(cell.y)]
                   [static_cast<std::size_t>(cell.x)]) {
      return false;
    }
  }
  return true;
}

void addShip(PlayerUiState &player, const std::vector<sf::Vector2i> &cells) {
  UiShip ship;
  ship.size = static_cast<int>(cells.size());
  ship.cells = cells;
  ship.hits.assign(cells.size(), false);

  for (const sf::Vector2i cell : cells) {
    player.ships[static_cast<std::size_t>(cell.y)]
                [static_cast<std::size_t>(cell.x)] = true;
  }

  for (const sf::Vector2i cell : cells) {
    for (int dy = -1; dy <= 1; ++dy) {
      for (int dx = -1; dx <= 1; ++dx) {
        const int row = cell.y + dy;
        const int col = cell.x + dx;
        if (row >= 0 && row < kBoardSize && col >= 0 && col < kBoardSize &&
            !player.ships[static_cast<std::size_t>(row)]
                         [static_cast<std::size_t>(col)]) {
          player.halo[static_cast<std::size_t>(row)]
                     [static_cast<std::size_t>(col)] = true;
        }
      }
    }
  }

  player.fleet.push_back(ship);
  --player.remaining[static_cast<std::size_t>(cells.size())];
}

bool fleetReady(const PlayerUiState &player) {
  for (int size = 1; size <= 4; ++size) {
    if (player.remaining[static_cast<std::size_t>(size)] != 0) {
      return false;
    }
  }
  return true;
}

void applyShot(PlayerUiState &defender, sf::Vector2i cell, int result) {
  const auto row = static_cast<std::size_t>(cell.y);
  const auto col = static_cast<std::size_t>(cell.x);

  if (result == 0) {
    defender.shots[row][col] = ShotState::Miss;
    return;
  }
  if (result < 1) {
    return;
  }

  defender.shots[row][col] = ShotState::Hit;
  for (UiShip &ship : defender.fleet) {
    for (std::size_t index = 0; index < ship.cells.size(); ++index) {
      if (ship.cells[index] != cell) {
        continue;
      }

      ship.hits[index] = true;
      bool sunk = true;
      for (const bool hit : ship.hits) {
        sunk = sunk && hit;
      }

      if (sunk || result == 2) {
        for (const sf::Vector2i shipCell : ship.cells) {
          defender.shots[static_cast<std::size_t>(shipCell.y)]
                        [static_cast<std::size_t>(shipCell.x)] =
              ShotState::Sunk;
          for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
              const int markRow = shipCell.y + dy;
              const int markCol = shipCell.x + dx;
              if (markRow >= 0 && markRow < kBoardSize && markCol >= 0 &&
                  markCol < kBoardSize &&
                  defender.shots[static_cast<std::size_t>(markRow)]
                                [static_cast<std::size_t>(markCol)] ==
                      ShotState::Unknown) {
                defender.shots[static_cast<std::size_t>(markRow)]
                              [static_cast<std::size_t>(markCol)] =
                    ShotState::Miss;
              }
            }
          }
        }
      }
      return;
    }
  }
}

void resetPlayer(PlayerUiState &player) { player = PlayerUiState{}; }

} // namespace frontend
