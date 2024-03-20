//
// Created by Тимофей Савинич on 19.03.24.
//

#include <SFML/Graphics.hpp>
#include "interface.h"

void interface()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Test");

    sf::RectangleShape rectangle(sf::Vector2f(200, 100));
    rectangle.setFillColor(sf::Color::Red);
    rectangle.setPosition(300, 250);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(rectangle);
        window.display();
    }
}