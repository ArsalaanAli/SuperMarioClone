#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "Player.h"

/**
 * @brief Round a float away from zero.
 *
 * @param x The float to round.
 * @return the rounded float.
 */
int roundAwayFromZero(float x) { return x < 0 ? floor(x) : ceil(x); }

Player::Player() {
  shape = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
  shape.setFillColor(sf::Color::Black);
  shape.setPosition(0, 0);
  isDying = false;
  vx = 0;
  vy = 0;
  cmap = sf::Image();
}

Player::Player(int cx, int cy, const sf::Image& collisionMap) {
  shape = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
  shape.setFillColor(sf::Color::Black);
  shape.setPosition(cx, cy);
  cmap = collisionMap;
  isDying = false;
  vx = 0;
  vy = 0;
}

Player::~Player() {}

sf::RectangleShape Player::getShape() { return shape; }

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(shape, states);
}

bool Player::checkCollision(int x, int y) {
  sf::Vector2u size = cmap.getSize();
  if (x > 0 && x < static_cast<int>(size.x) && y > 0 && y < static_cast<int>(size.y))
    return cmap.getPixel(x, y) == sf::Color::Red;
  return false;
}

bool Player::isGrounded() {
  sf::Vector2<float> pos = shape.getPosition();
  sf::Vector2<float> size = shape.getSize();

  for (int i = 0; i < shape.getSize().x; i++) {
    if (checkCollision(pos.x + i, pos.y + size.y + 1)) {
      // snap to ground
      float newY = pos.y + size.y;
      while (checkCollision(pos.x + i, newY)) {
        newY -= 1;
      }
      shape.setPosition(pos.x, newY - CELL_SIZE);

      vy = 0;
      return true;
    }
  }
  return false;
}

void Player::processInput(sf::Vector2<int> input, bool grounded, float dt) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && grounded) {
    jump();
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
    die();
  }

  // handle horizontal movement
  if (input.x) {
    // accelerate
    vx += input.x * ACCEL_RATE * dt;
    vx = std::min(vx, MAX_SPEED);
    vx = std::max(vx, -MAX_SPEED);
  } else {
    // decelerate
    if (abs(vx) > DECEL_RATE * dt)
      vx -= DECEL_RATE * ((vx > 0) - (vx < 0)) * dt;
    else
      vx = 0;
  }
}

bool Player::shouldDie() {
  // check if player is below the screen
  return shape.getPosition().y >= 665;
}

void Player::jump() { vy = JUMP_FORCE; }

void Player::die() {
  if (!isDying) {
    // on first invocation, jump
    jump();
    isDying = true;
  }

  // disable horizontal movement
  vx = 0;
}

// Update called once per gameloop
void Player::update(const FrameState& state) {
  float dt = state.deltaTime;
  sf::Vector2<int> input = state.input;

  bool grounded = isGrounded();

  if (shouldDie() || isDying) {
    die();

    // once going down from jump, trigger reset level
    if (vy <= 0) {
      Game::endLevel(false);
    }
  } else {
    processInput(input, grounded, dt);
  }

  // handle vertical movement and gravity
  if (!grounded) {
    bool falling = vy <= 0 || !sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    vy -= AIR_DECEL_RATE * (falling ? 2.0f : 1.0f) * dt;
    vy = std::min(vy, MAX_AIR_SPEED);
    vy = std::max(vy, -MAX_AIR_SPEED);
  }

  // move player based on velocity
  MovePlayer(vx * dt, -vy * dt);
}

void Player::MovePlayer(float xoffset, float yoffset) {
  sf::Vector2<float> pos = shape.getPosition();
  sf::Vector2<float> size = shape.getSize();

  int newX = roundAwayFromZero(xoffset);

  // check for collision on left and right sides
  for (int i = 0; i <= size.y - 3; i++) {
    if (checkCollision(pos.x + newX, pos.y + i) ||
      checkCollision(pos.x + size.x + newX, pos.y + i)) {
      // if collision, ignore x movement
      xoffset = 0;
      break;
    }
  }

  int newY = roundAwayFromZero(yoffset);

  // check for collision on top and bottom sides
  for (int i = 0; i <= size.x; i++) {
    if (checkCollision(pos.x + i, pos.y + newY)) {
      // if collision, invert y movement
      yoffset = abs(yoffset);
      vy = -yoffset;
      break;
    }
  }

  // move player
  shape.setPosition(pos.x + xoffset, pos.y + yoffset);
}
