#include <SFML/Graphics.hpp>

#include "GameState.h"

#define MAX_SPEED 400.0f
#define DECEL_RATE 3000.0f
#define ACCEL_RATE 3000.0f
#define JUMP_FORCE 1150.0f
#define AIR_DECEL_RATE (DECEL_RATE * 1.0f)
#define MAX_AIR_SPEED (MAX_SPEED * 5.0f)

class Entity : public sf::Drawable {
public:
  /**
   * @brief Construct a new Entity object
   *
   * @param cx The spawnpoint x
   * @param cy The spawnpoint y
   */
  Entity(int cx, int cy);

  /**
   * @brief Destroy the Entity object
   */
  ~Entity();

  /**
   * @brief Update loop for the entity called every frame.
   *
   * @param state The current game state.
   */
  void update(GameState& state);

  /**
   * @brief Get the underlying sf::Shape object for the entity.
   *
   * @return The underlying sf::Shape object.
   */
  sf::RectangleShape getShape();

  /**
   * @brief Kill the entity.
   */
  void die();

  /**
   * @brief Reset the entity to its spawnpoint.
   */
  void reset();

protected:
  /**
   * @brief The entity's spawnpoint.
   */
  int spawn_x, spawn_y;

  /**
   * @brief The entity's velocity.
   */
  float vx, vy;

  /**
   * @brief The underlying shape of the entity.
   */
  sf::RectangleShape shape;

  /**
   * @brief Flag for whether the entity is dying.
   */
  bool isDying;


  /**
   * @brief API for drawing the entity from a window.
   *
   * @example window.draw(entity);
   * @param target The window to draw the entity on.
   * @param states The render states to apply to the entity.
   */
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  /**
   * @brief Check if the entity is grounded.
   *
   * @param state The current game state.
   * @return Whether the entity is grounded.
   */
  bool isGrounded(GameState& state);

  /**
   * @brief Move the entity.
   *
   * @param xoffset The x offset.
   * @param yoffset The y offset.
   * @param state The current game state.
   */
  void MoveEntity(float xoffset, float yoffset, GameState& state);
};

