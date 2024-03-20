//
// Created by Тимофей Савинич on 19.03.24.
//

#include "interface.h"

const int gridSize = 8;
const int cellSize = 50;

void interface()
{
    sf::RenderWindow window(sf::VideoMode(1240, 720), "Grid Example");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Cyan);
        drawGrid(window, 0);
        drawGrid(window, 1);
        window.display();
    }
}

void drawGrid(sf::RenderWindow& window, bool check)
{
    sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
    cell.setOutlineThickness(1.0f);
    cell.setOutlineColor(sf::Color::Black);

    sf::Color color1(175, 200, 225);   // Цвет ячейки 1
    sf::Color color2(200, 225, 175);   // Цвет ячейки 2

    for (int row = 0; row < gridSize; ++row)
    {
        for (int col = 0; col < gridSize; ++col)
        {
            int x = col * cellSize;
            int y = row * cellSize;

            if (check == true)
            {
                cell.setPosition(x, 360 - y);
            }
            else if (check == false)
            {
                cell.setPosition(1240 - x - cellSize, 360 - y);
            }

            // Чередуем цвета ячеек
            if ((row + col) % 2 == 0)
            {
                cell.setFillColor(color1);
            }
            else
            {
                cell.setFillColor(color2);
            }

            window.draw(cell);
        }
    }
}