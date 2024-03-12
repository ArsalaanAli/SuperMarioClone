#include "GameState.h"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

/**
 * @brief The Enemy class is responsible for handling enemy movement and interactions.
 */
class Enemy : public sf::Drawable
{
public:
  /**
   * @brief Construct a new Enemy object
   *
   * @param cx The spawnpoint x
   * @param cy The spawnpoint y
   */
  Enemy(int cx, int cy);

  /**
   * @brief Destroy the Enemy object
   */
  ~Enemy();

  /**
   * @brief Update loop for the enemy called every frame.
   *
   * @param state The current game state.
   */
  void update(GameState &state);

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
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

  /**
   * @brief The underlying shape of the enemy.
   */
  sf::RectangleShape shape;

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
  bool isGrounded(GameState &state);

  /**
   * @brief Move the enemy.
   *
   * @param xoffset The x offset.
   * @param yoffset The y offset.
   * @param state The current game state.
   */
  void MoveEnemy(float xoffset, float yoffset, GameState &state);

  /**
   * @brief Round a float away from zero.
   *
   * @param x The float to round.
   * @return the rounded float.
   */
  int roundAwayFromZero(float x);
};
