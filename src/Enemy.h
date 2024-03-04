#ifndef ENEMY_H
#define ENEMY_H
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

#include "FrameState.h"

class Enemy : public sf::Drawable {
public:
  Enemy(int cx, int cy);
  Enemy(Enemy &&) = default;
  Enemy(const Enemy &) = default;
  Enemy &operator=(Enemy &&) = default;
  Enemy &operator=(const Enemy &) = default;
  ~Enemy();

  void update(FrameState &state);
  bool checkPlayerCollision(float px, float py);
  sf::RectangleShape getShape();
  void die();
  void reset();
private:
  int spawn_x, spawn_y;

  // Velocities
  float vx, vy;

  // override draw
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

  // Underlying SFML object
  sf::RectangleShape shape;

  bool isDying;

  bool isGrounded(FrameState &state);
  void MoveEnemy(float xoffset, float yoffset, FrameState &state);
  int roundAwayFromZero(float x);
};
#endif // ENEMY_H
