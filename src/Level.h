#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Player.h"
#include "Enemy.h"

class Level {
public:
  /**
   * @brief Construct a new Level object
   */
  Level();

  /**
   * @brief Construct a new Level object
   *
   * @param texturePath The path to the texture of the level.
   * @param collisionMapPath The path to the collision map of the level.
   */
  Level(std::string texturePath, std::string collisionMapPath);

  /**
   * @brief Destroy the Level object
   */
  ~Level();

  /**
   * @brief Get the Level End
   *
   * @return float The marked end of the currently loaded level.
   */
  float getLevelEnd() { return levelEnd; }

  /**
   * @brief The update loop of the level. Called every frame.
   */
  void update();

  /**
   * @brief Draw the level to the window.
   *
   * @param window The window to draw the level to.
   */
  void draw(sf::RenderWindow& window);

  /**
   * @brief Handle events in the level.
   *
   * @param event The event to handle.
   */
  void handleEvent(sf::Event event);

  /**
   * @brief Check for collisions with the level at a given point.
   *
   * @param x The x coordinate to check.
   * @param y The y coordinate to check.
   * @return if there is a collision at the given point.
   */
  bool checkCollision(int x, int y);

  /**
   * @brief Reset the level.
   */
  void reset();

  /**
   * @brief End the level.
   *
   * @param win If the player won the level. If false, resets the level.
   */
  void endLevel(bool win);

  /**
   * @brief Check if a powerup should be spawned.
   */
  void checkPowerUp();

private:
  /**
   * @brief the internal clock of the level.
   */
  sf::Clock clock;

  /**
   * @brief The player object.
   */
  Player player;
  /**
   * @brief The collection of enemies in the level.
   */
  std::vector<Enemy> enemies;

  /**
   * @brief The texture of the level.
   */
  sf::Texture texture;

  /**
   * @brief The image of the level.
   */
  sf::Image img;

  /**
   * @brief The collision map of the level.
   */
  sf::Image collisionMap;

  /**
   * @brief The texture of the coins.
   *
   */
  sf::Texture coinTexture;
  /**
   * @brief The collection of coins in the level.
   */
  std::vector<sf::Sprite> coins;

  /**
   * @brief The texture of the powerups.
   */
  sf::Texture powerUpTexture;

  /**
   * @brief The collection of powerups in the level.
   */
  std::vector<sf::Sprite> powerUps;

  /**
   * @brief The marked end of the currently loaded level.
   */
  float levelEnd;

  /**
   * @brief The finish line of the level.
   */
  int levelFinish;

  /**
   * @brief The number of kills the player has in the current run.
   */
  int kills;

  /**
   * @brief The number of lives the player has remaining.
   */
  int lives;

  /**
   * @brief The player's score.
   */
  int coinsCollected;

  /**
   * @brief Helper function to create coins.
   */
  void makeCoins();

  void initCoins();

  void drawCoins(sf::RenderWindow &window);

  /**
   * @brief Helper function to draw the HUD.
   *
   * @param window The window to draw the HUD to.
   * @param time The time elapsed in the level.
   * @param coinsCollected The number of coins collected.
   * @param lives The number of lives remaining.
   * @param score The player's score.
   */
  void drawHUD(sf::RenderWindow& window, int time, int coinsCollected, int lives, int score);
};

#endif // LEVEL_H
