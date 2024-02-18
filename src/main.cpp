#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    // Create a rectangle shape
    sf::RectangleShape rectangle(sf::Vector2f(100, 50));
    rectangle.setPosition(100, 100);
    rectangle.setFillColor(sf::Color::Red);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(rectangle); // Draw the rectangle
        window.display();
    }

    return 0;

}


//main

//Level - map








