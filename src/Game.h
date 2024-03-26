#ifndef Game_H
#define Game_H

#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <tuple>

#include "FrameState.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define VIEW_SCROLL_MARGIN 200
#define VIEW_SCROLL_MARGIN_FROM_CENTER \
  ((WINDOW_WIDTH / 2.0f) - VIEW_SCROLL_MARGIN)
#define CELL_SIZE 50.0f
#define GROUND_HEIGHT (620 - CELL_SIZE)

enum GameState {
  MainMenu,
  Running,
  Paused
};


/**
 * @brief The Game class is responsible for running the game loop and
 * handling input.
 */
class Game {
public:
  /**
   * @brief Construct a new Game State object
   */
  Game();

  /**
   * @brief Destroy the Game State object
   */
  ~Game();

  /**
   * @brief Run the game loop.
   */
  void run();

  /**
   * @brief End the level.
   *
   * @param win Whether the player won the level. If false, the level is incomplete so restart the level
   */
  static void endLevel(bool win);

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

  void drawMainMenu();
  void drawPausePopup();
  bool isMouseOverText(sf::Text& text);

private:
  /**
   * @brief Flag to reset the level in gameloop
   */
  static bool resetLevel;

  /**
   * @brief The main window of the game.
   */
  sf::RenderWindow* window;

  /**
   * @brief the internal clock of game loop
   */
  sf::Clock clock;

  /**
   * @brief The state of the current frame.
   */
  FrameState fstate;

  /**
   * @brief The collision map of the currently loaded level.
   */
  sf::Image collisionMap;

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

  GameState state;
  void handlePauseInput(sf::Event event);
  sf::Font font;
  sf::Font secondaryFont;
  int selectedMenuItem;
};

#endif // Game_H
