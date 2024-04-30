//
// Created by Тимофей Савинич on 23.04.24.
//

#include "inputIp.h"

ip::Ip::Ip(sf::RenderWindow &window, const sf::RectangleShape& background) {

    sf::Font textFont;

    if (!textFont.loadFromFile("../interface/fonts/MultiroundPro.otf"))
    {
        return;
    }

    sf::Text headerText;

    headerText.setFont(globalFont);
    headerText.setString("Title");
    headerText.setCharacterSize(148);
    headerText.setFillColor(sf::Color::Red);
    headerText.setPosition(globalScreenWigth / 2 - 148, globalScreenHeight / 6);

    sf::Text inputText;
    inputText.setFont(textFont);
    inputText.setCharacterSize(96);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(globalScreenWigth / 2 - 250, globalScreenHeight / 2 - 60);

    sf::RectangleShape textBox;
    textBox.setSize(sf::Vector2f(800, 120));
    textBox.setFillColor(sf::Color::Magenta);
    textBox.setFillColor(sf::Color::White);
    textBox.setPosition(globalScreenWigth / 2 - 400, globalScreenHeight / 2 - 50);


    std::string currentText;

    sf::Texture texture_window_background;
    if (!texture_window_background.loadFromFile("../interface/img/background2.jpg"))
    {
        return;
    }

    sf::Event event;
    while(window.isOpen())
    {

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128)
                {
                    if (event.text.unicode == '\b' && !currentText.empty())
                    {
                        currentText.pop_back();
                    }
                    else if (event.text.unicode != '\b')
                    {
                        if (currentText.size() < 6)
                        {
                            printf("Size = %ld", currentText.size());
                            currentText += static_cast<char>(event.text.unicode);
                        }
                    }

                    inputText.setString(currentText);
                }
            }
            if (event.key.code == sf::Keyboard::Return)
            {
                sf::String adress = inputText.getString();
                if (adress.isEmpty())
                {
                    printf("Error\n");

                }
                else
                {
                    std::string adressStr = adress.toAnsiString();
                    std::cout << adressStr << std::endl;
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(headerText);
        window.draw(textBox);
        window.draw(inputText);
        window.display();
    }
}
