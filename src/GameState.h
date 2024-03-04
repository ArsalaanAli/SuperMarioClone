#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <tuple>

#include "EntityManager.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define VIEW_SCROLL_MARGIN 200
#define VIEW_SCROLL_MARGIN_FROM_CENTER                                         \
  ((WINDOW_WIDTH / 2.0f) - VIEW_SCROLL_MARGIN)
#define CELL_SIZE 50.0f

class GameState {

public:
  GameState();
  static EntityManager em;

  void runGame();
  bool checkCollision(int x, int y);
private:
};

#endif // GAMESTATE_H
