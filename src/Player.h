#ifndef PLAYER_H
#define PLAYER_H

#include "GameState.h"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

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
  bool isGrounded();
};

#endif // PLAYER_H
