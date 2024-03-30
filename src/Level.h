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
  sf::Image collisionMap;

  /**
   * @brief The marked end of the currently loaded level.
   */
  float levelEnd;

  // void updateViewScroll()
};

#endif // LEVEL_H
