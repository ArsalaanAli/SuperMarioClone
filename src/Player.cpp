#include "Player.h"
#include "GameState.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"

#define MAX_SPEED 200.0f

Player::Player(int cx, int cy) {
  x = cx;
  y = cy;
  shape = sf::RectangleShape(sf::Vector2f(50.0f, 50.0f));
  shape.setFillColor(sf::Color::Black);
  shape.setPosition(0, 0);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(shape, states);
}

Player::~Player() {}

void Player::update(GameState &state) {
  float dt = state.getDeltaTime();
  sf::Vector2<int> input = state.getInputAxis();

  vx += std::max(10.0f * input.x, 200.0f);
  vy += std::max(10.0f * input.y, 200.0f);

  shape.move(x * dt, y * dt);
}
