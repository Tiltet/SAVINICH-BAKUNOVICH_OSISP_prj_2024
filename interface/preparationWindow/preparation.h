//
// Created by Тимофей Савинич on 28.03.24.
//

#pragma once
#include "SFML/Graphics.hpp"
#include "../menuWindow/menu.h"

#ifndef COURSE_WORK_PREPARATION_H
#define COURSE_WORK_PREPARATION_H

namespace pre
{
    class Preparation
    {
    private:
        game::GameMenu gameMenu;

    public:
        explicit Preparation(sf::RenderWindow &window, sf::RectangleShape background,
                             game::GameMenu gameMenu);
    };
}

#endif //COURSE_WORK_PREPARATION_H