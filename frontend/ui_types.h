#pragma once

#include <SFML/Graphics.hpp>

#include <array>
#include <string>
#include <vector>

namespace frontend {

constexpr int kBoardSize = 10;
constexpr float kCellSize = 44.f;
constexpr float kCellGap = 3.f;
constexpr float kBoardStep = kCellSize + kCellGap;

enum class Screen {
  Placement,
  Handoff,
  Battle,
  Finished,
};

enum class Orientation {
  Horizontal,
  Vertical,
};

enum class ShotState {
  Unknown,
  Miss,
  Hit,
  Sunk,
};

struct UiShip {
  int size{};
  std::vector<sf::Vector2i> cells;
  std::vector<bool> hits;
};

struct PlayerUiState {
  std::array<std::array<bool, kBoardSize>, kBoardSize> ships{};
  std::array<std::array<bool, kBoardSize>, kBoardSize> halo{};
  std::array<std::array<ShotState, kBoardSize>, kBoardSize> shots{};
  std::vector<UiShip> fleet;
  std::array<int, 5> remaining{0, 4, 3, 2, 1};
};

struct Button {
  sf::FloatRect bounds;
  std::string label;
  bool enabled = true;
};

} // namespace frontend
