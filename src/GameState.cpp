#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "Enemy.h"
#include "GameState.h"
#include "Player.h"

GameState::GameState()
{
  clock = sf::Clock();
  resetLevel = false;
}

GameState::~GameState() {}

float GameState::getDeltaTime()
{
  // not using clock.getElapsedTime() since we want since last recorded frame
  return deltaTime;
}

sf::Vector2<int> GameState::getInputAxis() { return input; }

sf::Vector2<int> GameState::updateInputAxis()
{
  int dir_x = 0, dir_y = 0;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
  {
    dir_x += 1;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
  {
    dir_x += -1;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
  {
    dir_y += 1;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
  {
    dir_y += -1;
  }

  return sf::Vector2<int>(dir_x, dir_y);
}

/**
 * @brief Update the view to scroll with the player.
 *
 * @param view The current view.
 * @param LEVEL_END The end of the level.
 * @param player The player object.
 * @return The updated view.
 */
sf::View updateLevelScroll(sf::View &view, const float &LEVEL_END,
                           Player &player)
{
  float x = player.getShape().getPosition().x + (CELL_SIZE / 2);
  float viewX = view.getCenter().x;

  // calculate excess from either end as a positive value
  float excess = ((x > viewX) - (x < viewX)) * (x - viewX) -
                 VIEW_SCROLL_MARGIN_FROM_CENTER;

  // cover excess based on the end excess is on
  if (excess > 0 && x > VIEW_SCROLL_MARGIN &&
      x < LEVEL_END - VIEW_SCROLL_MARGIN)
  {
    view.move(((x > viewX) - (x < viewX)) * excess, 0);
  }

  return view;
}

bool GameState::checkCollision(int x, int y)
{
  sf::Vector2u size = collisionMap.getSize();
  if (x > 0 && x < size.x && y > 0 && y < size.y)
    return collisionMap.getPixel(x, y) == sf::Color::Red;
  return false;
}

void GameState::endLevel(bool win)
{
  if (win)
  {
    // load next level
  }
  else
  {
    resetLevel = true;
  }
}

void GameState::runGame()
{
  // Create the main window and view
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                          "Super Mario Clone");
  sf::View view(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
  window.setView(view);

  // Load level assets
  sf::Texture backgroundTexture;
  if (!backgroundTexture.loadFromFile("assets/map1.png"))
  {
    return;
  }
  sf::Sprite backgroundSprite(backgroundTexture);
  backgroundSprite.setPosition(0, 0);

  LEVEL_END = backgroundTexture.getSize().x;

  if (!collisionMap.loadFromFile("assets/colourmap1.png"))
  {
    // Error handling if loading fails
    return;
  }

  // Spawn player and enemies
  Player player = Player(0, 0);
  std::vector<Enemy> enemies;

  for (int i = 0; i < 1; i++)
  {
    Enemy enemy = Enemy((i + 1) * 1500, 0);
    enemies.push_back(enemy);
  }

  // Game loop
  while (window.isOpen())
  {

    // Handle events
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
    }

    // reset level if needed
    if (resetLevel)
    {
      player = Player(0, 0);
      view = sf::View(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
      for (auto &enemy : enemies)
      {
        enemy.reset();
      }

      resetLevel = false;
    }

    // Update game state
    deltaTime = clock.restart().asSeconds();
    input = updateInputAxis();

    // update level scroll
    view = updateLevelScroll(view, LEVEL_END, player);
    window.setView(view);

    // entity updates
    player.update(*this);
    for (auto &enemy : enemies)
    {
      enemy.update(*this);

      sf::Vector2f pos = player.getShape().getPosition();
      sf::Vector2f epos = enemy.getShape().getPosition();

      // Check for player collision with enemy
      if (enemy.checkPlayerCollision(pos.x, pos.y))
      {
        std::cout << pos.y - epos.y << "\n";
        // Check if player is above enemy
        if (pos.y - epos.y < -(CELL_SIZE / 2))
        {
          enemy.die();
          player.jump();
        }
        else
        {
          player.die();
        }
      }
    }

    // Draw everything
    window.clear(sf::Color::White); // Clear the window with white color
    window.draw(backgroundSprite);  // Draw background first
    for (Enemy enemy : enemies)
    {
      window.draw(enemy);
    }
    window.draw(player);
    window.display();
  }
}
