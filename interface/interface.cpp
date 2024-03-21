//
// Created by Тимофей Савинич on 19.03.24.
//

#include "interface.h"

const int gridSize = 10;
const int cellSize = 45;

// 1240 - ширина
// 720 - высота

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
    sf::RenderWindow window(sf::VideoMode(1240, 720), "Sea Battle", sf::Style::Titlebar | sf::Style::Close);
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