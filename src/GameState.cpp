#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

#include "GameState.h"
#include "Player.h"

GameState::GameState() {
    clock = sf::Clock();
    resetLevel = false;
    gameState = MainMenu;

    if (!font.loadFromFile("assets/SuperMario256.ttf")) {
        std::cerr << "Failed to load font file!" << std::endl;
    }
    if (!secondaryFont.loadFromFile("assets/pixel-nes.otf")) {
        std::cerr << "Failed to load font file!" << std::endl;
    }
}

float GameState::getDeltaTime() {
    // not using clock.getElapsedTime() since we want since last recorded frame
    return deltaTime;
}

sf::Vector2<int> GameState::getInputAxis() { return input; }

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

sf::View updateLevelScroll(sf::View &view, const float &LEVEL_END,
                           Player &player) {
    float x = player.getShape().getPosition().x + 25;
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

bool GameState::checkCollision(int x, int y) {
    sf::Vector2u size = collisionMap.getSize();
    if (x > 0 && x < size.x && y > 0 && y < size.y)
        return collisionMap.getPixel(x, y) == sf::Color::Red;
    return false;
}

void GameState::endLevel(bool win) {
    if (win) {
        // load next level
    } else {
        resetLevel = true;
    }
}

void GameState::drawMainMenu(sf::RenderWindow &window) {
    sf::Vector2f windowCenter = window.getView().getCenter();

    // Draw the overlay
    sf::RectangleShape overlay(sf::Vector2f(windowCenter.x + WINDOW_WIDTH / 2, windowCenter.y + WINDOW_HEIGHT / 2));
    overlay.setFillColor(sf::Color(0, 0, 0, 100)); // Semi-transparent black overlay
    window.draw(overlay);

    // Draw "TIME" label
    sf::Text timeLabel("TIME", secondaryFont, 50);
    timeLabel.setPosition(windowCenter.x - 400, windowCenter.y - WINDOW_HEIGHT / 2 + 10);
    timeLabel.setFillColor(sf::Color(255, 255, 0));
    window.draw(timeLabel);

    std::string timeString = std::to_string(getDeltaTime());
    sf::Text time(timeString, secondaryFont, 50);
    time.setPosition(windowCenter.x - 240, windowCenter.y - WINDOW_HEIGHT / 2 + 10);
    time.setFillColor(sf::Color(255, 255, 0));
    window.draw(time);

    float bannerWidth = window.getView().getSize().x * 0.50f;
    float bannerHeight = window.getView().getSize().y * 0.40f;
    float bannerX = windowCenter.x - bannerWidth / 2;
    float bannerY = windowCenter.y - window.getView().getSize().y / 2 + window.getView().getSize().y * 0.18f;

    // Draw the banner
    sf::RectangleShape banner(sf::Vector2f(bannerWidth, bannerHeight));
    banner.setPosition(bannerX, bannerY);
    banner.setFillColor(sf::Color(222, 66, 28));
    banner.setOutlineThickness(2);
    banner.setOutlineColor(sf::Color::White);
    window.draw(banner);

    sf::Text superText("SUPER", font, 80);
    superText.setPosition(bannerX + 10, bannerY + 10);
    superText.setFillColor(sf::Color(255, 255, 0));
    window.draw(superText);

    sf::Text marioCloneText("MARIO CLONE", font, 80);
    marioCloneText.setPosition(bannerX + 10, bannerY + bannerHeight - 170);
    marioCloneText.setFillColor(sf::Color(0, 255, 255));
    window.draw(marioCloneText);

    sf::Text copyRightText("c Nintendo 1985", secondaryFont, 30);
    copyRightText.setPosition(bannerX + bannerWidth - 345, bannerY + bannerHeight);
    copyRightText.setFillColor(sf::Color::White);
    window.draw(copyRightText);

    sf::RectangleShape startButton(sf::Vector2f(300, 60));
    startButton.setPosition(windowCenter.x - 150, windowCenter.y + WINDOW_HEIGHT / 2 - 200);
    startButton.setFillColor(sf::Color(0, 150, 136));
    startButton.setOutlineThickness(2);
    startButton.setOutlineColor(sf::Color(0, 188, 212));

    // Check mouse interaction with "Start Game" button
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    if (startButton.getGlobalBounds().contains(window.mapPixelToCoords(mousePosition))) {
        startButton.setFillColor(sf::Color(76, 175, 80));
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            gameState = Running;
        }
    } else {
        startButton.setFillColor(sf::Color(0, 150, 136));
    }

    window.draw(startButton);

    // Draw "Start Game" text
    sf::Text startGameText("Start Game", secondaryFont, 35);
    sf::FloatRect textBounds = startGameText.getLocalBounds();
    startGameText.setPosition(windowCenter.x - textBounds.width / 2, windowCenter.y + WINDOW_HEIGHT / 2 - 190);
    startGameText.setFillColor(sf::Color::White);
    window.draw(startGameText);

    // Draw "Quit Game" button
    sf::RectangleShape quitButton(sf::Vector2f(300, 60));
    quitButton.setPosition(windowCenter.x - 150, windowCenter.y + WINDOW_HEIGHT / 2 - 100);
    quitButton.setFillColor(sf::Color(244, 67, 54)); 
    quitButton.setOutlineThickness(2);
    quitButton.setOutlineColor(sf::Color::White);

    // Check mouse interaction with "Quit Game" button
    sf::Vector2i mousePositionQuit = sf::Mouse::getPosition(window);
    if (quitButton.getGlobalBounds().contains(window.mapPixelToCoords(mousePositionQuit))) {
        quitButton.setFillColor(sf::Color(233, 30, 99)); 
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            window.close();
        }
    } else {
        quitButton.setFillColor(sf::Color(244, 67, 54)); 
    }

    window.draw(quitButton);

    // Draw "Quit Game" text
    sf::Text quitGameText("Quit Game", secondaryFont, 35);
    sf::FloatRect quitTextBounds = quitGameText.getLocalBounds();
    quitGameText.setPosition(windowCenter.x - quitTextBounds.width / 2, windowCenter.y + WINDOW_HEIGHT / 2 - 100);
    quitGameText.setFillColor(sf::Color::White);
    window.draw(quitGameText);

    window.display();
}


