#include <SFML/Graphics.hpp>

#include "GameState.h"


GameState::GameState(){
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
    
    // Create a rectangle (player)
    sf::RectangleShape player(sf::Vector2f(50.0f, 50.0f));
    player.setFillColor(sf::Color::Black);
    player.setPosition(375.0f, 500.0f);

    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setPosition(0, 0);

    // Create the ground
    sf::RectangleShape ground(sf::Vector2f(1280.0f, 10.0f));
    ground.setFillColor(sf::Color::Black);
    ground.setPosition(0.0f, 550.0f);

    // Set the movement speed of the player
    float movementSpeed = 200.0f;

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Calculate elapsed time
        float deltaTime = clock.restart().asSeconds();

        // Handle movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            player.move(-movementSpeed * deltaTime, 0.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            player.move(movementSpeed * deltaTime, 0.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            player.move(0.0f, -movementSpeed * deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            player.move(0.0f, movementSpeed * deltaTime);
        }

        // Ensure the player does not move out of the window bounds
        sf::FloatRect playerBounds = player.getGlobalBounds();

        if (collisionMap.getPixel(playerBounds.left, playerBounds.top) == sf::Color::Red){
        }
        if (playerBounds.left < 0)
        {
            player.setPosition(0, player.getPosition().y);
        }
        else if (playerBounds.left + playerBounds.width > window.getSize().x)
        {
            player.setPosition(window.getSize().x - playerBounds.width, player.getPosition().y);
        }

        // Draw everything
        window.clear(sf::Color::White); // Clear the window with white color
        window.draw(ground);
        window.draw(backgroundSprite); // Draw background first
        window.draw(player);
        window.display();
    }
}

