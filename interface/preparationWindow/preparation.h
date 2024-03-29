//
// Created by Тимофей Савинич on 28.03.24.
//

#pragma once
#include "SFML/Graphics.hpp"
#include "../menuWindow/menu.h"
#include "../../logicpart/place/place.h"
#include "iostream"

#ifndef COURSE_WORK_PREPARATION_H
#define COURSE_WORK_PREPARATION_H

namespace pre
{
    enum class CellState
    {
        Empty,
        Ship,
        Hit,
        Miss
    };


    struct Cell
    {
        CellState state;
        sf::RectangleShape shape;
    };

    class Preparation
    {
    private:
        game::GameMenu gameMenu;
        std::vector<std::vector<Cell>> map;

    public:
        explicit Preparation(sf::RenderWindow &window, sf::RectangleShape background, game::GameMenu gameMenu);
        void initMap();
        void clearMap();
        void drawMap(sf::RenderWindow& window);
        static void drawShips(std::vector<std::vector<pre::Cell>>& mapUser);
    };
}

#endif //COURSE_WORK_PREPARATION_H