void GameState::drawPausePopup(sf::RenderWindow &window) {
    // Get the center of the window
    sf::Vector2f windowCenter = window.getView().getCenter();

    // Draw the overlay
    sf::RectangleShape overlay(sf::Vector2f(windowCenter.x + WINDOW_WIDTH / 2, windowCenter.y + WINDOW_HEIGHT / 2));
    overlay.setFillColor(sf::Color(0, 0, 0, 128)); // Semi-transparent black overlay
    window.draw(overlay);

    // Draw the "Paused" text
    sf::Text pauseText("Paused", font, 50);
    pauseText.setPosition(windowCenter.x - pauseText.getLocalBounds().width / 2, windowCenter.y - 200);
    window.draw(pauseText);

    // Define the dimensions and position of the menu box
    float boxWidth = 300;
    float boxHeight = 180;
    sf::RectangleShape menuBox(sf::Vector2f(boxWidth, boxHeight));
    menuBox.setFillColor(sf::Color::Black);
    menuBox.setPosition(windowCenter.x - boxWidth / 2, windowCenter.y - boxHeight / 2);
    window.draw(menuBox);

    // Define the menu items
    sf::Text resumeText("Resume", secondaryFont, 30);
    sf::Text restartText("Restart", secondaryFont, 30);
    sf::Text quitText("Quit", secondaryFont, 30);

    // Calculate the vertical offset between menu items
    float textOffsetY = 20;

    // Position menu items vertically centered and spaced evenly
    resumeText.setPosition(windowCenter.x - resumeText.getLocalBounds().width / 2, windowCenter.y - 70);
    restartText.setPosition(windowCenter.x - restartText.getLocalBounds().width / 2, windowCenter.y + textOffsetY - 50);
    quitText.setPosition(windowCenter.x - quitText.getLocalBounds().width / 2, windowCenter.y + 2 * textOffsetY + restartText.getLocalBounds().height - 50);

    // Draw menu items
    window.draw(resumeText);
    window.draw(restartText);
    window.draw(quitText);

    // Draw the selection indicator
    sf::ConvexShape selector;
    selector.setPointCount(3);
    selector.setPoint(0, sf::Vector2f(0, 0));
    selector.setPoint(1, sf::Vector2f(20, 10));
    selector.setPoint(2, sf::Vector2f(0, 20));
    selector.setFillColor(sf::Color::White);

    switch (selectedMenuItem) {
    case 0:
        selector.setPosition(resumeText.getPosition().x - 30, resumeText.getPosition().y + resumeText.getGlobalBounds().height / 2);
        break;
    case 1:
        selector.setPosition(restartText.getPosition().x - 30, restartText.getPosition().y + restartText.getGlobalBounds().height / 2);
        break;
    case 2:
        selector.setPosition(quitText.getPosition().x - 30, quitText.getPosition().y + quitText.getGlobalBounds().height / 2);
        break;
    }

    window.draw(selector);
}


void GameState::handlePauseInput(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            // Move selection up (from first item to last item)
            selectedMenuItem = (selectedMenuItem == 0) ? 2 : selectedMenuItem - 1;
        } else if (event.key.code == sf::Keyboard::Down) {
            // Move selection down (from last item to first item)
            selectedMenuItem = (selectedMenuItem == 2) ? 0 : selectedMenuItem + 1;
        }
    }
}

void GameState::runGame() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Super Mario Clone");
    sf::View view(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    window.setView(view);

    // Load background image
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/map1.png")) {
        return;
    }

    LEVEL_END = backgroundTexture.getSize().x;

    if (!collisionMap.loadFromFile("assets/colourmap1.png")) {
        // Error handling if loading fails
        return;
    }

    Player player = Player(0, 0);

    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setPosition(0, 0);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Handle events based on game state
            if (gameState == MainMenu) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    gameState = Running;
                }
            } else if (gameState == Running) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
                    gameState = Paused;
                }
            } else if (gameState == Paused) {

                handlePauseInput(event);

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
                    gameState = Running;
                }

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    switch (selectedMenuItem) {
                    case 0:
                        gameState = Running;
                        selectedMenuItem = 0;
                        break;
                    case 1:
                        resetLevel = true;
                        gameState = Running;
                        selectedMenuItem = 0;
                        break;
                    case 2:
                        resetLevel = true;
                        gameState = MainMenu;
                        // window.close();
                        break;
                    default:
                        break;
                    }
                }
            }
        }

        if (gameState == MainMenu) {
            window.draw(backgroundSprite);
            drawMainMenu(window);
        } else if (gameState == Running) {
            deltaTime = clock.restart().asSeconds();
            input = updateInputAxis();

            if (resetLevel) {
                player = Player(0, 0);
                view = sf::View(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
                resetLevel = false;
            }

            view = updateLevelScroll(view, LEVEL_END, player);
            window.setView(view);

            // Entity updates
            player.update(*this);

            // Draw everything
            window.clear(sf::Color::White);
            window.draw(backgroundSprite);
            window.draw(player);
        } else if (gameState == Paused) {
            window.draw(backgroundSprite);
            drawPausePopup(window);
        }

        window.display();
    }
}
