#include "app.h"
#include "../backend/game.h"
#include "ui_logic.h"
#include "ui_render.h"

#include <SFML/Graphics.hpp>

#include <array>
#include <optional>
#include <string>
#include <vector>

namespace frontend {

int runApplication() {

  constexpr sf::Vector2f viewSize{1280.f, 760.f};

  const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
  sf::RenderWindow window(desktopMode, "Sea Battle", sf::Style::Default);
  window.setPosition({0, 0});
  window.setView(sf::View(sf::FloatRect({0.f, 0.f}, viewSize)));
  window.setVerticalSyncEnabled(true);

  const std::optional<sf::Font> font = loadFont();
  Game game;
  std::array<PlayerUiState, 2> players;

  Screen screen = Screen::Placement;
  int activePlayer = 0;
  int selectedSize = 4;
  Orientation orientation = Orientation::Horizontal;
  std::string message = "Player 1: place your fleet.";
  std::string handoffMessage;
  int winner = 0;

  const sf::Vector2f placementOrigin{92.f, 168.f};
  const sf::Vector2f ownBattleOrigin{68.f, 176.f};
  const sf::Vector2f enemyBattleOrigin{710.f, 176.f};

  while (window.isOpen()) {
    std::optional<std::vector<sf::Vector2i>> preview;
    bool previewValid = false;
    const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    const sf::Vector2f mouse = window.mapPixelToCoords(mousePosition);

    if (screen == Screen::Placement) {
      const std::optional<sf::Vector2i> hovered =
          cellAt(mouse, placementOrigin);
      if (hovered.has_value()) {
        preview = shipCells(*hovered, selectedSize, orientation);
        previewValid =
            players[static_cast<std::size_t>(activePlayer)]
                    .remaining[static_cast<std::size_t>(selectedSize)] > 0 &&
            canPlace(players[static_cast<std::size_t>(activePlayer)], *preview);
      }
    }

    const Button finishButton{
        {{840.f, 604.f}, {240.f, 48.f}},
        "Finish placement",
        fleetReady(players[static_cast<std::size_t>(activePlayer)])};
    const Button continueButton{
        {{520.f, 486.f}, {240.f, 54.f}}, "Continue", true};
    const Button restartButton{
        {{520.f, 500.f}, {240.f, 54.f}}, "New game", true};
    const Button rotateButton{
        {{840.f, 348.f}, {240.f, 44.f}},
        orientation == Orientation::Horizontal ? "Horizontal" : "Vertical",
        true};
    const std::array<Button, 4> shipButtons{{
        {{{840.f, 188.f}, {76.f, 44.f}},
         "x1",
         players[static_cast<std::size_t>(activePlayer)].remaining[1] > 0},
        {{{924.f, 188.f}, {76.f, 44.f}},
         "x2",
         players[static_cast<std::size_t>(activePlayer)].remaining[2] > 0},
        {{{1008.f, 188.f}, {76.f, 44.f}},
         "x3",
         players[static_cast<std::size_t>(activePlayer)].remaining[3] > 0},
        {{{1092.f, 188.f}, {76.f, 44.f}},
         "x4",
         players[static_cast<std::size_t>(activePlayer)].remaining[4] > 0},
    }};

    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }

      if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
        if (key->scancode == sf::Keyboard::Scancode::Escape) {
          window.close();
        }
        if (key->scancode == sf::Keyboard::Scancode::R) {
          orientation = orientation == Orientation::Horizontal
                            ? Orientation::Vertical
                            : Orientation::Horizontal;
        }
        if (screen == Screen::Placement &&
            key->scancode >= sf::Keyboard::Scancode::Num1 &&
            key->scancode <= sf::Keyboard::Scancode::Num4) {
          selectedSize = static_cast<int>(key->scancode) -
                         static_cast<int>(sf::Keyboard::Scancode::Num0);
        }
      }

