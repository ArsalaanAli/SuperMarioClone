#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "Player.h"

/**
 * @brief Round a float away from zero.
 *
 * @param x The float to round.
 * @return the rounded float.
 */
int roundAwayFromZero(float x) { return x < 0 ? floor(x) : ceil(x); }

Player::Player(int cx, int cy) {
  loadSprites();
  isDying = false;
  vx = 0;
  vy = 0;
}

void Player::loadSprites() {
  if (!spriteTexture.loadFromFile("assets/MarioSprites/run0.png")) {
    // Error handling if loading fails
    return;
  }
  sprite.setTexture(spriteTexture);
  sprite.setScale(2, 2);
  sprite.setPosition(0, 0);
}

Player::~Player() {}

sf::Sprite Player::getShape() { return sprite; }

// compatibility for windows.draw(player)
void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite); // Draw the sprite member variable
}


bool Player::isGrounded(GameState& state) {
  sf::Vector2<float> pos = sprite.getPosition();
  sf::Vector2<float> size;
  size.x = sprite.getGlobalBounds().width;
  size.y = sprite.getGlobalBounds().height;

  for (int i = 0; i < size.x; i++) {
    if (state.checkCollision(pos.x + i, pos.y + size.y + 1)) {
      float newY = pos.y + size.y;
      while (state.checkCollision(pos.x + i, newY)) {
        newY -= 1;
      }
      sprite.setPosition(pos.x, newY - CELL_SIZE);

      vy = 0;
      return true;
    }
  }

  return false;
}

void Player::processInput(sf::Vector2<int> input, bool grounded, float dt) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && grounded) {
    jump();
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
    die();
  }

  // handle horizontal movement
  if (input.x) {
    // accelerate
    vx += input.x * ACCEL_RATE * dt;
    vx = std::min(vx, MAX_SPEED);
    vx = std::max(vx, -MAX_SPEED);
  } else {
    // decelerate
    if (abs(vx) > DECEL_RATE * dt)
      vx -= DECEL_RATE * ((vx > 0) - (vx < 0)) * dt;
    else
      vx = 0;
  }
}

bool Player::shouldDie() {
  // TODO: include check for enemy collision and any other death conditions
  return sprite.getPosition().y >= 665;
}

void Player::jump() { vy = JUMP_FORCE; }

void Player::die() {
  if (!isDying) {
    // on first invocation, jump
    jump();
    isDying = true;
  }

  // disable horizontal movement
  vx = 0;
}

// Update called once per gameloop
void Player::update(GameState& state) {
  float dt = state.getDeltaTime();
  sf::Vector2<int> input = state.getInputAxis();
  bool grounded = isGrounded(state);

  if (shouldDie() || isDying) {
    die();

    // once going down from jump, trigger reset level
    if (vy <= 0) {
      state.endLevel(false);
    }
  } else {
    processInput(input, grounded, dt);
  }

  // handle vertical movement and gravity
  if (!grounded) {
    bool falling = vy <= 0 || !sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    if (dt < 0.1) {
      vy -= AIR_DECEL_RATE * (falling ? 2.0f : 1.0f) * dt;
      vy = std::min(vy, MAX_AIR_SPEED);
      vy = std::max(vy, -MAX_AIR_SPEED);
    }
  }

  // move player based on velocity
  MovePlayer(vx * dt, -vy * dt, state);


  // std::cout << vx << "," << vy << "\n";
  // std::cout << pos.x << "," << pos.y << "\n";
}

void Player::MovePlayer(float xoffset, float yoffset, GameState& state) {
  sf::Vector2<float> pos = sprite.getPosition();
  sf::Vector2<float> size;
  size.x = sprite.getGlobalBounds().width;
  size.y = sprite.getGlobalBounds().height;

  int newX = roundAwayFromZero(xoffset);

  // check for collision on left and right sides
  for (int i = 0; i <= size.y - 3; i++) {
    if (state.checkCollision(pos.x + newX, pos.y + i) ||
      state.checkCollision(pos.x + size.x + newX, pos.y + i)) {
      // if collision, ignore x movement
      xoffset = 0;
      break;
    }
  }

  int newY = roundAwayFromZero(yoffset);

  // check for collision on top and bottom sides
  for (int i = 0; i <= size.x; i++) {
    if (state.checkCollision(pos.x + i, pos.y + newY)) {
      // if collision, invert y movement
      yoffset = abs(yoffset);
      vy = -yoffset;
      break;
    }
  }

  // cout << pos.x + xoffset << " " <<pos.y <<" " << yoffset << endl;
  sprite.setPosition(pos.x + xoffset, pos.y + yoffset);
}
