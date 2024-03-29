//
// Created by Тимофей Савинич on 28.03.24.
//

#include "preparation.h"

pre::Preparation::Preparation(sf::RenderWindow &window, sf::RectangleShape background, game::GameMenu gameMenu) : gameMenu(std::move(gameMenu))
{
    this->gameMenu.setTitle("Menu", 144, sf::Color::White);
    this->gameMenu.addItem("Auto", 86, sf::Color::White);
    this->gameMenu.addItem("Manual", 86, sf::Color::White);
    this->gameMenu.addItem("Start", 86, sf::Color::White);
    this->gameMenu.addItem("Exit", 86, sf::Color::White);
    this->gameMenu.alignMenu(3);

    sf::Texture texture_window_background;
    if (!texture_window_background.loadFromFile("../interface/img/background2.jpg"))
    {
        return;
    }

    background.setTexture(&texture_window_background);

    initMap();
    drawShips(this->map);

    int check;
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
                else if (event.key.code == sf::Keyboard::Return)
                {
                    int selectedItemIndex = this->gameMenu.getSelectedItemIndex();
                    switch (selectedItemIndex)
                    {
                        case 0:
                            std::cout << "Auto" << std::endl;
                            clearMap();
                            drawShips(this->map);
                            check = 1;
                            break;
                        case 1:
                            std::cout << "Start Fun Mode" << std::endl;
                            check = 2;
                            break;
                        case 3:
                            std::cout << "Exit" << std::endl;
                            window.close();
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        window.clear();
        window.draw(background);
        this->gameMenu.draw(window);
        drawMap(window);
        window.display();
    }
}

void pre::Preparation::initMap()
{
    std::vector<std::vector<Cell>> mapUser(10, std::vector<Cell>(10));
    this->map = mapUser;

    for (int row = 0; row < 10; ++row)
    {
        for (int col = 0; col < 10; ++col)
        {
            Cell& cell = this->map[row][col];
            cell.state = CellState::Empty;

            cell.shape.setSize(sf::Vector2f(60, 60));
            cell.shape.setOutlineThickness(3.f);
            cell.shape.setFillColor(sf::Color::White);
            cell.shape.setOutlineColor(sf::Color::Black);
            cell.shape.setPosition(col * 60 + globalScreenWigth / 2, row * 60 + globalScreenHeight / 7);
        }
    }
}

void pre::Preparation::clearMap()
{
    for (int row = 0; row < 10; ++row)
    {
        for (int col = 0; col < 10; ++col)
        {
            Cell& cell = this->map[row][col];
            cell.shape.setFillColor(sf::Color::White);
            cell.state = CellState::Empty;
        }
    }
}

void pre::Preparation::drawMap(sf::RenderWindow &window)
{
    sf::Color colorBorder(0, 73, 194);
    sf::Color colorCell(202, 237, 247);

    for (int row = 0; row < 10; ++row)
    {
        for (int col = 0; col < 10; ++col)
        {
            Cell& cell = this->map[row][col];
            window.draw(cell.shape);
        }
    }
}

void pre::Preparation::drawShips(std::vector<std::vector<pre::Cell>> &mapUser)
{
    int map[10][10] = {0};

    place(map);

    for (int row = 0; row < 10; ++row)
    {
        for (int col = 0; col < 10; ++col)
        {
            if (map[row][col] == 1)
            {
                mapUser[row][col].shape.setFillColor(sf::Color::Red);
            }
        }
    }
}