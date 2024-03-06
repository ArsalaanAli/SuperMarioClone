#include "GameState.h"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

class Enemy : public sf::Drawable {
public:
  Enemy(int cx, int cy);
  Enemy(Enemy &&) = default;
  Enemy(const Enemy &) = default;
  Enemy &operator=(Enemy &&) = default;
  Enemy &operator=(const Enemy &) = default;
  ~Enemy();

  void update(GameState &state);
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

  bool isGrounded(GameState &state);
  void MoveEnemy(float xoffset, float yoffset, GameState &state);
  int roundAwayFromZero(float x);
};

