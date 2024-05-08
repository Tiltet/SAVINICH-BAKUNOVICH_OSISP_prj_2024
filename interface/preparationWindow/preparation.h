//
// Created by Тимофей Савинич on 28.03.24.
//

#pragma once
#include "../menuBlock/menu.h"
#include "../../logicpart/place/place.h"
#include "../gameWindow/game.h"
#include "../global/global.h"
#include "../ipInputWindow/inputIp.h"
#include "iostream"

#ifndef COURSE_WORK_PREPARATION_H
#define COURSE_WORK_PREPARATION_H

namespace pre
{
    class Preparation
    {
    private:
        std::vector<std::vector<Cell>> map;

    public:
        explicit Preparation(sf::RenderWindow &window, sf::RectangleShape background);
        void initMap();
        void clearMap();
        void drawMap(sf::RenderWindow& window);
        static void drawShips(std::vector<std::vector<Cell>>& mapUser);
    };
}

#endif //COURSE_WORK_PREPARATION_H