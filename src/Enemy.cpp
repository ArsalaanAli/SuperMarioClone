#if 1
#else

#include <iostream>
#include <cmath>
#include <SFML/Graphics/Color.hpp>

#include "Enemy.h"

int Enemy::roundAwayFromZero(float x) { return x < 0 ? floor(x) : ceil(x); }

Enemy::Enemy(int cx, int cy) {
  spawn_x = cx;
  spawn_y = cy;

  shape = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
  shape.setFillColor(sf::Color::Red);
  shape.setPosition(cx, cy);

  vx = MAX_SPEED;
  vy = MAX_AIR_SPEED;

  isDying = false;
}

Enemy::~Enemy() {}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(shape, states);
}

void Enemy::reset() {
  shape = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
  shape.setFillColor(sf::Color::Red);
  shape.setPosition(spawn_x, spawn_y);

  vx = MAX_SPEED;
  vy = MAX_AIR_SPEED;

  isDying = false;
}

bool Enemy::isGrounded(GameState& state) {
  sf::Vector2<float> pos = shape.getPosition();
  sf::Vector2<float> size = shape.getSize();

  for (int i = 0; i < size.x; i++) {
    if (state.checkCollision(pos.x + i, pos.y + size.y + 1)) {
      // snap to ground
      float newY = pos.y + size.y;
      while (state.checkCollision(pos.x + i, newY)) {
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
    shape.setFillColor(sf::Color::Yellow); // replace with change to dead texture
    isDying = true;
  }

  // disable horizontal movement
  vx = 0;
}

void Enemy::update(GameState& state) {
  float dt = state.getDeltaTime();
  isGrounded(state);

  // move the enemy based on its velocity
  MoveEnemy(vx * dt, vy * dt, state);
}

void Enemy::MoveEnemy(float xoffset, float yoffset, GameState& state) {
  sf::Vector2<float> pos = shape.getPosition();
  sf::Vector2<float> size = shape.getSize();

  int newX = roundAwayFromZero(xoffset);

  // check for collision on left and right sides
  for (int i = 0; i <= size.y - 3; i++) {
    if (state.checkCollision(pos.x + newX, pos.y + i) ||
      state.checkCollision(pos.x + size.x + newX, pos.y + i)) {
      // if collision, invert x movement
      xoffset *= -1;
      vx *= -1;
      break;
    }
  }

  int newY = roundAwayFromZero(yoffset);

  // check for collision on top and bottom sides
  for (int i = 0; i <= size.x; i++) {
    if (state.checkCollision(pos.x + i, pos.y + newY)) {
      // if collision, invert y movement
      yoffset = abs(yoffset);
      vy = -yoffset;
      break;
    }
  }

  // move player
  shape.setPosition(pos.x + xoffset, pos.y + yoffset);
}

#endif // 0
