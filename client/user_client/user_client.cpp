#include "user_client.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#define BUFFER_SIZE 2000
#define PORT 8082

int user_client() {
    int socket_desc;
    struct sockaddr_in serverAddress;
    char message[BUFFER_SIZE], server_reply[BUFFER_SIZE];
    char ip_address[16];

    // Создание сокета
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Не удалось создать сокет\n");
        return 1;
    }

    // Prompt user to enter the IP address
    printf("Enter the IP address: ");
    fgets(ip_address, sizeof(ip_address), stdin);
    ip_address[strcspn(ip_address, "\n")] = '\0';

    // Настройка адреса сервера и порта
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    if (inet_pton(AF_INET, ip_address, &(serverAddress.sin_addr)) != 1)
    {
        std::cerr << "Неверный IP-адрес" << std::endl;
        close(socket_desc);
        return 1;
    }

    // Подключение к серверу
    if (connect(socket_desc, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Не удалось подключиться");
        return 1;
    }

    printf("Подключено к серверу\n");

    while (1) {

        printf("Введите сообщение: ");
        fgets(message, BUFFER_SIZE, stdin);

        // Отправка данных на сервер
        if (send(socket_desc, message, strlen(message), 0) < 0) {
            perror("Не удалось отправить данные");
            return 1;
        }

        // Получение ответа от сервера
        if (recv(socket_desc, server_reply, BUFFER_SIZE, 0) < 0) {
            perror("Не удалось получить данные от сервера");
            break;
        }

        printf("Ответ сервера: %s\n", server_reply);
    }

    // Закрытие сокета
    // Закрытие сокета
    close(socket_desc);
    printf("Сокет закрыт\n");

    return 0;
}