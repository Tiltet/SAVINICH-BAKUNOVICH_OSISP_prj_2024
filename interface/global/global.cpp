//
// Created by Тимофей Савинич on 28.03.24.
//

#include "global.h"
#include "iostream"

sf::Font globalFont;
sf::Color Grey(191,191,191);
sf::Color Red(255, 0, 0);
sf::Color Blue(0, 132, 255);
unsigned int globalScreenWight;
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
    globalScreenWight = desktopMode.width;
    globalScreenHeight = desktopMode.height;

#if defined(__APPLE__)
    globalScreenWight = globalScreenWight / 2 + 5;
    globalScreenHeight = globalScreenHeight / 2;
#endif

    std::cout << globalScreenWight << " " <<  globalScreenHeight << std::endl;
}