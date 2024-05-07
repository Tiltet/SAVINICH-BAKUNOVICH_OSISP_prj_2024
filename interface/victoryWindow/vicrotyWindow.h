//
// Created by Тимофей Савинич on 7.05.24.
//

#include "../menuBlock/menu.h"

#ifndef COURSE_WORK_VICROTYWINDOW_H
#define COURSE_WORK_VICROTYWINDOW_H

namespace victory
{
    class Victory
    {
    private:
        bool check{};
    public:
        explicit Victory(sf::RenderWindow& window, bool check);
    };
}

#endif //COURSE_WORK_VICROTYWINDOW_H
