//
// Created by Тимофей Савинич on 29.03.24.
//

#include "game.h"
#include <utility>

const int gridSize = 10;
const int cellSize = 60;

game::Game::Game(sf::RenderWindow &window, sf::RectangleShape background, std::vector<std::vector<Cell>> map)
{
    std::cout << "Game" << std::endl;
    this->mapUser = std::move(map);

    window.setMouseCursorVisible(true);

    sf::Texture texture_window_background1;
    if (!texture_window_background1.loadFromFile("../interface/img/gameBack.jpg"))
    {
        return;
    }

    background.setTexture(&texture_window_background1);
    initMapEnemy();

    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("../interface/sounds/bam2.mp3"))
    {
        return;
    }

    sf::Sound sound;
    sound.setBuffer(soundBuffer);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed)   // событие нажатия на кнопку мыши
            {
                if (event.mouseButton.button == sf::Mouse::Left)    // если нажата левая клавиша мыши
                {

                    shoot(window);
                    sound.play();
                }
            }
        }

        window.clear();
        window.draw(background);
        this->drawMaps(window);
        window.display();
    }
}

void game::Game::initMapEnemy()
{
    std::vector<std::vector<Cell>> mapUser(10, std::vector<Cell>(10));
    this->mapEnemy = mapUser;

    for (int row = 0; row < 10; ++row)
    {
        for (int col = 0; col < 10; ++col)
        {
            Cell& cell = this->mapEnemy[row][col];
            cell.state = CellState::Empty;

            cell.shape.setSize(sf::Vector2f(60, 60));
            cell.shape.setOutlineThickness(3.f);
            cell.shape.setFillColor(sf::Color::White);
            cell.shape.setOutlineColor(sf::Color::Black);
        }
    }
}

void game::Game::drawMaps(sf::RenderWindow& window)
{
    sf::Color colorBorder(0, 73, 194);
    sf::Color colorCell(202, 237, 247);

    for (int row = 0; row < 10; ++row)
    {
        for (int col = 0; col < 10; ++col)
        {
            Cell& cellUser = this->mapUser[row][col];
            cellUser.shape.setPosition(col * 60 + globalScreenWigth / 20, row * 60 + globalScreenHeight / 6);
            window.draw(cellUser.shape);

            Cell& cellEnemy = this->mapEnemy[row][col];
            cellEnemy.shape.setPosition(col * 60 + globalScreenWigth / 1.85, row * 60 + globalScreenHeight / 6);
            window.draw(cellEnemy.shape);
        }
    }
}

game::ShootCoordinates game::Game::shoot(sf::RenderWindow &window)
{

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

    // Считаем номер колонки и строки в поле Enemy
    int colEnemy = (worldPos.x - globalScreenWigth / 1.85) / cellSize;
    int rowEnemy = (worldPos.y - globalScreenHeight / 6) / cellSize;

    ShootCoordinates coordinates;

    if (colEnemy >= 0 && colEnemy < gridSize && rowEnemy >= 0 && rowEnemy < gridSize)
    {
        Cell& cell = this->mapEnemy[rowEnemy][colEnemy];

        if (cell.state == CellState::Empty)
        {
            cell.state = CellState::Miss;
            cell.shape.setFillColor(sf::Color::Blue);
            std::cout << "miss" << std::endl;
            std::cout << "Row = " << rowEnemy << std::endl;
            std::cout << "Col = " << colEnemy << std::endl;
        }
        else if (cell.state == CellState::Ship)
        {
            cell.state = CellState::Hit;
            cell.shape.setFillColor(sf::Color::Red);
            std::cout << "hit" << std::endl;
            std::cout << "Row = " << rowEnemy << std::endl;
            std::cout << "Col = " << colEnemy << std::endl;
        }
        coordinates.x = rowEnemy;
        coordinates.y = colEnemy;
    }

    // Раскрываем опционал и выводим координаты выстрела в консоль
    std::cout << "Координаты выстрела x: ";
    if (coordinates.x && *coordinates.y)
    {
        std::cout << *coordinates.x << " " << *coordinates.y;
    }
    else
    {
        std::cout << "Неправильные координат\n";
    }

    return coordinates;
}