#include <iostream>
#include <cmath>
#include <SFML/Graphics/Color.hpp>

#include "Enemy.h"
#include "Game.h"

int Enemy::roundAwayFromZero(float x) { return x < 0 ? floor(x) : ceil(x); }

Enemy::Enemy(int cx, int cy) {
  if (!texture.loadFromFile("assets/enemy.png")) {
    std::cerr << "Failed to load enemy texture!" << std::endl;
    exit(1);
  }

  spawn_x = cx;
  spawn_y = cy;

  shape = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
  shape.setFillColor(sf::Color::Red);
  shape.setPosition(cx, cy);

  sprite.setTexture(texture);
  sprite.setPosition(cx, cy);

  vx = MAX_SPEED;
  vy = MAX_AIR_SPEED;

  isDying = false;
}

Enemy::~Enemy() {}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite, states);
}

void Enemy::reset() {
  shape = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
  shape.setFillColor(sf::Color::Red);
  shape.setPosition(spawn_x, spawn_y);

  vx = MAX_SPEED;
  vy = MAX_AIR_SPEED;

  isDying = false;
}

bool Enemy::isGrounded() {
  sf::Vector2<float> pos = shape.getPosition();
  sf::Vector2<float> size = shape.getSize();
  Level* level = Game::getInstance()->getLevel();

  for (int i = 0; i < size.x; i++) {
    if (level->checkCollision(pos.x + i, pos.y + size.y + 1)) {
      // snap to ground
      float newY = pos.y + size.y;
      while (level->checkCollision(pos.x + i, newY)) {
        newY -= 1;
      }

      shape.setPosition(pos.x, newY - CELL_SIZE);

      vy = 0;
      return true;
    }
  }

  vy = MAX_AIR_SPEED;
  return false;
}

bool Enemy::checkPlayerCollision(float px, float py) {
  // if the enemy is dying, don't check for collision
  if (isDying)
    return false;

  sf::Vector2<float> pos = shape.getPosition();

  // check if the player is within the enemy's bounding box
  float distance = ((pos.x - px) * (pos.x - px)) + ((pos.y - py) * (pos.y - py));
  return distance < CELL_SIZE * CELL_SIZE;
}

sf::RectangleShape Enemy::getShape() { return shape; }

void Enemy::die() {
  if (!isDying) {
    // on first invocation, change color
    sprite.setScale(1, 0.75);
    sprite.move(0, CELL_SIZE / 4);
    isDying = true;
  }

  // disable horizontal movement
  vx = 0;
}

void Enemy::update() {
  float dt = Game::getInstance()->getDeltaTime();
  isGrounded();

  sprite.setTexture(texture);

  // move the enemy based on its velocity
  if (isDying) return;

  MoveEnemy(vx * dt, vy * dt);
}

void Enemy::MoveEnemy(float xoffset, float yoffset) {
  sf::Vector2<float> pos = shape.getPosition();
  sf::Vector2<float> size = shape.getSize();
  Level* level = Game::getInstance()->getLevel();

  int newX = roundAwayFromZero(xoffset);
  bool collisionX = false;

  //check for br and bl corners of enemy to not fall off edge
  if (!level->checkCollision(pos.x + size.x + 3, pos.y + size.y + 3) ||
    !level->checkCollision(pos.x - 3, pos.y + size.y + 3)) {
    collisionX = true;
  }

  // check for collision on left and right sides
  for (int i = 0; i <= size.y - 3; i++) {
    if (level->checkCollision(pos.x + newX, pos.y + i) ||
      level->checkCollision(pos.x + size.x + newX, pos.y + i) ||
      pos.x + newX < 0 || pos.x + size.x + newX > level->getLevelEnd()) {
      // if collision, invert x movement
      collisionX = true;
      break;
    }
  }

  if (collisionX) {
    xoffset *= -1;
    vx *= -1;
  }

  // move player
  shape.setPosition(pos.x + xoffset, pos.y + yoffset);
  sprite.setPosition(shape.getPosition());
}
