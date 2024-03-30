#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Player.h"

class Level {
public:
  Level();
  Level(std::string texturePath, std::string collisionMapPath);
  ~Level();

  void update();
  void draw(sf::RenderWindow& window);
  void handleEvent(sf::Event event);
  bool checkCollision(int x, int y);

  void reset();
  void endLevel(bool win);
private:
  Player player;
  // std::vector<Enemy> enemies;

  sf::Texture texture;
  sf::Image collisionMap;

  // void updateViewScroll()
};

#endif // LEVEL_H
