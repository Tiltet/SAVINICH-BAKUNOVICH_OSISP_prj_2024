//
// Created by Тимофей Савинич on 28.03.24.
//

#include "preparation.h"
#include "iostream"

pre::Preparation::Preparation(sf::RenderWindow &window, sf::RectangleShape background,
                              game::GameMenu gameMenu) : gameMenu(gameMenu) {

    this->gameMenu.setTitle("Menu", 144, sf::Color::White);
    this->gameMenu.addItem("Classic", 86, sf::Color::White);
    this->gameMenu.addItem("Fun Mode", 86, sf::Color::White);
    this->gameMenu.addItem("Exit", 86, sf::Color::White);
    this->gameMenu.alignMenu(3);

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
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    this->gameMenu.moveUp();
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    this->gameMenu.moveDown();
                }
            }
        }

        window.clear();
        window.draw(background);
        this->gameMenu.draw(window);
        window.display();
    }
}

/*
void drawShips(std::vector<std::vector<Cell>>& mapUser)
{
    int map[gridSize][gridSize] = {0};

    place(map);

    for (int row = 0; row < gridSize; ++row)
    {
        for (int col = 0; col < gridSize; ++col)
        {
            std::cout << map[row][col] <<  " ";
            if (map[row][col] == 1)
            {
                mapUser[row][col].shape.setFillColor(sf::Color::Red);
            }
        }
        std::cout << std::endl;
    }
}
*/