//
// Created by Тимофей Савинич on 19.03.24.
//

#ifndef COURSE_WORK_INTERFACE_H
#define COURSE_WORK_INTERFACE_H

#include <SFML/Graphics.hpp>
#include <iostream>

#include <optional>

// Класс с координатми выстрела
// опциональный так как User может не попасть по полю
// если пользователь не попал - придут std::nullopt в полях
struct ShootCoordinates
{
    std::optional<int> x;
    std::optional<int> y;
};

// #TODO
// Класс информации о поле
enum class CellState
{
    Empty,
    Ship,
    Hit,
    Miss
};


// Класс поля
struct Cell
{
    CellState state;
    sf::RectangleShape shape;
};

void interface();
ShootCoordinates shoot(sf::RenderWindow& window, std::vector<std::vector<Cell>>& map);
void drawMap(sf::RenderWindow& window, const std::vector<std::vector<Cell>>& map);
void interfaceTest();

#endif //COURSE_WORK_INTERFACE_H
