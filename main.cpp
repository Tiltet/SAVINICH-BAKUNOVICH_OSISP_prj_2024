#include <iostream>
#include <getopt.h>
#include <SFML/Graphics.hpp>

#include "server/server.h"
#include "client/client.h"

int main(int argc, char *argv[])
{
    // SFML test code
    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Application");
    sf::CircleShape shape;
    shape.setRadius(40.f);
    shape.setPosition(100.f, 100.f);
    shape.setFillColor(sf::Color::Cyan);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
    // SFML test code


    int option;
    int s_option = 0;
    int c_option = 0;

struct option long_options[] =
{
    {"s", no_argument, &s_option, 1},
    {"c", no_argument, &c_option, 1},
    {nullptr, 0, nullptr, 0}
};

    while ((option = getopt_long(argc, argv, "sc", long_options, nullptr)) != -1) {
        switch (option) {
            case 's':
                s_option = 1;
                break;
            case 'c':
                c_option = 1;
                break;
            default:
                std::cout << "Введи маркер запуска игра Морской Бой: \n -s - сервер \n -c - клиент";
                break;
        }
    }

    if (s_option == 1 && c_option == 1)
    {
        std::cerr << "Ошибка: Нельзя указывать оба маркера -s и -c одновременно." << std::endl;
        return 1;
    }

    if (s_option == 1)
    {
        std::cout << "Запускаем сервер..." << std::endl;
        bool num = server();
        return num;
    }
    else if (c_option == 1)
    {
        bool num = client();
        return num;
    }
    else
    {
        std::cerr << "Ошибка";
        return 1;
    }
}

