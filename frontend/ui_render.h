#pragma once

#include "ui_types.h"

#include <optional>
#include <string>
#include <vector>

namespace frontend {

std::optional<sf::Font> loadFont();
void drawText(sf::RenderWindow &window, const std::optional<sf::Font> &font,
              const std::string &value, unsigned int size,
              sf::Vector2f position, sf::Color color = sf::Color(29, 41, 57));
void drawCenteredText(sf::RenderWindow &window,
                      const std::optional<sf::Font> &font,
                      const std::string &value, unsigned int size,
                      const sf::FloatRect &bounds,
                      sf::Color color = sf::Color(29, 41, 57));
void drawButton(sf::RenderWindow &window, const std::optional<sf::Font> &font,
                const Button &button);
void drawBoard(
    sf::RenderWindow &window, const std::optional<sf::Font> &font,
    const PlayerUiState &board, sf::Vector2f origin, bool showShips,
    const std::string &title,
    const std::optional<std::vector<sf::Vector2i>> &preview = std::nullopt,
    bool previewValid = false);

} // namespace frontend
