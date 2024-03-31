//
// Created by Тимофей Савинич on 19.03.24.
//

#include "interface.h"

const int gridSize = 10;
const int cellSize = 45;

void interfaceTest()
{
    setGlobalFont();
    setGlobalScreenSize();


    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("../interface/sounds/goToMenu.mp3"))
    {
        return;
    }

    sf::Sound sound;
    sound.setBuffer(soundBuffer);

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Sea Battle", sf::Style::Fullscreen);
    window.setMouseCursorVisible(false);

    sf::RectangleShape background(sf::Vector2f(globalScreenWigth, globalScreenHeight));
    sf::Texture texture_window_background1;
    if (!texture_window_background1.loadFromFile("../interface/img/background.jpg"))
    {
        return;
    }

    background.setTexture(&texture_window_background1);

    menu::Menu menu(globalScreenWigth / 2, globalScreenWigth / 10);

    menu.setTitle("Sea Battle", 144, sf::Color::White);
    menu.addItem("Classic", 86, sf::Color::White);
    menu.addItem("Fun Mode", 86, sf::Color::White);
    menu.addItem("Exit", 86, sf::Color::White);
    menu.alignMenu(3);

    int check = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    menu.moveUp();
                    sound.play();
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    menu.moveDown();
                    sound.play();
                }
                else if (event.key.code == sf::Keyboard::Return)
                {
                    int selectedItemIndex = menu.getSelectedItemIndex();
                    switch (selectedItemIndex)
                    {
                        case 0:
                            std::cout << "Start Classic Mode" << std::endl;
                            check = 1;
                            break;
                        case 1:
                            //TODO Fun Mode
                            std::cout << "Start Fun Mode" << std::endl;
                            window.close();
                            break;
                        case 2:
                            std::cout << "Exit" << std::endl;
                            window.close();
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        if (check == 0)
        {
            window.clear();
            window.draw(background);
            menu.draw(window);
            window.display();
        }
        else if (check == 1)
        {
            pre::Preparation preparation(window, background, menu::Menu
            (globalScreenWigth / 5, globalScreenHeight / 5));
            window.close();
        }
        else
        {
            window.close();
            // interface();
        }
    }
}