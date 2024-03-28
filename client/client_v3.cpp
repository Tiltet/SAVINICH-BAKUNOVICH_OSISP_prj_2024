#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8082
#define BUFFER_SIZE 1024

int client() {
    int client_socket;
    struct sockaddr_in server_address;

    // Создание клиентского сокета
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Failed to create client socket");
        return 1;
    }

    // Настройка адреса сервера
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(server_address.sin_addr)) <= 0) {
        perror("Invalid server address");
        return 1;
    }

    // Подключение к серверу
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Failed to connect to the server");
        return 1;
    }

    printf("Connected to the server.\n");

    char buffer[BUFFER_SIZE];
    ssize_t received;

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        // Получение данных от сервера
        received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (received <= 0) {
            perror("Failed to receive data from the server");
            break;
        }

        printf("Received from server: %s\n", buffer);

        if (received > 0) {
            // Проверка, является ли это вашим ходом
            if (strcmp(buffer, "Your turn") == 0) {
                // Ваш ход. Введите ваш ход и отправьте на сервер
                printf("Your turn. Enter your move: ");
                fgets(buffer, sizeof(buffer), stdin);

                // Отправка хода на сервер
                ssize_t sent = send(client_socket, buffer, strlen(buffer), 0);
                if (sent < 0) {
                    perror("Failed to send data to the server");
                    break;
                }
            } else {
                // Ход другого игрока. Продолжайте ожидать своей очереди.
                printf("Waiting for other player's move...\n");
            }
        }
    }

    // Закрытие клиентского сокета
    close(client_socket);

    return 0;
}