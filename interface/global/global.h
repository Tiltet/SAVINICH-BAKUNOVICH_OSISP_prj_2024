//
// Created by Тимофей Савинич on 28.03.24.
//

#ifndef COURSE_WORK_GLOBAL_H
#define COURSE_WORK_GLOBAL_H

#pragma once
#include "SFML/Graphics.hpp"

enum class CellState
{
    Empty,
    Ship,
    Hit,
    Miss,
    Killed
};


struct Cell
{
    CellState state = CellState::Empty;
    sf::RectangleShape shape;
};

extern sf::Font globalFont;
extern sf::Color Grey;
extern sf::Color Red;
extern sf::Color Blue;
extern unsigned int globalScreenWight;
extern unsigned int globalScreenHeight;

extern

void setGlobalFont();
void setGlobalScreenSize();

#endif //COURSE_WORK_GLOBAL_H
