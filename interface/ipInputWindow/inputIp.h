//
// Created by Тимофей Савинич on 23.04.24.
//

#ifndef COURSE_WORK_INPUTIP_H
#define COURSE_WORK_INPUTIP_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../global/global.h"
#include <iostream>

namespace ip {

    class Ip
    {
    private:

    public:
        explicit Ip(sf::RenderWindow &window, const sf::RectangleShape& background);
    };
}

#endif //COURSE_WORK_INPUTIP_H
