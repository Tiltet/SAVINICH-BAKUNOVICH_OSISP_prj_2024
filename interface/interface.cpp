//
// Created by Тимофей Савинич on 19.03.24.
//

#include <SFML/Graphics.hpp>
#include "interface.h"

void interface()
{
    // SFML test code
    sf::RenderWindow window(sf::VideoMode(1240, 720), "Sea Battle");

    // Создание шрифта для заголовка
    sf::Font font;
    if (!font.loadFromFile("../interface/fonts/Arial Unicode.ttf"))
    {
        return;
    }

    // Создание текста заголовка
    sf::Text titleText("Sea Battle", font, 24);
    titleText.setFillColor(sf::Color::White);

    // Получение размеров текста
    sf::FloatRect textBounds = titleText.getGlobalBounds();

    // Центрирование текста
    float x = (window.getSize().x - textBounds.width) / 2.0f;
    float y = (window.getSize().y - textBounds.height) / 2.0f;
    titleText.setPosition(x,y);

    // ОСНОВНОЙ ЦИКЛ ПРИЛОЖЕНИЯ
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(titleText);
        window.display();
    }
}