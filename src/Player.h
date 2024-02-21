#include "GameState.h"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

class Player : public sf::Drawable {
public:
  Player(int cx, int cy);
  ~Player();

  // Update call from gameloop
  void update(GameState &state);

private:
  // Position
  int x, y;

  // override draw
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

  // Underlying SFML object
  sf::RectangleShape shape;

  // Velocities
  float vx, vy;
};
