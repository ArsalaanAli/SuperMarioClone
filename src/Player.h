#ifndef PLAYER_H
#define PLAYER_H

#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

using namespace std;

#define MAX_SPEED 400.0f
#define DECEL_RATE 3000.0f
#define ACCEL_RATE 3000.0f
#define JUMP_FORCE 1150.0f
#define AIR_DECEL_RATE (DECEL_RATE * 1.0f)
#define MAX_AIR_SPEED (MAX_SPEED * 5.0f)

class Player : public sf::Drawable {
public:
  /**
   * @brief Construct a new Player object
   *
   * @param cx spawnpoint x
   * @param cy spawnpoint y
   */
  Player(int cx, int cy);

  /**
   * @brief Destroy the Player object
   */
  ~Player();

  /**
   * @brief Jump the player.
   */
  void jump();

  /**
   * @brief Kill the player.
   */
  void die();

  /**
   * @brief Update loop for the player called every frame.
   *
   * @param state The current game state.
   */
  void update();

  /**
   * @brief Get the Shape object
   *
   * @return The player's shape.
   */
  sf::Sprite getShape();

private:
  static const int SCALE = 2;
  static const int STAND = 0;
  static const int RUN = 1;

  /**
   * @brief The player's position.
   */
  float x, y;

  /**
   * @brief API for drawing the player from a window.
   *
   * @example window.draw(player);
   * @param target The window to draw the player on.
   * @param states The render states to apply to the player.
   */
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  // Underlying SFML object
  // Velocities
  float vx, vy;

  /**
   * @brief Flag for whether the player is dying.
   */
  bool isDying;

  /**
   * @brief The player's sprite.
   */
  sf::Sprite sprite;

  /**
   * @brief The player's textures.
   */
  vector<vector<sf::Texture>> textures;

  /**
   * @brief The player's animations.
   */
  vector<int> numSprites = { 1, 3 };

  /**
   * @brief The current frame of the animation.
   */
  int curFrame;

  /**
   * @brief The current animation.
   */
  int curAnim;

  /**
   * @brief The player's animation interval.
   */
  float animationInterval = 0.15f;

  /**
   * @brief The player's animation time.
   */
  float animationTime;

  /**
   * @brief Animate the player.
   *
   * @param deltaTime The time since the last frame.
   */
  void AnimatePlayer(float deltaTime);

  /**
   * @brief Load the player's sprites.
   */
  void loadSprites();

  /**
   * @brief Set the player's position.
   *
   * @param cx The x position.
   * @param cy The y position.
   */
  void setPosition(int cx, int cy);

  /**
   * @brief Process the input for the player.
   *
   * @param input The input axis.
   * @param grounded Whether the player is grounded.
   * @param dt The time since the last frame.
   */
  void processInput(sf::Vector2<int> input, bool grounded, float dt);

  /**
   * @brief Check if the player is grounded.
   *
   * @param state The current game state.
   * @return Whether the player is grounded.
   */
  bool isGrounded();

  /**
   * @brief Move the player.
   *
   * @param xoffset The x offset.
   * @param yoffset The y offset.
   * @param state The current game state.
   */
  void MovePlayer(float xoffset, float yoffset);

  /**
   * @brief Check if the player should die.
   *
   * @return Whether the player should die.
   */
  bool shouldDie();
};

#endif // PLAYER_H
