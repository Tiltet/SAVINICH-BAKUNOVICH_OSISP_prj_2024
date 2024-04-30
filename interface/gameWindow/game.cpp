//
// Created by Тимофей Савинич on 29.03.24.
//

#include <utility>

#include "game.h"
#include "../../connection_funcs/con_funcs.h"
#include "../../user_client/user_client.h"

#define SERVER_IP "127.0.0.1"
#define PORT 8082
#define BUFFER_SIZE 1024

const int gridSize = 10;
const int cellSize = 60;



void convert_string_to_ints(const char* string, int* first_int, int* second_int) {
    if (strlen(string) != 2) {
        printf("Строка должна содержать ровно две цифры.\n");
        return;
    }

    if (!isdigit(string[0]) || !isdigit(string[1])) {
        printf("Строка должна содержать только цифры.\n");
        return;
    }

    *first_int = string[0] - '0';
    *second_int = string[1] - '0';
}

void add_letter_and_int(char *buffer, char letter, int number)
{
    snprintf(buffer, 2, "%c%d", letter, number);
}

game::Game::Game(sf::RenderWindow &window, sf::RectangleShape background, std::vector<std::vector<Cell>> map)
{
    char ip_address[16];
    char buffer[BUFFER_SIZE];
    int currentPlayer = 0;
    printf("Enter the IP address: ");
    fgets(ip_address, sizeof(ip_address), stdin);
    ip_address[strcspn(ip_address, "\n")] = '\0';

    std::cout << "Game" << std::endl;
    this->mapUser = std::move(map);

    window.setMouseCursorVisible(true);

    sf::Texture texture_window_background1;
    if (!texture_window_background1.loadFromFile("../interface/img/gameBack.jpg"))
    {
        return;
    }

    background.setTexture(&texture_window_background1);
    initMapEnemy();

    // Create client socket
    this->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        printf("Could not create socket\n");
        exit(1);
    }

    //    sf::SoundBuffer soundBuffer;
    //    if (!soundBuffer.loadFromFile("../interface/sounds/bam2.mp3"))
    //    {
    //        return;
    //    }

    //    sf::Sound sound;
    //    sound.setBuffer(soundBuffer);


    // Настройка адреса сервера
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12346);
    serverAddress.sin_addr.s_addr = inet_addr(ip_address);

    // Подключение к серверу
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
    {
        printf("Connection failed\n");
        exit(1);
    }

    printf("Connected to the server.\n");

    // Ожидание начала игры
    printf("Waiting for game to start...\n");
    receive_message(clientSocket, buffer);
    printf("%s\n", buffer);
    receive_message(clientSocket, buffer);
    printf("%s\n", buffer);

    if (strcmp(buffer, "Your turn: ") == 0)
    {
        currentPlayer = 0;
        std::cout << "Ходит первым\n" << std::endl;
    }
    else
    {
        currentPlayer = 1;
        std::cout << "Ходит вторым\n" << std::endl;
    }

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (currentPlayer == 1)
            {
                std::cout << "ИГРОК ЖДЕТ" << std::endl;
                window.setMouseCursorVisible(false);
                receive_message(clientSocket, buffer);
                printf("Wait: Буфер с сервера = %s\n", buffer);

                int x, y = 0;
                convert_string_to_ints(buffer, &x, &y);

                std::cout << "Координата x после парса = " << x << std::endl;
                std::cout << "Координата y после парса = " << y << std::endl;

                Cell& cell = this->mapUser[x][y];

                if (cell.state == CellState::Ship)
                {
                    cell.state = CellState::Hit;
                    cell.shape.setFillColor(sf::Color::Green);
                    std::cout << "Отправлено на сервер: Hit" << std::endl;
                    if (!checkKilled(x, y))
                    {
                        std::cout << "Отправлено на сервер: Hit" << std::endl;
                        send_message(clientSocket, "Hit");
                    }
                    else
                    {
                        std::cout << "Отправлено на сервер: Killed" << std::endl;
                        send_message(clientSocket, "Killed");
                    }
                }
                else
                {
                    cell.state = CellState::Miss;
                    cell.shape.setFillColor(sf::Color::Yellow);
                    std::cout << "Отправлено на сервер: Miss" << std::endl;
                    send_message(clientSocket, "Miss");
                }

                std::cout << "ОЖИДАНИЕ ЗАКОНЧЕНО\n\n" << std::endl;
                currentPlayer = 0;
            }
            else
            {
                window.setMouseCursorVisible(true);
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
                else if (event.type == sf::Event::MouseButtonPressed)   // событие нажатия на кнопку мыши
                {
                    if (event.mouseButton.button == sf::Mouse::Left)    // если нажата левая клавиша мыши
                    {
                        game::ShootCoordinates coordinates{};
                        std::cout << "ИГРОК СТРЕЛЯЕТ" << std::endl;

                        coordinates = shoot(window);

                        std::cout << "Координа выстрела x = " << coordinates.x << std::endl;
                        std::cout << "Координа выстрела y = " << coordinates.y << std::endl;

                        receive_message(clientSocket, buffer);
                        printf("Shoot: Буфер с сервера = %s\n", buffer);

                        Cell& cell = this->mapEnemy[coordinates.x][coordinates.y];
                        if (std::strcmp(buffer, "Hit") == 0)
                        {
                            cell.state = CellState::Hit;
                            cell.shape.setFillColor(sf::Color::Green);
                            std::cout << "Получено с сервера: Hit" << std::endl;
                        }
                        else if (std::strcmp(buffer, "Miss") == 0)
                        {
                            cell.state = CellState::Miss;
                            cell.shape.setFillColor(sf::Color::Yellow);
                            std::cout << "Получено с сервера: Miss" << std::endl;
                        }
                        else if (std::strcmp(buffer, "Killed") == 0)
                        {
                            cell.state = CellState::Killed;
                            cell.shape.setFillColor(sf::Color::Red);
                            std::cout << "Получено с сервера: Killed" << std::endl;
                        }

                        std::cout << "ВЫСТРЕЛ ЗАВЕРШЕН\n" << std::endl;

                        currentPlayer = 1;
                        // sleep(1);
                        //sound.play();
                    }
                }
            }
        }

        window.clear();
        window.draw(background);
        this->drawMaps(window);
        window.display();
    }
}

