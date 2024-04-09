#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "server_v3.h"
#include "connection_funcs/con_funcs.h"
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netdb.h>

#include <netinet/if_ether.h>


//#define SERVER_IP "172.20.10.15"
#define PORT 8082

int server_v3() {
    int server_socket, client_sockets[2];
    struct sockaddr_in server_address, client_addresses[2];
    socklen_t client_address_len = sizeof(client_addresses[0]);
    char buffer[1024];
    int current_player = 0; // 0 или 1, чтобы отслеживать текущего игрока

    // Создание серверного сокета
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Failed to create server socket");
        return 1;
    }

    // Подготовка адреса сервера
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Привязка сокета к порту
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Failed to bind server socket");
        return 1;
    }

    // Ожидание подключения клиентов
    if (listen(server_socket, 2) < 0) {
        perror("Failed to listen on server socket");
        return 1;
    }

    char hostname[1024];
    gethostname(hostname, sizeof(hostname));
    //net_scan(hostname);
    std::cout << "IP игры - " << getIpForOS(hostname) << std::endl;
    std::cout << "Ожидание подключения клиента..." << std::endl;
    printf("Waiting for clients to connect...\n");

    // Принятие соединений от двух клиентов
    for (int i = 0; i < 2; i++) {
        client_sockets[i] = accept(server_socket, (struct sockaddr *)&client_addresses[i], &client_address_len);
        if (client_sockets[i] < 0) {
            perror("Failed to accept client connection");
            return 1;
        }
        printf("Client %d connected.\n", i + 1);
    }

    // Основной цикл игры
    while (1) {
        // Отправка сигнала текущему игроку о начале его хода
        send(client_sockets[current_player], buffer, sizeof(buffer), 0);

        // Получение хода от текущего игрока
        memset(buffer, 0, sizeof(buffer));
        if (recv(client_sockets[current_player], buffer, sizeof(buffer), 0) <= 0) {
            perror("Failed to receive data from the client");
            break;
        }
        printf("Received from client %d: %s\n", current_player + 1, buffer);

        // Переключение на другого игрока
        current_player = 1 - current_player;
    }

    // Закрытие сокетов
    for (int i = 0; i < 2; i++) {
        close(client_sockets[i]);
    }
    close(server_socket);

    return 0;
}