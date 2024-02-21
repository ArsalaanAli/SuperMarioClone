#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <tuple>

class GameState {
public:
  GameState();
  void runGame();
  float getDeltaTime();
  sf::Vector2<int> getInputAxis();

private:
  sf::Clock clock;
  float deltaTime;

  sf::Vector2<int> input;
  sf::Vector2<int> updateInputAxis();
};

#endif // GAMESTATE_H
