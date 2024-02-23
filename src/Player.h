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
  void die();

  // Update call from gameloop
  void update(GameState &state);
  sf::RectangleShape getShape();

private:
  // Position
  float x, y;

  // override draw
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

  // Underlying SFML object
  sf::RectangleShape shape;

  // Velocities
  float vx, vy;

  bool isDying;

  void setPosition(int cx, int cy);
  void processInput(sf::Vector2<int> input, bool grounded, float dt);
  void gravity();
  bool isGrounded(GameState &state);
  void MovePlayer(float xoffset, float yoffset, GameState &state);
  bool shouldDie();
};

#endif // PLAYER_H
