#include "Entity.h"

int roundAwayFromZero(float x) { return x < 0 ? floor(x) : ceil(x); }

Entity::Entity() {
  int cx = 0;
  int cy = 0;

  spawn_x = cx;
  spawn_y = cy;

  // vx = MAX_SPEED;
  // vy = MAX_AIR_SPEED;

  shape = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
  shape.setFillColor(sf::Color::Red);
  shape.setPosition(cx, cy);

  isDying = false;
}

Entity::Entity(int cx, int cy) {
  spawn_x = cx;
  spawn_y = cy;

  // vx = MAX_SPEED;
  // vy = MAX_AIR_SPEED;

  shape = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
  shape.setFillColor(sf::Color::Red);
  shape.setPosition(cx, cy);

  isDying = false;
}

Entity::~Entity() {}

sf::RectangleShape Entity::getShape() {
  return shape;
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(shape, states);
}

bool Entity::isGrounded(GameState& state) {
  sf::Vector2<float> pos = shape.getPosition();
  sf::Vector2<float> size = shape.getSize();

  for (int i = 0; i < size.x; i++) {
    if (state.checkCollision(pos.x + i, pos.y + size.y + 1)) {
      // snap to ground
      float newY = pos.y + size.y;
      while (state.checkCollision(pos.x + i, newY)) {
        newY -= 1;
      }

      shape.setPosition(pos.x, newY - CELL_SIZE);

      // vy = 0;
      return true;
    }
  }

  // vy = MAX_AIR_SPEED;
  return false;
}

void Entity::update(GameState& state) {}

void Entity::MoveEntity(float xoffset, float yoffset, GameState& state) {
  shape.move(xoffset, yoffset);
}

void Entity::die() {
  if (!isDying) {
    // on first invocation, change color
    // TODO: shape.setFillColor(sf::Color::Yellow); // replace with change to dead texture
    isDying = true;
  }

  // disable horizontal movement
  vx = 0;
}

void Entity::reset() {
  shape = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
  shape.setFillColor(sf::Color::Red);
  shape.setPosition(spawn_x, spawn_y);

  // vx = MAX_SPEED;
  // vy = MAX_AIR_SPEED;

  isDying = false;
}

