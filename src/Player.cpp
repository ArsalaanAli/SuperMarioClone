#include "Player.h"
#include "GameState.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <iostream>

#define MAX_SPEED 400.0f
#define DECEL_RATE 1500.0f
#define ACCEL_RATE 1500.0f
#define JUMP_FORCE 1150.0f
#define AIR_DECEL_RATE (DECEL_RATE * 2.0f)
#define MAX_AIR_SPEED (MAX_SPEED * 5.0f)

#define GROUND_HEIGHT (620 - 50)

Player::Player(int cx, int cy) {
  shape = sf::RectangleShape(sf::Vector2f(50.0f, 50.0f));
  shape.setFillColor(sf::Color::Black);
  shape.setPosition(0, 0);
}

Player::~Player() {}

sf::RectangleShape Player::getShape() { return shape; }

// compatibility for windows.draw(player)
void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(shape, states);
}

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

void Player::jump() { vy = JUMP_FORCE; }

// Update called once per gameloop
void Player::update(GameState &state) {
  float dt = state.getDeltaTime();
  sf::Vector2<int> input = state.getInputAxis();

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isGrounded()) {
    jump();
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

  if (!isGrounded()) {
    bool falling = vy <= 0 || !sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    vy -= AIR_DECEL_RATE * (falling ? 2.0f : 1.0f) * dt;
    vy = std::min(vy, MAX_AIR_SPEED);
    vy = std::max(vy, -MAX_AIR_SPEED);
  }


  shape.move(vx * dt, -vy * dt);

  std::cout << x << "," << y << "\n";
  std::cout << vx << "," << vy << "\n";
}
