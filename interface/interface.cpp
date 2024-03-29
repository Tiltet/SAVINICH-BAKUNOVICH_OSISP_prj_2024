//
// Created by Тимофей Савинич on 19.03.24.
//

#include <SFML/Graphics.hpp>
#include "interface.h"
#include "menuWindow/menu.h"
#include "preparationWindow/preparation.h"
#include "global/global.h"
#include "../logicpart/place/place.h"

const int gridSize = 10;
const int cellSize = 45;


// Функция отрисовки кораблей
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

// Функция, котрая отрисовывает игровые
void drawMap(sf::RenderWindow& window, const std::vector<std::vector<Cell>>& map)
{
    sf::Color colorBorder(0, 73, 194);
    sf::Color colorCell(202, 237, 247);

    for (int row = 0; row < gridSize; ++row)
    {
        for (int col = 0; col < gridSize; ++col)
        {
            const Cell& cell = map[row][col];
            window.draw(cell.shape);
        }
    }
}

// Функции ВЫСТРЕЛА
ShootCoordinates shoot(sf::RenderWindow& window, std::vector<std::vector<Cell>>& map)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    ShootCoordinates coordinates;

    // Считаем номер колонки и строки в поле Enemy
    int colEnemy = (worldPos.x - 740) / cellSize;
    int rowEnemy = (worldPos.y - 120) / cellSize;

    if (colEnemy >= 0 && colEnemy < gridSize && rowEnemy >= 0 && rowEnemy < gridSize)
    {
        Cell& cell = map[rowEnemy][colEnemy];

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
    if (coordinates.x)
    {
        std::cout << *coordinates.x;
    }
    else
    {
        std::cout << "N/A";
    }

    if (coordinates.y)
    {
        std::cout << *coordinates.y;
    }
    else
    {
        std::cout << "N/A";
    }

    return coordinates;
}

// ОСНОВНАЯ ФУНКЦИЯ ИНТЕРФЕЙСА
void interface()
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktopMode, "Resizable Window Example", sf::Style::Default);


    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(154, 215, 254));

    std::vector<std::vector<Cell>> mapUser(gridSize, std::vector<Cell>(gridSize));      // поле User
    std::vector<std::vector<Cell>> mapEnemy(gridSize, std::vector<Cell>(gridSize));     // поле Enemy

    // Проходим по полю User
    for (int row = 0; row < gridSize; ++row)
    {
        for (int col = 0; col < gridSize; ++col)
        {
            Cell& cell = mapUser[row][col];     // текущая клетка
            cell.state = CellState::Empty;      // ставим статус Empty

            cell.shape.setSize(sf::Vector2f(cellSize, cellSize));                   // размер
            cell.shape.setOutlineThickness(3.f);                                 // границы
            cell.shape.setFillColor(sf::Color::White);                              // фон
            cell.shape.setOutlineColor(sf::Color::Black);                           // цвет границ
            cell.shape.setPosition(col * cellSize + 50, row * cellSize + 120);      // позиция
        }
    }

    for (int row = 0; row < gridSize; ++row)
    {
        for (int col = 0; col < gridSize; ++col)
        {
            Cell& cell = mapEnemy[row][col];
            cell.state = CellState::Empty;

            cell.shape.setSize(sf::Vector2f(cellSize, cellSize));
            cell.shape.setOutlineThickness(3.f);
            cell.shape.setFillColor(sf::Color::White);
            cell.shape.setOutlineColor(sf::Color::Black);
            cell.shape.setPosition(col * cellSize + 740, row * cellSize + 120);
        }
    }

    drawShips(mapUser);

    // Главный цикл приложения
    while (window.isOpen())
    {
        sf::Event event{};
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
                    shoot(window, mapEnemy);
                }
            }
        }

        window.clear();
        window.draw(background);
        drawMap(window, mapUser);   // функции отрисовки полей: передаем поле, в функции отрисовываем в соответствии со всеми параметрами
        drawMap(window, mapEnemy);
        window.display();
    }
}

void interfaceTest()
{
    setGlobalFont();
    setGlobalScreenSize();

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Sea Battle", sf::Style::Fullscreen);
    window.setMouseCursorVisible(false);

    sf::RectangleShape background(sf::Vector2f(globalScreenWigth, globalScreenHeight));
    sf::Texture texture_window_background1;
    if (!texture_window_background1.loadFromFile("../interface/img/background.jpg"))
    {
        return;
    }

    background.setTexture(&texture_window_background1);

    game::GameMenu menu(globalScreenWigth / 2, globalScreenWigth / 10);

    menu.setTitle("Sea Battle", 144, sf::Color::White);
    menu.addItem("Classic", 86, sf::Color::White);
    menu.addItem("Fun Mode", 86, sf::Color::White);
    menu.addItem("Exit", 86, sf::Color::White);
    menu.alignMenu(3);

    int check = 0;

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
                    menu.moveUp();
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    menu.moveDown();
                }
                else if (event.key.code == sf::Keyboard::Return)
                {
                    int selectedItemIndex = menu.getSelectedItemIndex();
                    switch (selectedItemIndex)
                    {
                        case 0:
                            std::cout << "Start Classic Mode" << std::endl;
                            check = 1;
                            break;
                        case 1:
                            std::cout << "Start Fun Mode" << std::endl;
                            window.close();
                            interface();
                            break;
                        case 2:
                            std::cout << "Exit" << std::endl;
                            window.close();
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        if (check == 0)
        {
            window.clear();
            window.draw(background);
            menu.draw(window);
            window.display();
        }
        else if (check == 1)
        {
            pre::Preparation preparation(window, background, game::GameMenu(globalScreenWigth / 5, globalScreenHeight / 5));
        }
        else
        {
            window.close();
            interface();
        }
    }
}