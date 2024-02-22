#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <tuple>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define VIEW_SCROLL_MARGIN 200 
#define VIEW_SCROLL_MARGIN_FROM_CENTER ((WINDOW_WIDTH / 2.0f) - VIEW_SCROLL_MARGIN)

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
