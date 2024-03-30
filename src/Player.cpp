#include <iostream>
#include <SFML/Window/Keyboard.hpp>

#include "Player.h"
#include "Game.h"

#define CELL_SIZE 50.0f

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
  animationTime = 0;
  curFrame = 0;
  curAnim = 0;
}

void Player::loadSprites() {
  for (int i = 0; i < numSprites.size(); i++) {
    vector<sf::Texture> tempVec;
    textures.push_back(tempVec);
  }
  // load standing sprites
  for (int i = 0; i < numSprites[STAND]; i++) {
    sf::Texture temp;
    if (!temp.loadFromFile("assets/MarioSprites/stand" + to_string(i) + ".png")) {
      return;
    }
    textures[STAND].push_back(temp);
  }
  //load running sprites
  for (int i = 0; i < numSprites[RUN]; i++) {
    sf::Texture temp;
    if (!temp.loadFromFile("assets/MarioSprites/run" + to_string(i) + ".png")) {
      return;
    }
    textures[RUN].push_back(temp);
  }
  sprite.setTexture(textures[0][0]);
  sprite.setScale(SCALE, SCALE);
  sprite.setPosition(0, 0);
  sf::FloatRect bounds = sprite.getLocalBounds();
  sprite.setOrigin(bounds.width / 2.f, sprite.getOrigin().y);
}

Player::~Player() {}

sf::Sprite Player::getShape() { return sprite; }

// compatibility for windows.draw(player)
void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite); // Draw the sprite member variable
}


bool Player::isGrounded() {
  sf::Vector2<float> pos = sprite.getPosition();
  sf::Vector2<float> size;
  size.x = sprite.getGlobalBounds().width;
  size.y = sprite.getGlobalBounds().height;
  Level* level = Game::getInstance()->getLevel();

  for (int i = 0; i < size.x; i++) {
    if (level->checkCollision(pos.x + i, pos.y + size.y + 1)) {
      float newY = pos.y + size.y;
      while (level->checkCollision(pos.x + i, newY)) {
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
    curAnim = RUN;
    if (input.x < 0 && sprite.getScale().x > 0) {
      sprite.scale(-1, 1);
    }
    if (input.x > 0 && sprite.getScale().x < 0) {
      sprite.scale(-1, 1);
    }
    // accelerate
    vx += input.x * ACCEL_RATE * dt;
    vx = std::min(vx, MAX_SPEED);
    vx = std::max(vx, -MAX_SPEED);
  } else {
    curAnim = STAND;
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
void Player::update() {
  sf::Clock clock;
  float dt = Game::getInstance()->getDeltaTime();
  sf::Vector2<int> input = Game::getInstance()->getInputAxis();
  bool grounded = isGrounded();

  if (shouldDie() || isDying) {
    die();

    // once going down from jump, trigger reset level
    if (vy <= 0) {
      Game::getInstance()->getLevel()->endLevel(false);
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
  MovePlayer(vx * dt, -vy * dt);
  AnimatePlayer(Game::getInstance()->getDeltaTime());
}

void Player::AnimatePlayer(float deltaTime) {
  animationTime += deltaTime;
  if (animationTime > animationInterval) {
    curFrame = (curFrame + 1) % numSprites[curAnim];
    animationTime = 0;
    sprite.setTexture(textures[curAnim][curFrame]);
  }
}

void Player::MovePlayer(float xoffset, float yoffset) {
  sf::Vector2<float> pos = sprite.getPosition();
  sf::Vector2<float> size;
  size.x = sprite.getGlobalBounds().width;
  size.y = sprite.getGlobalBounds().height;
  Level* level = Game::getInstance()->getLevel();

  int newX = roundAwayFromZero(xoffset);

  // check for collision on left and right sides
  for (int i = 0; i <= size.y - 3; i++) {
    if (level->checkCollision(pos.x + newX, pos.y + i) ||
      level->checkCollision(pos.x + size.x + newX, pos.y + i)) {
      // if collision, ignore x movement
      xoffset = 0;
      break;
    }
  }

  int newY = roundAwayFromZero(yoffset);

  // check for collision on top and bottom sides
  for (int i = 0; i <= size.x; i++) {
    if (level->checkCollision(pos.x + i, pos.y + newY)) {
      // if collision, invert y movement
      yoffset = abs(yoffset);
      vy = -yoffset;
      break;
    }
  }

  sprite.setPosition(pos.x + xoffset, pos.y + yoffset);
}
