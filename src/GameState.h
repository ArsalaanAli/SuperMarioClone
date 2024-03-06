#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <tuple>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define VIEW_SCROLL_MARGIN 200 
#define VIEW_SCROLL_MARGIN_FROM_CENTER ((WINDOW_WIDTH / 2.0f) - VIEW_SCROLL_MARGIN)

enum GameStateEnum { MainMenu, Running, Paused };

class GameState {
public:
  GameState();
  void runGame();
  float getDeltaTime();
  bool checkCollision(int x, int y);
  sf::Vector2<int> getInputAxis();
  void endLevel(bool win);
  void drawMainMenu(sf::RenderWindow& window);
  void drawPausePopup(sf::RenderWindow& window);

private:
  sf::Clock clock;
  float deltaTime;
  sf::Image collisionMap;
  sf::Vector2<int> input;
  sf::Vector2<int> updateInputAxis();
  float LEVEL_END;
  bool resetLevel;
  GameStateEnum gameState;
  void handlePauseInput(sf::Event event);
  sf::Font font;
  sf::Font secondaryFont;
  int selectedMenuItem;

};

#endif // GAMESTATE_H
