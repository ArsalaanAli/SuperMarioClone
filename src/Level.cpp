#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

#include "Game.h"
#include "Level.h"

Level::Level() : player(Player(WINDOW_HEIGHT / 4, -CELL_SIZE * 2)) {}

Level::Level(std::string texturePath, std::string collisionMapPath, int difficultyInput)
    : player(Player(WINDOW_HEIGHT / 4, -CELL_SIZE * 2))
{

  Game::getInstance()->getWindow()->setView(
    Game::getInstance()->getWindow()->getDefaultView());

  difficulty = difficultyInput;

  if (!texture.loadFromFile("assets/map1.png")) {
    std::cerr << "Failed to load background texture!" << std::endl;
    exit(1);
  }
  levelEnd = texture.getSize().x;
  levelFinish = 9480;

  if (!collisionMap.loadFromFile(collisionMapPath)) {
    std::cerr << "Failed to load collision map file!" << std::endl;
    exit(1);
  }

  if (!img.loadFromFile("assets/map1.png")) {
    std::cerr << "Failed to load img file!" << std::endl;
    exit(1);
  }

  for (auto& spawnPoint : eSpawnPoints[difficulty-1]) {
    enemies.emplace_back(spawnPoint, 600);
  }

  initCoins();

  lives = difficultyLives[difficulty-1];
  kills = 0;
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
  if (coins.size() > 0) {
    for (auto& coin : coins) {
      if (coin.getGlobalBounds().intersects(
        player.getShape().getGlobalBounds())) {
        coin.move(0, -WINDOW_HEIGHT);
        coinsCollected++;

        switch (coinsCollected) {
        case 5:
        case 10:
        case 15:
          lives++;
          break;
        }
      }
    }
  }

  for (auto& powerUp : powerUps) {
    if (powerUp.getGlobalBounds().intersects(
      player.getShape().getGlobalBounds())) {
      powerUp.move(0, -WINDOW_HEIGHT);
      player.activatePowerup();
    }
  }

  window->setView(updateLevelScroll(window->getView(), levelEnd, player));

  if (player.getShape().getPosition().x > levelFinish) {
    endLevel(true);
  }
}

void Level::initCoins() {
  if (!coinTexture.loadFromFile("assets/coin.png")) {
    std::cerr << "Failed to load coin texture!" << std::endl;
    exit(1);
  }

  coinsCollected = 0;
  coins.clear();
  for (int i = 500; i < levelEnd; i += 1000) {
    sf::Sprite coin;
    // 75% chance of spawning a coin
    if (rand() % 100 > 75) {
      continue;
    }

    // random y position
    int _y[] = { 250, 300, 555 };
    int y = _y[rand() % 3];

    // random x position within 25 pixels of i
    int x = i + (rand() % 50 - 25);

    coin.setTexture(coinTexture);
    coin.setPosition(x, y);
    coins.push_back(coin);
  }
}

void Level::drawCoins(sf::RenderWindow& window) {
  if (coins.size() < 1) {
    return;
  }
  for (auto& coin : coins) {
    coin.setTexture(coinTexture);
    window.draw(coin);
  }
}

void Level::draw(sf::RenderWindow& window) {
  // TODO: can't make this a class var for some reason
  sf::Sprite sprite = sf::Sprite(texture);
  window.draw(sprite);

  drawCoins(window);
  for (auto& pup : powerUps) {
    window.draw(pup);
  }
  for (auto& enemy : enemies) {
    window.draw(enemy);
  }
  window.draw(player);

  int time = clock.getElapsedTime().asMilliseconds();
  int distance = player.getShape().getPosition().x;
  int score = coinsCollected * 500 + distance / 2 + kills * 1000;
  drawHUD(window, time, coinsCollected, lives, score);
}

