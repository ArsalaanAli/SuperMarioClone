#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <SFML/Graphics.hpp>

#include "EntityManager.h"
#include "FrameState.h"
#include "GameState.h"
#include "Player.h"

GameState::GameState() {}

sf::View updateLevelScroll(sf::View &view, const float &LEVEL_END, float px) {
  float x = px + CELL_SIZE / 2;
  float viewX = view.getCenter().x;

  // calculate excess from either end as a positive value
  float excess = ((x > viewX) - (x < viewX)) * (x - viewX) -
                 VIEW_SCROLL_MARGIN_FROM_CENTER;

  // cover excess based on the end excess is on
  if (excess > 0 && x > VIEW_SCROLL_MARGIN &&
      x < LEVEL_END - VIEW_SCROLL_MARGIN) {
    view.move(((x > viewX) - (x < viewX)) * excess, 0);
  }

  return view;
}

void GameState::runGame() {
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                          "Super Mario Clone");
  sf::View view(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
  window.setView(view);

  // Load background image
  sf::Texture backgroundTexture;
  if (!backgroundTexture.loadFromFile("assets/map1.png")) {
    return;
  }

  sf::Sprite backgroundSprite(backgroundTexture);
  backgroundSprite.setPosition(0, 0);

  const int LEVEL_END = backgroundTexture.getSize().x;

  sf::Image collisionMap;
  if (!collisionMap.loadFromFile("assets/colourmap1.png")) {
    // Error handling if loading fails
    return;
  }

  FrameState state = FrameState();
  em = EntityManager(collisionMap);

  em.spawnPlayer(0, 0);
  for (int i = 0; i < 1; i++) {
    em.spawnEnemy((i + 1) * 1500, 0);
  }

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    // update level scroll
    view = updateLevelScroll(view, LEVEL_END,
                             em.getPlayer()->getShape().getPosition().x);
    window.setView(view);

    state.update();
    em.update(state);

    // Draw everything
    window.clear(sf::Color::White); // Clear the window with white color
    window.draw(backgroundSprite);  // Draw background first
    for (auto &enemy : *em.getEnemies()) {
      window.draw(enemy);
    }
    window.draw(*em.getPlayer());

    window.display();

    // Post-frame processing
    if (state.getReset()) {
      // bypass win condition while not implemented
      if (state.getWin() && false) {
        // TODO: handle win and load next level
      } else {
        // reset level
        view = sf::View(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
        em.resetEntities();
      }
    }
  }
}
