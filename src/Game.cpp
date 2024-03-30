#include "Game.h"

Game* Game::instance = nullptr;

Game* Game::getInstance() {
  if (instance == nullptr) {
    instance = new Game();
  }
  return instance;
}

Game::Game() {
  if (!font.loadFromFile("assets/SuperMario256.ttf")) {
    std::cerr << "Failed to load font file!" << std::endl;
    exit(1);
  }

  if (!secondaryFont.loadFromFile("assets/pixel-nes.otf")) {
    std::cerr << "Failed to load font file!" << std::endl;
    exit(1);
  }

  if (!backgroundTexture.loadFromFile("assets/map1.png")) {
    std::cerr << "Failed to load background texture!" << std::endl;
    exit(1);
  }

  window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
    "Super Mario Clone");

  scene = Scene::MainMenu;

  // Setup the selection indicator
  selectedMenuItem = 0;
  selector.setPointCount(3);
  selector.setPoint(0, sf::Vector2f(0, 0));
  selector.setPoint(1, sf::Vector2f(20, 10));
  selector.setPoint(2, sf::Vector2f(0, 20));
  selector.setFillColor(sf::Color::Transparent);
}

Game::~Game() {
  delete window;
  delete instance;
}

float Game::getDeltaTime() {
  return deltaTime;
}

sf::Vector2<int> Game::getInputAxis() { return input; }

sf::Vector2<int> Game::calculateInputAxis() {
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

Level* Game::getLevel() {
  return &level;
}

sf::RenderWindow* Game::getWindow() {
  return window;
}

void Game::setScene(Scene target) {
  switch (scene) {
  case Scene::MainMenu:
    if (target == Scene::Running) {
      // Start the game
      std::cout << "Starting game..." << std::endl;
      level = Level("assets/map1.png", "assets/newcolourmap1.png");
    }
    break;
  default:
    break;
  }

  scene = target;
}

bool isMouseOverText(sf::RenderWindow& window, sf::Text& text) {
  sf::FloatRect bounds = text.getGlobalBounds();
  sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
  return bounds.contains(mousePos);
}

void Game::drawMainMenu(sf::RenderWindow& window) {
  sf::Vector2f windowCenter = window.getView().getCenter();

  // Draw the overlay
  sf::RectangleShape overlay(sf::Vector2f(windowCenter.x + WINDOW_WIDTH / 2, windowCenter.y + WINDOW_HEIGHT / 2));
  overlay.setFillColor(sf::Color(0, 0, 0, 100)); // Semi-transparent black overlay
  window.draw(overlay);

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
      setScene(Scene::Running);
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
  quitGameText.setPosition(windowCenter.x - quitTextBounds.width / 2, windowCenter.y + WINDOW_HEIGHT / 2 - 95);
  quitGameText.setFillColor(sf::Color::White);
  window.draw(quitGameText);
}

void Game::drawPauseMenu(sf::RenderWindow& window) {
  const std::string menuLabels[3] = { "Resume", "Restart", "Quit" };
  const int fontSize = 30;

  // Get the center of the window
  sf::Vector2f windowCenter = window.getView().getCenter();

  // Draw the overlay
  sf::RectangleShape overlay(sf::Vector2f(windowCenter.x + WINDOW_WIDTH / 2, windowCenter.y + WINDOW_HEIGHT / 2));
  overlay.setFillColor(sf::Color(0, 0, 0, 128)); // Semi-transparent black overlay
  window.draw(overlay);

  // Define the dimensions and position of the menu box
  float boxWidth = 300;
  float boxHeight = menuLabels->size() * fontSize + 20;
  sf::RectangleShape menuBox(sf::Vector2f(boxWidth, boxHeight));
  menuBox.setFillColor(sf::Color::Black);
  menuBox.setPosition(windowCenter.x - boxWidth / 2, windowCenter.y - boxHeight / 2);
  window.draw(menuBox);

  // Draw the "Paused" text
  sf::Text pauseText("Paused", font, fontSize + 20);
  pauseText.setPosition(windowCenter.x - pauseText.getLocalBounds().width / 2, windowCenter.y - 200);
  window.draw(pauseText);

  // Calculate the vertical offset between menu items
  float textOffsetY = 20;
  int actionItem = -1;

  for (int i = 0; i < 3; i++) {
    sf::Text text(menuLabels[i], secondaryFont, fontSize);
    int x = windowCenter.x - text.getLocalBounds().width / 2;
    int y = windowCenter.y - boxHeight / 2 + i * 60 + textOffsetY;
    text.setPosition(x, y);

    if (isMouseOverText(window, text)) {
      selector.setFillColor(sf::Color::White);
      selectedMenuItem = i;
      selector.setPosition(x - 30, y + text.getGlobalBounds().height / 2);

      if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        actionItem = i;
      }
    }

    window.draw(text);
  }

  window.draw(selector);

  switch (actionItem) {
  case 0:
    setScene(Scene::Running);
    break;
  case 1:
    // Restart the game
    level.reset();
    setScene(Scene::Running);
    break;
  case 2:
    setScene(Scene::MainMenu);
    break;
  }
}

void Game::run() {
  sf::Sprite backgroundSprite = sf::Sprite(backgroundTexture);

  while (window->isOpen()) {
    sf::Event event;
    while (window->pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window->close();
      }

      if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        case sf::Keyboard::Escape:
          if (scene == Scene::Running) {
            setScene(Scene::Paused);
          } else if (scene == Scene::Paused) {
            setScene(Scene::Running);
          }
          break;
        default:
          break;
        }
      }

      // Propagate events to the level
      if (scene == Scene::Running) {
        level.handleEvent(event);
      }
    }

    deltaTime = dt_clock.restart().asSeconds();
    input = calculateInputAxis();

    window->clear(sf::Color::Black);

    switch (scene) {
    case Scene::MainMenu:
      window->draw(backgroundSprite);
      drawMainMenu(*window);
      break;
    case Scene::Running:
      // drawHud(*window, 100, 0, 3, 0);
      level.update();
      level.draw(*window);
      break;
    case Scene::Paused:
      // drawPausePopup(*window);
      window->draw(backgroundSprite);
      drawPauseMenu(*window);
      break;
    }

    window->display();
  }
}
