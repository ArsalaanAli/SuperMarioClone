#include <iostream>

#include "Level.h"
#include "Game.h"

Level::Level() : player(Player(50, 50)) {}

Level::Level(std::string texturePath, std::string collisionMapPath) : player(Player(50, 50)) {
  if (!texture.loadFromFile("assets/map1.png")) {
    std::cerr << "Failed to load background texture!" << std::endl;
    exit(1);
  }
  levelEnd = texture.getSize().x;

  if (!collisionMap.loadFromFile(collisionMapPath)) {
    std::cerr << "Failed to load collision map file!" << std::endl;
    exit(1);
  }

  const int eSpawnPoints[] = {
    1500,
    2000,
    2300,
    2500,
    3500,
    3800,
    4500,
    5000,
    5300,
    6300,
    8000,
    8300,
    9200,
  };

  for (auto& spawnPoint : eSpawnPoints) {
    enemies.emplace_back(spawnPoint, 600);
  }
}

Level::~Level() {}

/**
 * @brief Update the view to scroll with the player.
 *
 * @param view The current view.
 * @param LEVEL_END The end of the level.
 * @param player The player object.
 * @return The updated view.
 */
sf::View updateLevelScroll(const sf::View& view, const float& LEVEL_END,
  Player& player) {
  float x = player.getShape().getPosition().x + (CELL_SIZE / 2);
  float viewX = view.getCenter().x;

  // calculate excess from either end as a positive value
  float excess = ((x > viewX) - (x < viewX)) * (x - viewX) -
    VIEW_SCROLL_MARGIN_FROM_CENTER;

  sf::View newView = view;
  // cover excess based on the end excess is on
  if (excess > 0 && x > VIEW_SCROLL_MARGIN &&
    x < LEVEL_END - VIEW_SCROLL_MARGIN) {
    newView.move(((x > viewX) - (x < viewX)) * excess, 0);
  }

  return newView;
}


void Level::update() {
  sf::RenderWindow* window = Game::getInstance()->getWindow();

  player.update();
  for (auto& enemy : enemies) {
    enemy.update();
    std::cout << "Enemy position: " << enemy.getShape().getPosition().x << std::endl;
  }

  window->setView(updateLevelScroll(
    window->getView(), levelEnd, player));
}

void Level::draw(sf::RenderWindow& window) {
  // TODO: can't make this a class var for some reason
  sf::Sprite sprite = sf::Sprite(texture);
  sprite.setPosition(0, 0);

  window.draw(sprite);
  window.draw(player);
  for (auto& enemy : enemies) {
    window.draw(enemy);
  }
}

void Level::handleEvent(sf::Event event) {
  if (event.type == sf::Event::KeyPressed) {
    switch (event.key.code) {
    case sf::Keyboard::Q:
      player.die();
      break;
    case sf::Keyboard::Period:
    std:cout << "Player position: " << player.getShape().getPosition().x << std::endl;
      break;
    default:
      break;
    }
  }
}

bool Level::checkCollision(int x, int y) {
  sf::Vector2u size = collisionMap.getSize();
  if (x > 0 && x < static_cast<int>(size.x) && y > 0 && y < static_cast<int>(size.y))
    return collisionMap.getPixel(x, y) == sf::Color::Red;
  return false;
}

void Level::reset() {
  std::cout << "Resetting level..." << std::endl;
  player = Player(WINDOW_HEIGHT / 4, -CELL_SIZE * 2);
  for (auto& enemy : enemies) {
    enemy.reset();
  }
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