void game::Game::initMapEnemy()
{
    std::vector<std::vector<Cell>> mapUser(10, std::vector<Cell>(10));
    this->mapEnemy = mapUser;

    for (int row = 0; row < 10; ++row)
    {
        for (int col = 0; col < 10; ++col)
        {
            Cell& cell = this->mapEnemy[row][col];
            cell.state = CellState::Empty;

            cell.shape.setSize(sf::Vector2f(60, 60));
            cell.shape.setOutlineThickness(3.f);
            cell.shape.setFillColor(sf::Color::White);
            cell.shape.setOutlineColor(sf::Color::Black);
        }
    }
}

void game::Game::drawMaps(sf::RenderWindow& window)
{
    sf::Color colorBorder(0, 73, 194);
    sf::Color colorCell(202, 237, 247);

    for (int row = 0; row < 10; ++row)
    {
        for (int col = 0; col < 10; ++col)
        {
            Cell& cellUser = this->mapUser[row][col];
            cellUser.shape.setPosition(col * 60 + globalScreenWigth / 20, row * 60 + globalScreenHeight / 6);
            window.draw(cellUser.shape);

            Cell& cellEnemy = this->mapEnemy[row][col];
            cellEnemy.shape.setPosition(col * 60 + globalScreenWigth / 1.85, row * 60 + globalScreenHeight / 6);
            window.draw(cellEnemy.shape);
        }
    }
}

game::ShootCoordinates game::Game::shoot(sf::RenderWindow &window) const
{
    ShootCoordinates coordinates{};
    char buffer[BUFFER_SIZE];

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

    // Считаем номер колонки и строки в поле Enemy
    int colEnemy = (worldPos.x - globalScreenWigth / 1.85) / cellSize;
    int rowEnemy = (worldPos.y - globalScreenHeight / 6) / cellSize;

    if (colEnemy >= 0 && colEnemy < gridSize && rowEnemy >= 0 && rowEnemy < gridSize)
    {
        coordinates.x = rowEnemy;
        coordinates.y = colEnemy;

        std::cout << coordinates.x << " " << coordinates.y << std::endl;
        std::sprintf(buffer, "%s%s", std::to_string(coordinates.x).c_str(), std::to_string(coordinates.y).c_str());
        buffer[strcspn(buffer, "\n")] = '\0';
        std::cout << buffer << std::endl;
        send_message(clientSocket, buffer);
    }
    return coordinates;
}

bool game::Game::checkKilled(int x, int y) {
    if ((x < 9) && (this->mapUser[x+1][y].state == CellState::Empty || this->mapUser[x+1][y].state == CellState::Killed))
    {
        if ((x > 0) && (this->mapUser[x-1][y].state == CellState::Empty || this->mapUser[x-1][y].state == CellState::Killed))
        {
            if ((y < 9) && (this->mapUser[x][y+1].state == CellState::Empty || this->mapUser[x][y+1].state == CellState::Killed))
            {
                if ((y > 0) && (this->mapUser[x][y-1].state == CellState::Empty || this->mapUser[x][y-1].state == CellState::Killed))
                {
                    return true;
                }
            }
        }
    }
    return false;
}
