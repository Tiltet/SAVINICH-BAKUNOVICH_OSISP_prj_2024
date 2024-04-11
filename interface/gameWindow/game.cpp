//
// Created by Тимофей Савинич on 29.03.24.
//

#include "game.h"
#include "../../client/client.h"
#include <utility>

#define SERVER_IP "127.0.0.1"
#define PORT 8082

const int gridSize = 10;
const int cellSize = 60;

void game::Game::handleServerMessages() {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        if (recv(client_socket, buffer, sizeof(buffer) - 1, 0) < 0) {
            perror("Failed to receive server message");
            break;
        } else {
            std::string message(buffer);
            std::cout << "Server message: " << message << std::endl;

            // Проверяем, кто ходит сейчас
            if (message.find("Your turn, player") != std::string::npos) {
                int currentPlayer = message.back() - '0'; // Последний символ сообщения указывает на текущего игрока
                std::cout << "It's player " << currentPlayer << "'s turn." << std::endl;
                // Здесь можно добавить логику для ожидания хода от текущего игрока

                // Получаем ответ от сервера
                std::pair<int, int> serverResponse = receiveServerResponse();
                if (serverResponse.first != -1 && serverResponse.second != -1) {
                    // Обработка полученных координат
                    std::cout << "Received coordinates from server: " << serverResponse.first << ", " << serverResponse.second << std::endl;
                    // Здесь вы можете добавить логику обработки координат, полученных от сервера
                    Cell &cell = this->mapUser[serverResponse.first][serverResponse.second];

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
                } else {
                    std::cout << "Failed to parse server response." << std::endl;
                }

            } else if (message.find("Player") != std::string::npos && message.find("wins!") != std::string::npos) {
                std::cout << "Game over. " << message << std::endl;

                // Здесь можно добавить логику для завершения игры


                break;
            }
        }
    }
}


void game::Game::sendShootData(int x, int y) {
    std::string message = std::to_string(x) + "," + std::to_string(y) + "\n";
    if (send(client_socket, message.c_str(), message.size(), 0) < 0) {
        perror("Failed to send shoot data to the server");
    }
}

std::pair<int, int> game::Game::receiveServerResponse() {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    if (recv(client_socket, buffer, sizeof(buffer) - 1, 0) < 0) {
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
//            Cell &cell = this->mapUser[x][y];
//
//            if (cell.state == CellState::Ship)
//            {
//                std::cout << "Попал" << std::endl;
//                cell.shape.setFillColor(sf::Color::Black);
//                cell.state = CellState::Hit;
//            }
//            else if (cell.state == CellState::Empty)
//            {
//                std::cout << "Мимо" << std::endl;
//                cell.shape.setFillColor(sf::Color::Yellow);
//                cell.state = CellState::Hit;
//            }

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
    this->client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Failed to create client socket");
    }

    //sf::SoundBuffer soundBuffer;
//    if (!soundBuffer.loadFromFile("../interface/sounds/bam2.mp3"))
//    {

//        return;
//    }

//    sf::Sound sound;
//    sound.setBuffer(soundBuffer);
//    // Создание сокета и подключение к серверу
//    client_socket = socket(AF_INET, SOCK_STREAM, 0);
//    if (client_socket == -1) {
//        perror("Failed to create client socket");
//    }


    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, ip_address, &(server_address.sin_addr)) <= 0) {
        perror("Failed to convert server IP address");
    }

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Failed to connect to the server");
    }

    printf("Connected to the server.\n");
    while (window.isOpen())
    {
        sf::Event event;
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

                    shoot(window);
                    //sound.play();
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

game::ShootCoordinates game::Game::shoot(sf::RenderWindow &window)
{

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

    // Считаем номер колонки и строки в поле Enemy
    int colEnemy = (worldPos.x - globalScreenWigth / 1.85) / cellSize;
    int rowEnemy = (worldPos.y - globalScreenHeight / 6) / cellSize;

    ShootCoordinates coordinates;

    if (colEnemy >= 0 && colEnemy < gridSize && rowEnemy >= 0 && rowEnemy < gridSize)
    {
        Cell& cell = this->mapEnemy[rowEnemy][colEnemy];

        if (cell.state == CellState::Empty)
        {
            cell.state = CellState::Miss;
            cell.shape.setFillColor(sf::Color::Blue);
            std::cout << "miss" << std::endl;
            std::cout << "Row = " << rowEnemy << std::endl;
            std::cout << "Col = " << colEnemy << std::endl;
        }
        else if (cell.state == CellState::Ship)
        {
            cell.state = CellState::Hit;
            cell.shape.setFillColor(sf::Color::Red);
            std::cout << "hit" << std::endl;
            std::cout << "Row = " << rowEnemy << std::endl;
            std::cout << "Col = " << colEnemy << std::endl;
        }
        coordinates.x = rowEnemy;
        coordinates.y = colEnemy;
    }

    // Раскрываем опционал и выводим координаты выстрела в консоль
    std::cout << "Координаты выстрела: ";
    if (*coordinates.x && *coordinates.y)
    {
        std::cout << *coordinates.x << " " << *coordinates.y << std::endl;
    }
    else
    {
        std::cout << "Неправильные координат\n";
    }
    handleServerMessages();
    sendShootData(*coordinates.x, *coordinates.y);

    return coordinates;
}