#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

enum Scene {
  MainMenu,
  Running,
  Paused
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

  /**
   * @brief Run the game loop.
   */
  void run();

  /**
   * @brief Start the level.
   * Called when the player starts a new game from the main menu.
   */
  void setScene();

  /**
   * @brief Draw the main menu.
   *
   * @param window The window to draw the main menu to.
   */
  void drawMainMenu(sf::RenderWindow& window);

  /**
   * @brief Draw the pause menu.
   *
   * @param window The window to draw the pause menu to.
   */
  void drawPauseMenu(sf::RenderWindow& window);

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
   * @brief Set the current scene.
   * @param target The scene to set.
   */
  void setScene(Scene target);
private:
  /**
   * @brief the internal clock of whole game
   */
  sf::Clock clock;

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
};

#endif // GAME_H
