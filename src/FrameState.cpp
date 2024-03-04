#include "FrameState.h"
#include "GameState.h"
#include "SFML/Window/Keyboard.hpp"

FrameState::FrameState() { clock = sf::Clock(); }

FrameState::~FrameState() {}

float FrameState::getDeltaTime() { return deltaTime; }

sf::Vector2<int> FrameState::getInputAxis() { return input; }

bool FrameState::getReset() { return resetLevel; }

bool FrameState::getWin() { return win; }

sf::Vector2<int> FrameState::updateInputAxis() {
  int dir_x = 0, dir_y = 0;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    dir_x += 1;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    dir_x += -1;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    dir_y += 1;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    dir_y += -1;
  }

  return sf::Vector2<int>(dir_x, dir_y);
}

void FrameState::endLevel(bool isWin) {
  resetLevel = true;
  win = isWin;
}

bool FrameState::checkLevelCollision(int x, int y) {
  return GameState::em.checkCollisionMap(x, y);
}

void FrameState::update() {
  deltaTime = clock.restart().asSeconds();
  input = updateInputAxis();
  resetLevel = false;
}