      if (const auto *click = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (click->button != sf::Mouse::Button::Left) {
          continue;
        }

        const sf::Vector2f point = window.mapPixelToCoords(click->position);

        if (screen == Screen::Handoff &&
            contains(continueButton.bounds, point)) {
          screen = game.getPhase() == GamePhase::Battle ? Screen::Battle
                                                        : Screen::Placement;
          continue;
        }

        if (screen == Screen::Finished &&
            contains(restartButton.bounds, point)) {
          game = Game{};
          for (PlayerUiState &player : players) {
            resetPlayer(player);
          }
          activePlayer = 0;
          selectedSize = 4;
          orientation = Orientation::Horizontal;
          message = "Player 1: place your fleet.";
          screen = Screen::Placement;
          continue;
        }

        if (screen == Screen::Placement) {
          for (int index = 0; index < 4; ++index) {
            if (contains(shipButtons[static_cast<std::size_t>(index)].bounds,
                         point)) {
              selectedSize = index + 1;
            }
          }

          if (contains(rotateButton.bounds, point)) {
            orientation = orientation == Orientation::Horizontal
                              ? Orientation::Vertical
                              : Orientation::Horizontal;
          }

          if (finishButton.enabled && contains(finishButton.bounds, point)) {
            game.finishPlacement();
            if (activePlayer == 0) {
              activePlayer = 1;
              handoffMessage = "Pass the screen to Player 2.";
              message = "Player 2: place your fleet.";
            } else {
              activePlayer = 0;
              handoffMessage = "Pass the screen to Player 1.";
              message = "Player 1: choose a target.";
            }
            screen = Screen::Handoff;
            continue;
          }

          const std::optional<sf::Vector2i> cell =
              cellAt(point, placementOrigin);
          if (cell.has_value()) {
            std::vector<sf::Vector2i> cells =
                shipCells(*cell, selectedSize, orientation);
            PlayerUiState &player =
                players[static_cast<std::size_t>(activePlayer)];
            if (player.remaining[static_cast<std::size_t>(selectedSize)] <= 0) {
              message = "No ships of that size left.";
              continue;
            }
            if (!canPlace(player, cells)) {
              message = "Ship does not fit here.";
              continue;
            }

            const sf::Vector2i end = cells.back();
            const int result = game.placeShip(
                selectedSize, cell->y, static_cast<char>('A' + cell->x), end.y,
                static_cast<char>('A' + end.x));
            if (result == 0) {
              addShip(player, cells);
              message = "Ship placed.";
            } else {
              message = "Backend rejected this position.";
            }
          }
        } else if (screen == Screen::Battle) {
          const std::optional<sf::Vector2i> cell =
              cellAt(point, enemyBattleOrigin);
          if (!cell.has_value()) {
            continue;
          }

          const int defenderIndex = 1 - activePlayer;
          PlayerUiState &defender =
              players[static_cast<std::size_t>(defenderIndex)];
          ShotState &shot = defender.shots[static_cast<std::size_t>(cell->y)]
                                          [static_cast<std::size_t>(cell->x)];
          if (shot != ShotState::Unknown) {
            message = "This cell was already targeted.";
            continue;
          }

          const int result =
              game.shoot(cell->y, static_cast<char>('A' + cell->x));
          if (result >= 0) {
            applyShot(defender, *cell, result);
          }

          if (game.getPhase() == GamePhase::Finished) {
            winner = activePlayer;
            screen = Screen::Finished;
            continue;
          }

          if (result == 0) {
            activePlayer = defenderIndex;
            handoffMessage = "Miss. Pass the screen to Player " +
                             std::to_string(activePlayer + 1) + ".";
            message = "Player " + std::to_string(activePlayer + 1) +
                      ": choose a target.";
            screen = Screen::Handoff;
          } else if (result == 1) {
            message = "Hit. Player " + std::to_string(activePlayer + 1) +
                      " shoots again.";
          } else if (result == 2) {
            message = "Ship destroyed. Player " +
                      std::to_string(activePlayer + 1) + " shoots again.";
          } else {
            message = "Shot rejected.";
          }
        }
      }
    }

    window.clear(sf::Color(241, 245, 249));

    sf::RectangleShape topBar({1280.f, 86.f});
    topBar.setFillColor(sf::Color(15, 23, 42));
    window.draw(topBar);
    drawText(window, font, "Sea Battle", 32, {44.f, 22.f}, sf::Color::White);
    drawText(window, font, message, 20, {262.f, 31.f},
             sf::Color(226, 232, 240));

    if (screen == Screen::Placement) {
      drawBoard(window, font, players[static_cast<std::size_t>(activePlayer)],
                placementOrigin, true,
                "Player " + std::to_string(activePlayer + 1) + " fleet",
                preview, previewValid);

      drawText(window, font, "Ships", 22, {840.f, 142.f});
      for (int size = 1; size <= 4; ++size) {
        Button button = shipButtons[static_cast<std::size_t>(size - 1)];
        if (selectedSize == size) {
          button.bounds.position.y -= 5.f;
        }
        drawButton(window, font, button);
        drawText(
            window, font,
            std::to_string(players[static_cast<std::size_t>(activePlayer)]
                               .remaining[static_cast<std::size_t>(size)]) +
                " left",
            16,
            {button.bounds.position.x + 12.f, button.bounds.position.y + 50.f},
            sf::Color(71, 85, 105));
      }

      drawText(window, font, "Direction", 22, {840.f, 306.f});
      drawButton(window, font, rotateButton);
      drawText(window, font, "Use R to rotate and 1-4 to pick size.", 16,
               {840.f, 414.f}, sf::Color(71, 85, 105));
      drawButton(window, font, finishButton);
    } else if (screen == Screen::Handoff) {
      sf::RectangleShape panel({520.f, 260.f});
      panel.setPosition({380.f, 250.f});
      panel.setFillColor(sf::Color::White);
      panel.setOutlineThickness(1.f);
      panel.setOutlineColor(sf::Color(203, 213, 225));
      window.draw(panel);
      drawCenteredText(window, font, handoffMessage, 28,
                       {{410.f, 300.f}, {460.f, 60.f}});
      drawCenteredText(window, font,
                       "The next player should continue from here.", 18,
                       {{410.f, 374.f}, {460.f, 42.f}}, sf::Color(71, 85, 105));
      drawButton(window, font, continueButton);
    } else if (screen == Screen::Battle) {
      drawBoard(window, font, players[static_cast<std::size_t>(activePlayer)],
                ownBattleOrigin, true, "Your waters");
      drawBoard(window, font,
                players[static_cast<std::size_t>(1 - activePlayer)],
                enemyBattleOrigin, false, "Enemy waters");
      drawText(window, font,
               "Player " + std::to_string(activePlayer + 1) + " turn", 22,
               {68.f, 660.f});
    } else if (screen == Screen::Finished) {
      sf::RectangleShape panel({560.f, 280.f});
      panel.setPosition({360.f, 238.f});
      panel.setFillColor(sf::Color::White);
      panel.setOutlineThickness(1.f);
      panel.setOutlineColor(sf::Color(203, 213, 225));
      window.draw(panel);
      drawCenteredText(window, font,
                       "Player " + std::to_string(winner + 1) + " wins", 34,
                       {{390.f, 296.f}, {500.f, 70.f}}, sf::Color(15, 23, 42));
      drawCenteredText(window, font, "All enemy ships are destroyed.", 19,
                       {{390.f, 382.f}, {500.f, 44.f}}, sf::Color(71, 85, 105));
      drawButton(window, font, restartButton);
    }

    window.display();
  }

  return 0;
}

} // namespace frontend
