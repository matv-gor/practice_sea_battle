#include "ui_render.h"

#include <array>

namespace frontend {

std::optional<sf::Font> loadFont() {
  const std::array<std::string, 6> paths{
      "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
      "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf",
      "/usr/share/fonts/truetype/freefont/FreeSans.ttf",
      "C:/Windows/Fonts/arial.ttf",
      "C:/Windows/Fonts/segoeui.ttf",
      "arial.ttf",
  };

  for (const auto &path : paths) {
    sf::Font font;
    if (font.openFromFile(path)) {
      return font;
    }
  }
  return std::nullopt;
}

void drawText(sf::RenderWindow &window, const std::optional<sf::Font> &font,
              const std::string &value, unsigned int size,
              sf::Vector2f position, sf::Color color) {
  if (!font.has_value()) {
    return;
  }

  sf::Text text(*font);
  text.setString(value);
  text.setCharacterSize(size);
  text.setFillColor(color);
  text.setPosition(position);
  window.draw(text);
}

void drawCenteredText(sf::RenderWindow &window,
                      const std::optional<sf::Font> &font,
                      const std::string &value, unsigned int size,
                      const sf::FloatRect &bounds, sf::Color color) {
  if (!font.has_value()) {
    return;
  }

  sf::Text text(*font);
  text.setString(value);
  text.setCharacterSize(size);
  text.setFillColor(color);
  const sf::FloatRect textBounds = text.getLocalBounds();
  text.setOrigin({textBounds.position.x + textBounds.size.x / 2.f,
                  textBounds.position.y + textBounds.size.y / 2.f});
  text.setPosition({bounds.position.x + bounds.size.x / 2.f,
                    bounds.position.y + bounds.size.y / 2.f - 1.f});
  window.draw(text);
}

void drawButton(sf::RenderWindow &window, const std::optional<sf::Font> &font,
                const Button &button) {
  sf::RectangleShape shape(button.bounds.size);
  shape.setPosition(button.bounds.position);
  shape.setFillColor(button.enabled ? sf::Color(47, 111, 237)
                                    : sf::Color(148, 163, 184));
  shape.setOutlineThickness(1.f);
  shape.setOutlineColor(sf::Color(30, 64, 175));
  window.draw(shape);

  drawCenteredText(window, font, button.label, 17, button.bounds,
                   sf::Color::White);
}

void drawBoard(sf::RenderWindow &window, const std::optional<sf::Font> &font,
               const PlayerUiState &board, sf::Vector2f origin, bool showShips,
               const std::string &title,
               const std::optional<std::vector<sf::Vector2i>> &preview,
               bool previewValid) {
  drawText(window, font, title, 22, {origin.x, origin.y - 76.f});

  for (int index = 0; index < kBoardSize; ++index) {
    drawCenteredText(
        window, font, std::string(1, static_cast<char>('A' + index)), 14,
        {{origin.x + static_cast<float>(index) * kBoardStep, origin.y - 36.f},
         {kCellSize, 20.f}},
        sf::Color(71, 85, 105));
    drawCenteredText(
        window, font, std::to_string(index + 1), 14,
        {{origin.x - 34.f, origin.y + static_cast<float>(index) * kBoardStep},
         {26.f, kCellSize}},
        sf::Color(71, 85, 105));
  }

  for (int row = 0; row < kBoardSize; ++row) {
    for (int col = 0; col < kBoardSize; ++col) {
      const auto rowIndex = static_cast<std::size_t>(row);
      const auto colIndex = static_cast<std::size_t>(col);
      sf::RectangleShape cell({kCellSize, kCellSize});
      cell.setPosition({origin.x + static_cast<float>(col) * kBoardStep,
                        origin.y + static_cast<float>(row) * kBoardStep});

      sf::Color fill(226, 244, 255);
      if (showShips && board.halo[rowIndex][colIndex]) {
        fill = sf::Color(203, 230, 241);
      }
      if (showShips && board.ships[rowIndex][colIndex]) {
        fill = sf::Color(78, 91, 111);
      }

      switch (board.shots[rowIndex][colIndex]) {
      case ShotState::Miss:
        fill = sf::Color(181, 214, 226);
        break;
      case ShotState::Hit:
        fill = sf::Color(248, 113, 113);
        break;
      case ShotState::Sunk:
        fill = sf::Color(127, 29, 29);
        break;
      case ShotState::Unknown:
        break;
      }

      cell.setFillColor(fill);
      cell.setOutlineThickness(1.f);
      cell.setOutlineColor(sf::Color(148, 163, 184));
      window.draw(cell);

      const ShotState shot = board.shots[rowIndex][colIndex];
      if (shot == ShotState::Miss) {
        sf::CircleShape dot(4.f);
        dot.setFillColor(sf::Color(51, 65, 85));
        dot.setPosition(
            {cell.getPosition().x + 18.f, cell.getPosition().y + 18.f});
        window.draw(dot);
      } else if (shot == ShotState::Hit || shot == ShotState::Sunk) {
        sf::RectangleShape slashA({34.f, 4.f});
        slashA.setFillColor(sf::Color::White);
        slashA.setOrigin({17.f, 2.f});
        slashA.setPosition({cell.getPosition().x + kCellSize / 2.f,
                            cell.getPosition().y + kCellSize / 2.f});
        slashA.setRotation(sf::degrees(45.f));
        window.draw(slashA);

        sf::RectangleShape slashB = slashA;
        slashB.setRotation(sf::degrees(-45.f));
        window.draw(slashB);
      }
    }
  }

  if (!preview.has_value()) {
    return;
  }

  for (const sf::Vector2i cellPos : *preview) {
    if (cellPos.x < 0 || cellPos.x >= kBoardSize || cellPos.y < 0 ||
        cellPos.y >= kBoardSize) {
      continue;
    }

    sf::RectangleShape cell({kCellSize, kCellSize});
    cell.setPosition({origin.x + static_cast<float>(cellPos.x) * kBoardStep,
                      origin.y + static_cast<float>(cellPos.y) * kBoardStep});
    cell.setFillColor(previewValid ? sf::Color(34, 197, 94, 150)
                                   : sf::Color(239, 68, 68, 150));
    window.draw(cell);
  }
}

} // namespace frontend