void Level::handleEvent(sf::Event event) {
  if (event.type == sf::Event::KeyPressed) {
    switch (event.key.code) {
    case sf::Keyboard::Q:
      player.die();
      break;
    case sf::Keyboard::Period:
      cout << "Player position: " << player.getShape().getPosition().x << ", "
        << player.getShape().getPosition().y << std::endl;
      break;
    default:
      break;
    }
  }
}

bool Level::checkCollision(int x, int y) {
  sf::Vector2u size = collisionMap.getSize();
  if (x > 0 && x < static_cast<int>(size.x) && y > 0 &&
    y < static_cast<int>(size.y))
    return collisionMap.getPixel(x, y) == sf::Color::Red;
  return false;
}

void Level::reset() {
  Game::getInstance()->getWindow()->setView(
    Game::getInstance()->getWindow()->getDefaultView());
  clock.restart();
  player = Player(WINDOW_HEIGHT / 4, -CELL_SIZE * 2);
  for (auto& enemy : enemies) {
    enemy.reset();
  }
  powerUps.clear();
  kills = 0;
  for (auto& coin : coins) {
    if (coin.getPosition().y < 0)
      coin.move(0, WINDOW_HEIGHT);
  }
  coinsCollected = 0;
  lives--;
}

void Level::endLevel(bool win) {
  if (win) {
    // TODO: go to next level
    Game::getInstance()->setScene(Scene::MainMenu);
  } else {
    if (lives > 0) {
      reset();
    } else {
      Game::getInstance()->setScene(Scene::MainMenu);
    }
  }
}

void Level::checkPowerUp() {
  // 20% chance of spawning a powerup
  if (rand() % 100 > 20) {
    return;
  }

  sf::Color powerUp = sf::Color(4237834495);
  sf::Vector2f pos = player.getShape().getPosition();

  for (int i = 0; i < CELL_SIZE; i++) {
    if (img.getPixel(pos.x + (player.getShape().getGlobalBounds().width / 2),
      pos.y - i) == powerUp) {
      // hit [?] Block
      sf::Sprite powerup;
      powerup.setTexture(coinTexture);
      powerup.setColor(sf::Color::Green);
      powerup.scale(0.8, 0.8);

      powerup.setPosition(pos.x, pos.y - CELL_SIZE * 2);

      powerUps.push_back(powerup);
      cout << "Powerup spawned" << endl;
      return;
    }
  }
}


void Level::drawHUD(sf::RenderWindow& window, int time, int coinsCollected,
  int lives, int score) {
  sf::Vector2f windowCenter = window.getView().getCenter();
  sf::Font font = *Game::getInstance()->getFont();
  sf::Font secondaryFont = *Game::getInstance()->getSecondaryFont();

  std::string Time = "TIME " + std::to_string(time);
  sf::Text timeText(Time, secondaryFont, 30);
  timeText.setPosition(windowCenter.x - 400,
    windowCenter.y - WINDOW_HEIGHT / 2 + 10);
  timeText.setFillColor(sf::Color(255, 255, 0));
  window.draw(timeText);

  std::string coins = "COINS " + std::to_string(coinsCollected);
  sf::Text coinsText(coins, secondaryFont, 30);
  coinsText.setPosition(windowCenter.x - 400, 75);
  coinsText.setFillColor(sf::Color(255, 255, 0));
  window.draw(coinsText);

  std::string Lives = "LIVES " + std::to_string(lives);
  sf::Text livesText(Lives, secondaryFont, 30);
  livesText.setPosition(windowCenter.x + 100,
    windowCenter.y - WINDOW_HEIGHT / 2 + 10);
  livesText.setFillColor(sf::Color(255, 255, 0));
  window.draw(livesText);

  std::string Score = "SCORE " + std::to_string(score);
  sf::Text scoreText(Score, secondaryFont, 30);
  scoreText.setPosition(windowCenter.x + 100, 75);
  scoreText.setFillColor(sf::Color(255, 255, 0));
  window.draw(scoreText);
}
