#define GAMESTATE_H
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <tuple>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define VIEW_SCROLL_MARGIN 200
#define VIEW_SCROLL_MARGIN_FROM_CENTER \
  ((WINDOW_WIDTH / 2.0f) - VIEW_SCROLL_MARGIN)
#define CELL_SIZE 50.0f
#define GROUND_HEIGHT (620 - CELL_SIZE)

enum GameStateEnum {
  MainMenu,
  Running,
  Paused
};

/**
 * @brief The GameState class is responsible for running the game loop and
 * handling input.
 */
class GameState {
public:
  /**
   * @brief Construct a new Game State object
   */
  GameState();

  /**
   * @brief Destroy the Game State object
   */
  ~GameState();

  /**
   * @brief Run the game loop.
   */
  void runGame();

  /**
   * @brief Get the Delta Time object
   *
   * @return The time in seconds since the last frame.
   */
  float getDeltaTime();

  /**
   * @brief Get the Input Axis object
   *
   * @return The input axis as a 2D vector.
   */
  sf::Vector2<int> getInputAxis();

  /**
   * @brief Check if a collision with the level occurs at the given coordinates.
   *
   * @param x The x coordinate.
   * @param y The y coordinate.
   * @return Whether a collision occurs with the level at the given coordinates.
   */
  bool checkCollision(int x, int y);

  /**
   * @brief End the level.
   *
   * @param win Whether the player won the level. If false, the level is incomplete so restart the level
   */
  void endLevel(bool win);
  void drawMainMenu(sf::RenderWindow& window);
  void drawHud(sf::RenderWindow& window, int time, int coinsCollected, int lives, int score);
  void drawPausePopup(sf::RenderWindow& window);
  bool isMouseOverText(sf::RenderWindow& window, sf::Text& text);

private:

  /**
   * @brief the internal clock of game loop
   */
  sf::Clock dt_clock;

  /**
   * @brief the internal clock of whole game
   */
  sf::Clock clock;

  /**
   * @brief The current time since the last frame.
   */
  float deltaTime;

  /**
   * @brief The collision map of the currently loaded level.
   */
  sf::Image collisionMap;

  /**
   * @brief The input axis as a 2D vector.
   */
  sf::Vector2<int> input;

  /**
   * @brief Update the input axis based on the current input.
   *
   * @return The updated input axis as a 2D vector.
   */
  sf::Vector2<int> updateInputAxis();

  /**
   * @brief The marked end of the currently loaded level.
   */
  float LEVEL_END;

  /**
   * @brief Flag to reset the level in gameloop
   */
  bool resetLevel;

  GameStateEnum gameState;
  void handlePauseInput(sf::Event event, sf::RenderWindow& window);
  sf::Font font;
  sf::Font secondaryFont;
  int selectedMenuItem;

  /**
   * @brief The player's current score.
   */
  int coinsCollected;

  /**
   * @brief The player's current lives.
   */
  int lives;

  /**
   * @brief The number of enemies killed.
   */
  int kills;
};

#endif // GAMESTATE_H
