//
// Created by Тимофей Савинич on 29.03.24.
//

#pragma once
#ifndef COURSE_WORK_GAME_H
#define COURSE_WORK_GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "iostream"
#include "../preparationWindow/preparation.h"
#include "../../logicpart/logic.h"
#include "../global/global.h"
#include "../../connection_funcs/con_funcs.h"
#include "../../user_client/user_client.h"

namespace game
{
    struct ShootCoordinates
    {
        int x;
        int y;
    };

    class Game
    {
    private:
        int number{};
        std::vector<std::vector<Cell>> mapUser;
        std::vector<std::vector<Cell>> mapEnemy;
        int clientSocket;

    public:
        explicit Game(sf::RenderWindow &window, sf::RectangleShape background, std::vector<std::vector<Cell>> map);
        void initMapEnemy();
        void drawMaps(sf::RenderWindow& window);
        ShootCoordinates shoot(sf::RenderWindow &window) const;
        bool checkKilled(int x, int y);
        void drawKilledShip(int x, int y, std::vector<std::vector<Cell>>& map);
    };
}

#endif //COURSE_WORK_GAME_H