//
// Created by Тимофей Савинич on 28.03.24.
//

#pragma once
#include "SFML/Graphics.hpp"

#ifndef COURSE_WORK_MENU_H
#define COURSE_WORK_MENU_H

namespace game
{
    class GameMenu {
    private:
        float menu_X;
        float menu_Y;
        int selectedItemIndex;
        float padding;
        sf::Font font;
        sf::Text titleText;
        std::vector<sf::Text> menuItems;

    public:
        GameMenu(float x, float y);
        void loadFont(const std::string& fontPath);
        void setTitle(const std::string& title, int characterSize, const sf::Color& color);
        void draw(sf::RenderWindow& window);
        void addItem(const std::string& itemText, int characterSize, const sf::Color& color);
        int getSelectedItemIndex() const;
        void moveDown();
        void moveUp();
        void alignMenu(int posx);
    };
}

#endif //COURSE_WORK_MENU_H