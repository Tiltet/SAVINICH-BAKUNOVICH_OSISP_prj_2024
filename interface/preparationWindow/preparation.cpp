//
// Created by Тимофей Савинич on 28.03.24.
//

#include "preparation.h"

pre::Preparation::Preparation(sf::RenderWindow& window, sf::RectangleShape background)
{
    sf::Texture texture_window_background;
    if (!texture_window_background.loadFromFile("../interface/img/background2.jpg"))
    {
        return;
    }

    background.setTexture(&texture_window_background);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();
        window.draw(background);
        window.display();
    }
}
