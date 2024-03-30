#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Player.h"
#include "Enemy.h"

class Level {
public:
  Level();
  Level(std::string texturePath, std::string collisionMapPath);
  ~Level();

  /**
   * @brief Get the Level End
   *
   * @return float The marked end of the currently loaded level.
   */
  float getLevelEnd() { return levelEnd; }

  void update();
  void draw(sf::RenderWindow& window);
  void handleEvent(sf::Event event);
  bool checkCollision(int x, int y);

  void reset();
  void endLevel(bool win);
private:
  Player player;
  std::vector<Enemy> enemies;

  sf::Texture texture;
  sf::Texture coinTexture;
  sf::Image collisionMap;
  std::vector<sf::Sprite> coins;

  /**
   * @brief the internal clock of the level.
   */
  sf::Clock clock;

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
