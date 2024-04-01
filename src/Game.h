#ifndef GAME_H
#define GAME_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Level.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define CELL_SIZE 50.0f
#define VIEW_SCROLL_MARGIN 400
#define VIEW_SCROLL_MARGIN_FROM_CENTER \
  ((WINDOW_WIDTH / 2.0f) - VIEW_SCROLL_MARGIN)
#define EASY 1
#define MEDIUM 2
#define HARD 3

enum Scene {
  MainMenu,
  DifficultySelect,
  Running,
  Paused,
  Ending
};

class Game {
  static Game* instance;

  /**
   * @brief Construct a new Game object
   */
  Game();
public:
  // Delete copy constructor for singleton
  Game(const Game& obj)
    = delete;

  /**
   * @brief Destroy the Game object
   */
  ~Game();

  static Game* getInstance();

  static void cleanup();

  /**
   * @brief Run the game loop.
   */
  void run();

  /**
   * @brief Draw the main menu.
   *
   * @param window The window to draw the main menu to.
   */
  void drawMainMenu(sf::RenderWindow& window);

  /**
   * @brief Draw the difficulty select screen.
   * @param window
   */
  void drawDifficultySelect(sf::RenderWindow& window);

  /**
   * @brief Draw the pause menu.
   *
   * @param window The window to draw the pause menu to.
   */
  void drawPauseMenu(sf::RenderWindow& window);

  /**
   * @brief Draw the ending screen.
   *
   * @param window The window to draw the ending screen to.
   */
  void drawEndingScreen(sf::RenderWindow& window);

  /**
   * @brief Draw the HUD.
   * @param window The window to draw the HUD to.
   * @param time The time elapsed in the level.
   * @param coinsCollected The number of coins collected.
   * @param lives The number of lives remaining.
   * @param score The player's score.
   */
  void drawHud(sf::RenderWindow& window, int time, int coinsCollected, int lives, int score);

  /**
   * @brief Get the Delta Time object
   *
   * @return The time since the last frame.
   */
  float getDeltaTime();

  /**
   * @brief Get the Input Axis object
   *
   * @return The input axis as a 2D vector.
   */
  sf::Vector2<int> getInputAxis();

  /**
   * @brief Get the Level object
   *
   * @return The current level.
   */
  Level* getLevel();

  /**
   * @brief Get the Font
   *
   * @return sf::Font* The game's font.
   */
  sf::Font* getFont();

  /**
   * @brief Get the Secondary Font
   *
   * @return sf::Font* The game's secondary font.
   */
  sf::Font* getSecondaryFont();

  /**
   * @brief Get the Window object
   *
   * @return sf::RenderWindow* The game's window.
   */
  sf::RenderWindow* getWindow();

  /**
   * @brief Set the current scene.
   * @param target The scene to set.
   */
  void setScene(Scene target);
private:
  int difficulty;

  /**
   * @brief the internal clock of game loop. Used to calculate deltaTime.
   */
  sf::Clock dt_clock;

  /**
   * @brief Background music player.
   */
  sf::Music bgm;

  /**
   * @brief The current time since the last frame.
   */
  float deltaTime;

  /**
   * @brief The input axis as a 2D vector.
   */
  sf::Vector2<int> input;

  /**
   * @brief The game's window.
   */
  sf::RenderWindow* window;

  /**
   * @brief The current scene.
   */
  Scene scene;

  /**
   * @brief The game's font.
   */
  sf::Font font;

  /**
   * @brief The game's secondary font.
   */
  sf::Font secondaryFont;

  /**
   * @brief The defualt texture of the background. Same as level 1 background.
   */
  sf::Texture backgroundTexture;

  /**
   * @brief The selection indicator in the pause menu.
   */
  sf::ConvexShape selector;

  /**
   * @brief The current menu item selected on pause.
   */
  int selectedMenuItem;

  /**
   * @brief Update the input axis based on the current input.
   *
   * @return The updated input axis as a 2D vector.
   */
  sf::Vector2<int> calculateInputAxis();

  /**
   * @brief The current level.
   */
  Level level;
};

#endif // GAME_H
