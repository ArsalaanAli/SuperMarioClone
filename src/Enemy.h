#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Game.h"

#define MAX_SPEED 400.0f
#define DECEL_RATE 3000.0f
#define ACCEL_RATE 3000.0f
#define JUMP_FORCE 1150.0f
#define AIR_DECEL_RATE (DECEL_RATE * 1.0f)
#define MAX_AIR_SPEED (MAX_SPEED * 5.0f)

/**
 * @brief The Enemy class is responsible for handling enemy movement and interactions.
 */
class Enemy : public sf::Drawable {
public:
  /**
   * @brief Construct a new Enemy object
   *
   * @param cx The spawnpoint x
   * @param cy The spawnpoint y
   */
  Enemy(int cx, int cy, const sf::Image& collisionMap);

  /**
   * @brief Destroy the Enemy object
   */
  ~Enemy();

  /**
   * @brief Update loop for the enemy called every frame.
   *
   * @param state The current game state.
   */
  void update(const FrameState& state);

  /**
   * @brief Check if the entity is colliding with the level.
   *
   * @param x The x coordinate.
   * @param y The y coordinate.
   *
   * @return Whether the player is colliding with the level.
   */
  bool checkCollision(int x, int y);

  /**
   * @brief Check if the player has collided with the enemy.
   *
   * @param px The player x position
   * @param py The player y position
   *
   * @return Whether the player has collided with the enemy.
   */
  bool checkPlayerCollision(float px, float py);

  /**
   * @brief Get the underlying sf::Shape object for the enemy.
   *
   * @return The underlying sf::Shape object.
   */
  sf::RectangleShape getShape();

  /**
   * @brief Kill the enemy.
   */
  void die();

  /**
   * @brief Reset the enemy to its spawnpoint.
   */
  void reset();

private:
  /**
   * @brief The enemy's spawnpoint.
   */
  int spawn_x, spawn_y;

  /**
   * @brief The enemy's velocity.
   */
  float vx, vy;

  /**
   * @brief API for drawing the enemy from a window.
   *
   * @example window.draw(enemy);
   * @param target The window to draw the enemy on.
   * @param states The render states to apply to the enemy.
   */
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  /**
   * @brief The underlying shape of the enemy.
   */
  sf::RectangleShape shape;

  /**
   * @brief The collision map of the level.
   */
  sf::Image cmap;

  /**
   * @brief Flag for whether the enemy is dying.
   */
  bool isDying;

  /**
   * @brief Check if the enemy is grounded.
   *
   * @param state The current game state.
   * @return Whether the enemy is grounded.
   */
  bool isGrounded();

  /**
   * @brief Move the enemy.
   *
   * @param xoffset The x offset.
   * @param yoffset The y offset.
   * @param state The current game state.
   */
  void MoveEnemy(float xoffset, float yoffset);

  /**
   * @brief Round a float away from zero.
   *
   * @param x The float to round.
   * @return the rounded float.
   */
  int roundAwayFromZero(float x);
};
