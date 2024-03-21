//
// Created by Тимофей Савинич on 17.03.24.
//

#include "server.h"

#define PORT 8081

bool server()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        std::cerr << "Ошибка при создании сокета" << std::endl;
        return 1;
    }

    std::string adressIP = getIP();

    struct sockaddr_in serverAddress {};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8081);

    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1)
    {
        std::cerr << "Ошибка при привязке сокета" << std::endl;
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 1) == -1)
    {
        std::cerr << "Ошибка при прослушивании сокета" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "IP игры - " << adressIP << std::endl;
    std::cout << "Ожидание подключения клиента..." << std::endl;

    struct sockaddr_in clientAddress {};
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr*>(&clientAddress), &clientAddressLength);
    if (clientSocket == -1)
    {
        std::cerr << "Ошибка при принятии соединения" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Подключение клиента успешно" << std::endl;

    // Здесь вы можете отправлять и принимать данные через clientSocket

    int number;
    ssize_t bytesReceived = recv(clientSocket, &number, sizeof(number), 0);
    if (bytesReceived == -1)
    {
        std::cerr << "Ошибка при получении данных" << std::endl;
    }
    else if (bytesReceived == 0)
    {
        std::cout << "Клиент закрыл соединение" << std::endl;
    }
    else
    {
        std::cout << "Получено число от клиента: " << number << std::endl;
    }

    close(clientSocket);
    close(serverSocket);
    return 0;
}

std::string getIP_1()
{
    std::string ipAddress;
    struct ifaddrs* ifAddrStruct = nullptr;
    struct ifaddrs* ifa = nullptr;
    void* tmpAddrPtr = nullptr;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next)
    {
        if (!ifa->ifa_addr)
        {
            continue;
        }

        // IPv4 and interface name "en0"
        if (ifa->ifa_addr->sa_family == AF_INET && strcmp(ifa->ifa_name, "en0") == 0)
        {
            tmpAddrPtr = &reinterpret_cast<struct sockaddr_in*>(ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            ipAddress = std::string(addressBuffer);
            break;
        }
    }

    if (ifAddrStruct != nullptr)
    {
        freeifaddrs(ifAddrStruct);
    }

    return ipAddress;
}