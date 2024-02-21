#include "Player.h"
#include "GameState.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"
#include <cstdlib>
#include <iostream>

#define MAX_SPEED 0.3f

Player::Player(int cx, int cy) {
  x = cx;
  y = cy;
  shape = sf::RectangleShape(sf::Vector2f(50.0f, 50.0f));
  shape.setFillColor(sf::Color::Black);
  shape.setPosition(0, 620 - 50);
}

// compatibility for windows.draw(player)
void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(shape, states);
}

Player::~Player() {}

void Player::update(GameState &state) {
  float dt = state.getDeltaTime();
  sf::Vector2<int> input = state.getInputAxis();
  std::cout << input.x << "," << input.y << "\n";

  const float DECEL_RATE = MAX_SPEED * 2.5f;
  const float ACCEL_RATE = MAX_SPEED * 2.5f;

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

  // if (!input.y) {
  //   vy -= DECEL_RATE * ((vy > 0) - (vy < 0));
  // } else {
  //   vy = std::max(vy + input.y * 1.0f, DECEL_RATE);
  // }

  std::cout << vx << "," << vy << "\n";

  shape.move(vx, input.y);
}
