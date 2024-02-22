#ifndef PLAYER_H
#define PLAYER_H

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include <cmath>
#include <iostream>

#include "GameState.h"

using namespace std;

class Player : public sf::Drawable {
public:
  Player(int cx, int cy);
  ~Player();

  void jump();

  // Update call from gameloop
  void update(GameState &state);
  sf::RectangleShape getShape();

private:
  // Position
  int x, y;

  // override draw
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

  // Underlying SFML object
  sf::RectangleShape shape;

  // Velocities
  float vx, vy;

  void setPosition(int cx, int cy);
  bool isGrounded(GameState &state);
  void MovePlayer(float xoffset, float yoffset, GameState &state);
  int roundAwayFromZero(float x);
};

#endif // PLAYER_H
