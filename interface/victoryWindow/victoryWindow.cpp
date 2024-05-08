//
// Created by Тимофей Савинич on 7.05.24.
//

#include <SFML/Graphics/RenderWindow.hpp>
#include "./vicrotyWindow.h"

victory::Victory::Victory(sf::RenderWindow& window, bool check)
{
    this->check = check;

    menu::Menu menu(globalScreenWight / 2, globalScreenWight / 10);

    if (check)
    {
        menu.setTitle("Victory", 144, sf::Color::White);
    }
    else
    {
        menu.setTitle("Lose", 144, sf::Color::White);
    }

    menu.addItem("Restart", 86, sf::Color::White);
    menu.addItem("Exit", 86, sf::Color::White);
    menu.alignMenu(3);

    sf::RectangleShape background(sf::Vector2f(globalScreenWight, globalScreenHeight));
    sf::Texture texture_window_background;
    if (!texture_window_background.loadFromFile("../interface/img/background2.jpg"))
    {
        return;
    }

    background.setTexture(&texture_window_background);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    menu.moveUp();
                    // sound.play();
                } else if (event.key.code == sf::Keyboard::Down) {
                    menu.moveDown();
                    // sound.play();
                } else if (event.key.code == sf::Keyboard::Return) {
                    int selectedItemIndex = menu.getSelectedItemIndex();
                    switch (selectedItemIndex) {
                        case 0:
                            std::cout << "Restart" << std::endl;
                            this->menuCheck = true;
                            break;
                        case 1:
                            std::cout << "Exit" << std::endl;
                            window.close();
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        if (this->menuCheck)
        {
            pre::Preparation(window, background);
            window.close();
        }

        window.clear();
        window.draw(background);
        menu.draw(window);
        window.display();
    }
}
