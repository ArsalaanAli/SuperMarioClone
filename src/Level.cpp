#include <iostream>

#include "Level.h"

Level::Level() : player(Player(50, 50)) {}

Level::Level(std::string texturePath, std::string collisionMapPath) : player(Player(50, 50)) {
  if (!texture.loadFromFile("assets/map1.png")) {
    std::cerr << "Failed to load background texture!" << std::endl;
    exit(1);
  }

  if (!collisionMap.loadFromFile(collisionMapPath)) {
    std::cerr << "Failed to load collision map file!" << std::endl;
    exit(1);
  }
}

Level::~Level() {}

void Level::update() {
  player.update();
}

void Level::draw(sf::RenderWindow& window) {
  // TODO: can't make this a class var for some reason
  window.draw(sf::Sprite(texture));
  window.draw(player.getShape());
}

void Level::handleEvent(sf::Event event) {}

bool Level::checkCollision(int x, int y) {
  sf::Vector2u size = collisionMap.getSize();
  if (x > 0 && x < static_cast<int>(size.x) && y > 0 && y < static_cast<int>(size.y))
    return collisionMap.getPixel(x, y) == sf::Color::Red;
  return false;
}

void Level::reset() {
  std::cout << "Resetting level..." << std::endl;
}

void Level::endLevel(bool win) {
  if (win) {
    std::cout << "You win!" << std::endl;
    // TODO: go to next level
    reset();
  } else {
    reset();
  }
}
