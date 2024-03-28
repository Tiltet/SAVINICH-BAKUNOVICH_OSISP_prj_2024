//
// Created by Тимофей Савинич on 28.03.24.
//

#include "menu.h"

game::GameMenu::GameMenu(float x, float y)
{
    this->menu_X = x;
    this->menu_Y = y;
    this->selectedItemIndex = 0;
    this->padding = 0;
}

void game::GameMenu::loadFont(const std::string &fontPath)
{
    if (!font.loadFromFile(fontPath))
    {
        return;
    }
}

void game::GameMenu::setTitle(const std::string &title, int characterSize, const sf::Color &color)
{
    titleText.setFont(font);
    titleText.setString(title);
    titleText.setCharacterSize(characterSize);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(menu_X, menu_Y);

    titleText.setOutlineThickness(5.0f);
    titleText.setOutlineColor(sf::Color::Red); // Установка цвета обводки на красный

    this->padding = padding + float(characterSize);
}

void game::GameMenu::draw(sf::RenderWindow &window)
{
    window.draw(titleText);
    for (const auto& menuItem : menuItems)
    {
        window.draw(menuItem);
    }
}

void game::GameMenu::addItem(const std::string &itemText, int characterSize, const sf::Color &color)
{
    sf::Text menuItem;
    sf::Text& lastItem = menuItems.back();

    menuItem.setFont(this->font);
    menuItem.setString(itemText);
    menuItem.setCharacterSize(characterSize);
    if (menuItems.empty())
    {
        menuItem.setFillColor(sf::Color::Red);
    }
    else
    {
        menuItem.setFillColor(color);
    }
    menuItem.setPosition(menu_X, menu_Y + this->padding + 10);
    this->padding = this->padding + float(characterSize) + 10;
    this->menuItems.push_back(menuItem);
}

int game::GameMenu::getSelectedItemIndex() const
{
    return selectedItemIndex;
}

void game::GameMenu::moveDown()
{
    if (selectedItemIndex < menuItems.size() - 1)
    {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex++;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

void game::GameMenu::moveUp()
{
    if (selectedItemIndex > 0)
    {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex--;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

void game::GameMenu::alignMenu(int posx)
{
    float nullx = 0;

    switch (posx)
    {
        case 1:
            nullx = 0;
            break;
        case 2:
            nullx = this->titleText.getLocalBounds().width;
            break;
        case 3:
            nullx = this->titleText.getLocalBounds().width / 2;
            break;
        default:
            nullx = 0;
            break;
    }

    this->titleText.setPosition(this->titleText.getPosition().x - nullx, this->titleText.getPosition().y);

    for (int i = 0; i < this->menuItems.size(); ++i)
    {
        switch (posx)
        {
            case 1:
                nullx = 0;
                break;
            case 2:
                nullx = this->menuItems[i].getLocalBounds().width;
                break;
            case 3:
                nullx = this->menuItems[i].getLocalBounds().width / 2;
            default:
                break;
        }

        menuItems[i].setPosition(menuItems[i].getPosition().x - nullx, menuItems[i].getPosition().y);
    }
}