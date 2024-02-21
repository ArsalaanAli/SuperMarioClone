#include <SFML/Graphics.hpp>
#include <tuple>

#include "GameState.h"
#include "Player.h"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"


GameState::GameState(){
  clock = sf::Clock();
}

float GameState::getDeltaTime() {
  // not using clock.getElapsedTime() since we want since last recorded frame
  return deltaTime;
}

sf::Vector2<int> GameState::getInputAxis() {
  return input;
}

sf::Vector2<int> GameState::updateInputAxis() {
  int dir_x = 0, dir_y = 0;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    dir_x += 1;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    dir_x += -1;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    dir_y += 1;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    dir_y += -1;
  }

  return sf::Vector2<int>(dir_x, dir_y);
}

void GameState::runGame(){
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Super Mario Clone");

    // Load background image
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/map1.png")) {
        return;
    }

    sf::Image collisionMap;
    if (!collisionMap.loadFromFile("assets/colourmap1.png")) {
        // Error handling if loading fails
        return;
    }

    Player player = Player(0, 0);

    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setPosition(0, 0);

    // Create the ground
    sf::RectangleShape ground(sf::Vector2f(1280.0f, 10.0f));
    ground.setFillColor(sf::Color::Black);
    ground.setPosition(0.0f, 550.0f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Calculate elapsed time
        deltaTime = clock.restart().asSeconds();
        input = updateInputAxis();

        // // Ensure the player does not move out of the window bounds
        // sf::FloatRect playerBounds = player.getGlobalBounds();
        //
        // if (collisionMap.getPixel(playerBounds.left, playerBounds.top) == sf::Color::Red){
        // }
        // if (playerBounds.left < 0)
        // {
        //     player.setPosition(0, player.getPosition().y);
        // }
        // else if (playerBounds.left + playerBounds.width > window.getSize().x)
        // {
        //     player.setPosition(window.getSize().x - playerBounds.width, player.getPosition().y);
        // }
        
        // updates
        player.update(*this);

        // Draw everything
        window.clear(sf::Color::White); // Clear the window with white color
        window.draw(ground);
        window.draw(backgroundSprite); // Draw background first
        window.draw(player);
        window.display();
    }
}

