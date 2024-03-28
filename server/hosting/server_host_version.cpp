#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8082
#define MAX_CLIENTS 5
#define BUFFER_SIZE 2000

struct client_info {
    int socket;
    struct sockaddr_in address;
};

void *handle_client(void *arg) {
    char buffer[BUFFER_SIZE];
    struct client_info *client = (struct client_info *)arg;
    int socket = client->socket;
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
    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;
    pthread_t thread_id;
    struct client_info *client_info_ptr;

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

    c = sizeof(struct sockaddr_in);
    while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c))) {
        client_info_ptr = (struct client_info *)malloc(sizeof(struct client_info));
        client_info_ptr->socket = client_sock;
        client_info_ptr->address = client;

        // Создание нового потока для обработки клиента
        if (pthread_create(&thread_id, NULL, handle_client, (void *)client_info_ptr) < 0) {
            perror("Не удалось создать поток для клиента");
            return 1;
        }

        // Освобождение ресурсов потока
        pthread_detach(thread_id);
    }

    if (client_sock < 0) {
        perror("Не удалось принять соединение");
        return 1;
    }

    return 0;
}