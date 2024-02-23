#include "Enemy.h"
#include <iostream>

#define MAX_SPEED 400.0f
#define DECEL_RATE 3000.0f
#define ACCEL_RATE 3000.0f
#define JUMP_FORCE 1150.0f
#define AIR_DECEL_RATE (DECEL_RATE * 1.0f)
#define MAX_AIR_SPEED (MAX_SPEED * 5.0f)

int Enemy::roundAwayFromZero(float x) { return x < 0 ? floor(x) : ceil(x); }

Enemy::Enemy(int cx, int cy) {
  shape = sf::RectangleShape(sf::Vector2f(50.0f, 50.0f));
  shape.setFillColor(sf::Color::Red);
  shape.setPosition(1665, 0);

  vx = MAX_SPEED;
  vy = MAX_AIR_SPEED;
}

Enemy::~Enemy() {}

void Enemy::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(shape, states);
}

bool Enemy::isGrounded(GameState &state) {
  sf::Vector2<float> pos = shape.getPosition();
  sf::Vector2<float> size = shape.getSize();

  for (int i = 0; i < size.x; i++) {
    if (state.checkCollision(pos.x + i, pos.y + size.y + 1)) {
      float newY = pos.y + size.y;
      while (state.checkCollision(pos.x + i, newY)) {
        newY -= 1;
      }

      shape.setPosition(pos.x, newY - 50);

      vy = 0;
      return true;
    }
  }

  vy = MAX_AIR_SPEED;
  return false;
}

bool Enemy::checkPlayerCollision(float px, float py) {
  sf::Vector2<float> pos = shape.getPosition();

  float distance = ((pos.x - px) * (pos.x - px)) + ((pos.y - py) * (pos.y - py));
  return distance < 50 * 50;
}

void Enemy::update(GameState &state) {
  float dt = state.getDeltaTime();
  isGrounded(state);

  MoveEnemy(vx * dt, vy * dt, state);
}

void Enemy::MoveEnemy(float xoffset, float yoffset, GameState &state) {
  sf::Vector2<float> pos = shape.getPosition();
  sf::Vector2<float> size = shape.getSize();

  int newX = roundAwayFromZero(xoffset);
  for (int i = 0; i <= size.y - 3; i++) {
    if (state.checkCollision(pos.x + newX, pos.y + i) ||
        state.checkCollision(pos.x + size.x + newX, pos.y + i)) {
      xoffset *= -1;
      vx *= -1;
      break;
    }
  }

  int newY = roundAwayFromZero(yoffset);
  for (int i = 0; i <= size.x; i++) {
    if (state.checkCollision(pos.x + i, pos.y + newY)) {
      yoffset = abs(yoffset);
      vy = -yoffset;
      break;
    }
  }

  shape.setPosition(pos.x + xoffset, pos.y + yoffset);
}
