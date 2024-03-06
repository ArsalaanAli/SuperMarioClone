#include "Player.h"
#include "GameState.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <iostream>

#define MAX_SPEED 400.0f
#define DECEL_RATE 3000.0f
#define ACCEL_RATE 3000.0f
#define JUMP_FORCE 1150.0f
#define AIR_DECEL_RATE (DECEL_RATE * 1.0f)
#define MAX_AIR_SPEED (MAX_SPEED * 5.0f)

#define GROUND_HEIGHT (620 - 50)

int roundAwayFromZero(float x) { return x < 0 ? floor(x) : ceil(x); }

Player::Player(int cx, int cy) {
  shape = sf::RectangleShape(sf::Vector2f(50.0f, 50.0f));
  shape.setFillColor(sf::Color::Black);
  shape.setPosition(cx, cy);
  isDying = false;
  vx = 0;
  vy = 0;
}

Player::~Player() {}

sf::RectangleShape Player::getShape() { return shape; }

// compatibility for windows.draw(player)
void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(shape, states);
}

bool Player::isGrounded(GameState &state) {
  sf::Vector2<float> pos = shape.getPosition();
  sf::Vector2<float> size = shape.getSize();

  for (int i = 0; i < shape.getSize().x; i++) {
    if (state.checkCollision(pos.x + i, pos.y + size.y)) {
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

  if (input.x) {
    vx += input.x * ACCEL_RATE * dt;
    vx = std::min(vx, MAX_SPEED);
    vx = std::max(vx, -MAX_SPEED);
  } else {
    if (abs(vx) > DECEL_RATE * dt)
      vx -= DECEL_RATE * ((vx > 0) - (vx < 0)) * dt;
    else
      vx = 0;
  }
}

bool Player::shouldDie() {
  // TODO: include check for enemy collision and any other death conditions
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
void Player::update(GameState &state) {
  float dt = state.getDeltaTime();
  sf::Vector2<int> input = state.getInputAxis();
  bool grounded = isGrounded(state);

  if (shouldDie() || isDying) {
    die();

    // once going down from jump, reset level
    if (vy <= 0) {
      state.endLevel(false);
    }
  } else {
    processInput(input, grounded, dt);
  }

  if (!grounded) {
    bool falling = vy <= 0 || !sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    vy -= AIR_DECEL_RATE * (falling ? 2.0f : 1.0f) * dt;
    vy = std::min(vy, MAX_AIR_SPEED);
    vy = std::max(vy, -MAX_AIR_SPEED);
  }

  MovePlayer(vx * dt, -vy * dt, state);

  sf::Vector2<float> pos = shape.getPosition();

  std::cout << vx << "," << vy << "\n";
  std::cout << pos.x << "," << pos.y << "\n";
}

void Player::MovePlayer(float xoffset, float yoffset, GameState &state) {
  sf::Vector2<float> pos = shape.getPosition();
  sf::Vector2<float> size = shape.getSize();

  int newX = roundAwayFromZero(xoffset);
  for (int i = 0; i <= size.y - 3; i++) {
    if (state.checkCollision(pos.x + newX, pos.y + i) ||
        state.checkCollision(pos.x + size.x + newX, pos.y + i)) {
      xoffset = 0;
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
