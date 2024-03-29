//
// Created by Тимофей Савинич on 28.03.24.
//

#include "global.h"
#include "iostream"

sf::Font globalFont;
unsigned int globalScreenWigth;
unsigned int globalScreenHeight;

void setGlobalFont()
{
    if (!globalFont.loadFromFile("../interface/fonts/Boomboom.otf"))
    {
        return;
    }
}

void setGlobalScreenSize()
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    globalScreenWigth = desktopMode.width;
    globalScreenHeight = desktopMode.height;

#if defined(__APPLE__)
    globalScreenWigth = globalScreenWigth / 2 + 5;
    globalScreenHeight = globalScreenHeight / 2;
#endif

    std::cout << globalScreenWigth << " " <<  globalScreenHeight << std::endl;
}