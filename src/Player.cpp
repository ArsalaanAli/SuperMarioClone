#include "Player.h"
#include "GameState.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"

#define MAX_SPEED 0.3f
#define DECEL_RATE (MAX_SPEED * 2.5f)
#define ACCEL_RATE (MAX_SPEED * 2.5f)
#define AIR_DECEL_RATE (DECEL_RATE * 2.0f)
#define MAX_AIR_SPEED (MAX_SPEED * 2.0f)

#define GROUND_HEIGHT (620 - 50)

Player::Player(int cx, int cy) {
  shape = sf::RectangleShape(sf::Vector2f(50.0f, 50.0f));
  shape.setFillColor(sf::Color::Black);
  shape.setPosition(0, 0);
}

// compatibility for windows.draw(player)
void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(shape, states);
}

Player::~Player() {}

void Player::setPosition(int cx, int cy) { shape.setPosition(x, y); }

bool Player::isGrounded() {
  sf::Vector2<float> pos = shape.getPosition();

  if (pos.y < GROUND_HEIGHT)
    return false;
  if (pos.y > GROUND_HEIGHT) {
    shape.setPosition(pos.x, GROUND_HEIGHT);
    vy = 0;
  }

  return true;
}

void Player::jump() { vy = -2.0f; }

void Player::update(GameState &state) {
  float dt = state.getDeltaTime();
  sf::Vector2<int> input = state.getInputAxis();

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isGrounded()) {
    jump();
  }

  if (!input.x) {
    if (abs(vx) > DECEL_RATE * dt)
      vx -= DECEL_RATE * ((vx > 0) - (vx < 0)) * dt;
    else
      vx = 0;
  } else {
    vx += input.x * ACCEL_RATE * dt;
    vx = std::min(vx, MAX_SPEED);
    vx = std::max(vx, -MAX_SPEED);
  }

  if (!isGrounded()) {
    bool falling = vy > 0 || !sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    vy += AIR_DECEL_RATE * (falling ? 2.0f : 1.0f) * dt;
    vy = std::min(vy, MAX_AIR_SPEED);
    vy = std::max(vy, -MAX_AIR_SPEED);
  }

  shape.move(vx, vy);
}
