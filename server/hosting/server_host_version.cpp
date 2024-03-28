#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8082
#define MAX_CLIENTS 2
#define BUFFER_SIZE 2000

int currentPlayer = 0;

struct client_info {
    int socket;
    struct sockaddr_in address;
};

void handleClient(int client_socket) {
    char buffer[BUFFER_SIZE];
    int n;

    while (1) {
        n = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (n > 0) {
            printf("Получено сообщение от клиента %d: %s\n", currentPlayer, buffer);
            // Переключение на другого игрока
            currentPlayer = 1 - currentPlayer;
            // Пересылка сообщения другому клиенту
            send(client_socket, buffer, n, 0);
        } else {
            break;
        }
    }
}

void *handle_client_v2(void *arg) {
    char buffer[BUFFER_SIZE];
    struct client_info *client = (struct client_info *)arg;
    int socket = client->socket;
    int new_socket;
    struct sockaddr_in address = client->address;
    char client_address[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(address.sin_addr), client_address, INET_ADDRSTRLEN);

    printf("Подключен клиент %s:%d\n", client_address, ntohs(address.sin_port));

    while (1) {
        memset(buffer, '\0', sizeof(buffer));

        // Принимаем данные от клиента
        ssize_t received = recv(socket, buffer, sizeof(buffer), 0);
        if (received < 0) {
            perror("Ошибка получения данных от клиента");
            break;
        } else if (received == 0) {
            printf("Клиент отключен: %s:%d\n", client_address, ntohs(address.sin_port));
            break;
        }

        // Обработка данных от клиента
        printf("Сообщение от клиента %s:%d: %s\n", client_address, ntohs(address.sin_port), buffer);

        // Отправляем данные клиенту
        if (send(socket, buffer, strlen(buffer), 0) < 0) {
            perror("Не удалось отправить данные клиенту");
            break;
        }
    }

    // Закрываем сокет клиента
    close(socket);
    free(client);
    pthread_exit(NULL);
}

int server_host() {
    int socket_desc, client_sock, c, new_socket;
    struct sockaddr_in server, client;
    pthread_t thread_id;
    struct client_info *client_info_ptr;
    int addrlen = sizeof(server);

    // Создание сокета
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Не удалось создать сокет\n");
        return 1;
    }

    // Подготовка структуры сервера
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Привязка сокета
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Не удалось привязать сокет");
        return 1;
    }

    printf("Сервер запущен на порту %d\n", PORT);

    // Ожидание подключения клиентов
    listen(socket_desc, MAX_CLIENTS);

    printf("Ожидание подключения клиентов...\n");

    int client_sockets[MAX_CLIENTS];
    int client_count = 0;
    while (client_count < MAX_CLIENTS) {
        if ((new_socket = accept(socket_desc, (struct sockaddr *)&server, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        client_sockets[client_count] = new_socket;
        client_count++;
    }

    // Обработка клиентов
    for (int i = 0; i < MAX_CLIENTS; i++) {
        handleClient(client_sockets[i]);
    }

    // Закрытие сокетов
    for (int i = 0; i < MAX_CLIENTS; i++) {
        close(client_sockets[i]);
    }

    return 0;
}