#include <iostream>

#include "Level.h"
#include "Game.h"

Level::Level() : player(Player(WINDOW_HEIGHT / 4, -CELL_SIZE * 2)) {}

Level::Level(std::string texturePath, std::string collisionMapPath) : player(Player(WINDOW_HEIGHT / 4, -CELL_SIZE * 2)) {
  if (!texture.loadFromFile("assets/map1.png")) {
    std::cerr << "Failed to load background texture!" << std::endl;
    exit(1);
  }
  levelEnd = texture.getSize().x;

  if (!collisionMap.loadFromFile(collisionMapPath)) {
    std::cerr << "Failed to load collision map file!" << std::endl;
    exit(1);
  }

  if (!coinTexture.loadFromFile("assets/coin.png")) {
    std::cerr << "Failed to load coin texture!" << std::endl;
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

  makeCoins();
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

    sf::Vector2f pos = player.getShape().getPosition();
    sf::Vector2f epos = enemy.getShape().getPosition();

    // Check for player collision with enemy
    if (enemy.checkPlayerCollision(pos.x, pos.y)) {
      // Check if player is above enemy
      if (pos.y - epos.y < -(CELL_SIZE / 2)) {
        enemy.die();
        player.jump();
      } else {
        player.die();
      }
    }
  }

  for (auto& coin : coins) {
    sf::Vector2f pos = player.getShape().getPosition();
    sf::Vector2f cpos = coin.getPosition();

    if (coin.getGlobalBounds().intersects(player.getShape().getGlobalBounds())) {
      coin.move(0, -WINDOW_HEIGHT);
      coinsCollected++;
    }
  }

  window->setView(updateLevelScroll(
    window->getView(), levelEnd, player));
}

void Level::draw(sf::RenderWindow& window) {
  // TODO: can't make this a class var for some reason
  sf::Sprite sprite = sf::Sprite(texture);
  sprite.setPosition(0, 0);

  window.draw(sprite);
  for (auto& coin : coins) {
    window.draw(coin);
  }
  for (auto& enemy : enemies) {
    window.draw(enemy);
  }
  window.draw(player);
}

void Level::handleEvent(sf::Event event) {
  if (event.type == sf::Event::KeyPressed) {
    switch (event.key.code) {
    case sf::Keyboard::Q:
      player.die();
      break;
    case sf::Keyboard::Period:
    std:cout << "Player position: " << player.getShape().getPosition().x << ", " << player.getShape().getPosition().y << std::endl;
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
  Game::getInstance()->getWindow()->setView(Game::getInstance()->getWindow()->getDefaultView());
  player = Player(WINDOW_HEIGHT / 4, -CELL_SIZE * 2);
  for (auto& enemy : enemies) {
    enemy.reset();
  }
  for (auto& coin : coins) {
    if (coin.getPosition().y < 0)
      coin.move(0, WINDOW_HEIGHT);
  }
  coinsCollected = 0;
}

void Level::endLevel(bool win) {
  if (win) {
    // TODO: go to next level
    reset();
  } else {
    reset();
  }
}

void Level::makeCoins() {
  coinsCollected = 0;
  coins.clear();
  for (int i = 500; i < levelEnd; i += 1000) {
    // 75% chance of spawning a coin
    if (rand() % 100 > 75) {
      continue;
    }

    // TODO: can't render sprite texture
    sf::Sprite coin(coinTexture);

    // random y position
    int _y[] = { 250, 300, 555 };
    int y = _y[rand() % 3];

    // random x position within 25 pixels of i
    int x = i + (rand() % 50 - 25);

    coin.setPosition(x, y);

    coins.push_back(coin);
  }
}
