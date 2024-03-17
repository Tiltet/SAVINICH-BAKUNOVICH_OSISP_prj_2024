//
// Created by Тимофей Савинич on 17.03.24.
//

#include "client.h"

#define PORT 8081

bool client()
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        std::cerr << "Ошибка при создании сокета" << std::endl;
        return 1;
    }

    std::string adressIP;
    std::cout << "Введите IP игры - ";
    std::cin >> adressIP;

    struct sockaddr_in serverAddress {};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    if (inet_pton(AF_INET, adressIP.c_str(), &(serverAddress.sin_addr)) != 1)
    {
        std::cerr << "Неверный IP-адрес" << std::endl;
        close(clientSocket);
        return 1;
    }

    if (connect(clientSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1)
    {
        std::cerr << "Ошибка при установлении соединения" << std::endl;
        close(clientSocket);
        return 1;
    }

    std::cout << "Соединение с сервером установлено" << std::endl;

    int number;
    std::cout << "Введите число: ";
    std::cin >> number;

    ssize_t bytesSent = send(clientSocket, &number, sizeof(number), 0);
    if (bytesSent == -1)
    {
        std::cerr << "Ошибка при отправке данных" << std::endl;
    }
    else if (bytesSent == 0)
    {
        std::cout << "Соединение закрыто" << std::endl;
    }
    else
    {
        std::cout << "Число успешно отправлено на сервер" << std::endl;
    }

    close(clientSocket);
    return 0;
}