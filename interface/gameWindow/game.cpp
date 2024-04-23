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

void add_letter_and_int(char *buffer, char letter, int number) {
    // Выделение памяти для строки (буква + число + завершающий нуль-символ)
    //char* result = (char*)malloc(3 * sizeof(char));

//    if (result == NULL) {
//        // Обработка ошибки выделения памяти
//        return NULL;
//    }

    // Заполнение строки с использованием snprintf
    snprintf(buffer, 2, "%c%d", letter, number);

    //return result;
}

void game::Game::sendShootData(int x, int y) {
    std::string message = std::to_string(x) + "," + std::to_string(y);
    if (send(clientSocket, message.c_str(), message.size(), 0) < 0) {
        perror("Failed to send shoot data to the server");
    }
}

std::pair<int, int> game::Game::receiveServerResponse() {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    if (recv(clientSocket, buffer, sizeof(buffer) - 1, 0) < 0) {
        perror("Failed to receive server response");
        return {-1, -1}; // Возвращаем пару (-1, -1) в случае ошибки
    } else {
        std::cout << "Server response: " << buffer << std::endl;
        // Разбиваем полученную строку на две части по запятой
        std::string response(buffer);
        size_t pos = response.find(',');
        if (pos != std::string::npos) {
            std::string xStr = response.substr(0, pos);
            std::string yStr = response.substr(pos + 1);
            // Преобразуем строки в целые числа
            int x = std::stoi(xStr);
            int y = std::stoi(yStr);

            // ЗАКРАСИЛ КЛЕТОЧКУ
            Cell &cell = this->mapUser[x][y];

            if (cell.state == CellState::Ship)
            {
                std::cout << "Попал" << std::endl;
                cell.shape.setFillColor(sf::Color::Black);
                cell.state = CellState::Hit;
            }
            else if (cell.state == CellState::Empty)
            {
                std::cout << "Мимо" << std::endl;
                cell.shape.setFillColor(sf::Color::Yellow);
                cell.state = CellState::Hit;
            }

            return {x, y};
        } else {
            // Если формат ответа не соответствует ожидаемому, возвращаем пару (-1, -1)
            return {-1, -1};
        }
    }
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
    this-> clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        printf("Could not create socket\n");
        exit(1);
    }

    //sf::SoundBuffer soundBuffer;
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
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        printf("Connection failed\n");
        exit(1);
    }

    //printf("Connected to the server.\n");

    // Ожидание начала игры
    printf("Waiting for game to start...\n");
    receive_message(clientSocket, buffer);
    printf("%s\n", buffer);
    //sleep(1);
    while (window.isOpen())
    {
        sf::Event event;
        receive_message(clientSocket, buffer);
        printf("%s\n", buffer);
        sleep(1);
        if (strcmp(buffer, "Your turn: ") == 0) {
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
                else if (event.type == sf::Event::MouseButtonPressed)   // событие нажатия на кнопку мыши
                {
                    if (event.mouseButton.button == sf::Mouse::Left)    // если нажата левая клавиша мыши
                    {
                        int x, y = 0;
                        shoot(window, &x, &y);
                        printf("%d, %d", x, y);
                        parse_shot(buffer, &x, &y);
                        receive_message(clientSocket, buffer);
                        printf("Shoot status: %s\n", buffer);
                        if (strcmp("Hit", buffer) == 0) {
                            Cell& cell = this->mapEnemy[y][x];

                            if (cell.state == CellState::Ship)
                            {
                                cell.state = CellState::Hit;
                                cell.shape.setFillColor(sf::Color::Red);
                                std::cout << "hit" << std::endl;
                                std::cout << "Row = " << x << std::endl;
                                std::cout << "Col = " << y << std::endl;
                            }
                            //send_message(clientSocket, "Hit");
                        } else {
                            Cell& cell = this->mapEnemy[x][y];
                            if (cell.state == CellState::Empty)
                            {
                                cell.state = CellState::Miss;
                                cell.shape.setFillColor(sf::Color::Blue);
                                std::cout << "miss" << std::endl;
                                std::cout << "Row = " << x << std::endl;
                                std::cout << "Col = " << y << std::endl;
                            }
                            //send_message(clientSocket, "Miss");
                        }
                        currentPlayer = 1 + currentPlayer;

                        //shoot(window);
                        //sound.play();
                    }
                }
            }
        }
        else
        {
            //window.setMouseCursorVisible(false);
            receive_message(clientSocket, buffer);
            printf("%s\n", buffer);
            int x, y = 0;
            parse_shot(buffer, &x, &y);
            printf("%d, %d", x, y);
            Cell& cell = this->mapUser[y][x];
            if (cell.state == CellState::Ship) {
                cell.state = CellState::Hit;
                cell.shape.setFillColor(sf::Color::Red);
                std::cout << "hit" << std::endl;
                send_message(clientSocket, "Hit");
            } else {
                cell.state = CellState::Miss;
                cell.shape.setFillColor(sf::Color::Blue);
                std::cout << "miss" << std::endl;
                send_message(clientSocket, "Miss");
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

game::ShootCoordinates game::Game::shoot(sf::RenderWindow &window, int *x, int*y)
{
    char buffer[BUFFER_SIZE];
    receive_message(clientSocket, buffer);
    printf("%s\n", buffer);
    if (strcmp(buffer, "Your turn: ") == 0) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

        // Считаем номер колонки и строки в поле Enemy
        int colEnemy = (worldPos.x - globalScreenWigth / 1.85) / cellSize;
        int rowEnemy = (worldPos.y - globalScreenHeight / 6) / cellSize;

        ShootCoordinates coordinates;

        if (colEnemy >= 0 && colEnemy < gridSize && rowEnemy >= 0 && rowEnemy < gridSize)
        {
//            Cell& cell = this->mapEnemy[rowEnemy][colEnemy];
//
//            if (cell.state == CellState::Empty)
//            {
//                cell.state = CellState::Miss;
//                cell.shape.setFillColor(sf::Color::Blue);
//                std::cout << "miss" << std::endl;
//                std::cout << "Row = " << rowEnemy << std::endl;
//                std::cout << "Col = " << colEnemy << std::endl;
//            }
//            else if (cell.state == CellState::Ship)
//            {
//                cell.state = CellState::Hit;
//                cell.shape.setFillColor(sf::Color::Red);
//                std::cout << "hit" << std::endl;
//                std::cout << "Row = " << rowEnemy << std::endl;
//                std::cout << "Col = " << colEnemy << std::endl;
//            }
            coordinates.x = rowEnemy;
            coordinates.y = colEnemy;
        }

        // Раскрываем опционал и выводим координаты выстрела в консоль
        std::cout << "Координаты выстрела x: ";
        if (*coordinates.x && *coordinates.y)
        {
            std::cout << *coordinates.x << " " << *coordinates.y << std::endl;
        }
        else
        {
            std::cout << "Неправильные координат\n";
        }
        *x = *coordinates.x;
        *y = *coordinates.y;
        //parse_shot(buffer, &x, &y);
        char letter = 'A' + *x - 1;
        //printf("%c\n", letter);
        add_letter_and_int(buffer, letter, *y);
        buffer[strcspn(buffer, "\n")] = '\0';
        send_message(clientSocket, buffer);
        return coordinates;
    }



    // Получаем ответ от сервера
//    auto serverResponse = receiveServerResponse();
//    if (serverResponse.first != -1 && serverResponse.second != -1) {
//        // Обработка полученных координат
//        std::cout << "Received coordinates from server: " << serverResponse.first << ", " << serverResponse.second << std::endl;
//        // Здесь вы можете добавить логику обработки координат, полученных от сервера
//    } else {
//        std::cout << "Failed to parse server response." << std::endl;
//    }
//    sendShootData(x, y);
}
