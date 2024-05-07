#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include "../connection_funcs/con_funcs.h"

#define BUFFER_SIZE 1024

void send_message_s(int socket, const char* message)
{
    if (send(socket, message, strlen(message), 0) == -1)
    {
        printf("Error sending message\n");
        exit(1);
    }
}

void receive_message_s(int socket, char* buffer)
{
    ssize_t bytesReceived = recv(socket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived == -1)
    {
        printf("Error receiving message\n");
        exit(1);
    }
    buffer[bytesReceived] = '\0';
}

int server_host()
{
    int serverSocket, player1Socket, player2Socket;;
    struct sockaddr_in serverAddress, clientAddress;
    char buffer[BUFFER_SIZE];

    // Создание сокета
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        printf("Could not create socket\n");
        exit(1);
    }

    // Настройка адреса сервера
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Привязка сокета к адресу сервера
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
    {
        printf("Bind failed\n");
        exit(1);
    }

    char hostname[1024];
    gethostname(hostname, sizeof(hostname));
    //net_scan(hostname);
    std::cout << "IP игры - " << getIpForOS(hostname) << std::endl;
    std::cout << "Ожидание подключения клиента..." << std::endl;
    printf("Waiting for clients to connect...\n");

    // Ожидание подключения клиента
    listen(serverSocket, 2);
    printf("Waiting for player 1 to connect...\n");
    player1Socket = accept(serverSocket, NULL, NULL);
    printf("Player 1 connected\n");
    send_message_s(player1Socket, "Connected to server. Waiting for player 2 to join...");

    // Ожидание подключения второго игрока
    printf("Waiting for player 2 to connect...\n");
    player2Socket = accept(serverSocket, NULL, NULL);
    printf("Player 2 connected\n");
    send_message_s(player2Socket, "Connected to server. Game is starting...");

    // Игровой цикл
    send_message_s(player1Socket, "Your turn: ");
    send_message_s(player2Socket, "Player 1 turn: ");
        // Игровой цикл для текущей партии
        int currentPlayer = 0;
        int kills1 = 0;
        int kills2 = 0;
        while (1) {

            if (currentPlayer == 0)
            {
                receive_message_s(player1Socket, buffer);
                printf("Player 1's turn: %s\n", buffer); // Для отkладки

                send_message_s(player2Socket, buffer);
                receive_message_s(player2Socket, buffer);
                printf("Buffer: %s\n", buffer);
                if (std::strcmp(buffer, "Killed") == 0)
                {
                    kills1++;
                    if (kills1 == 10)
                    {
                        printf("Player 1 Win.\n");
                        break;
                    }
                    send_message_s(player1Socket, buffer);

                }
                else if (std::strcmp(buffer, "Hit") == 0)
                {
                    send_message_s(player1Socket, buffer);
                }
                else
                {
                    send_message_s(player1Socket, buffer);
                    currentPlayer = 1 - currentPlayer;
                }

            }
            else
            {
                receive_message_s(player2Socket, buffer);
                printf("Player 2's turn: %s\n", buffer); // Для отладки
                send_message_s(player1Socket, buffer);
                receive_message_s(player1Socket, buffer);
                printf("Buffer: %s\n", buffer);
                if (std::strcmp(buffer, "Killed") == 0)
                {
                    kills2++;
                    if (kills2 == 10)
                    {
                        printf("Player 2 Win.\n");
                        break;
                    }
                    send_message_s(player2Socket, buffer);

                }
                else if (std::strcmp(buffer, "Hit") == 0)
                {
                    send_message_s(player2Socket, buffer);
                }
                else
                {
                    send_message_s(player2Socket, buffer);
                    currentPlayer = 1 - currentPlayer;
                }

            }
            sleep(1);
        }

        // Отправка результатов игры игрокам
        if (kills1 == 10)
        {
            send_message_s(player1Socket, "Victory");
            send_message_s(player2Socket, "Lose");

        }
        else
        {
            send_message_s(player1Socket, "Lose");
            send_message_s(player2Socket, "Victory");
        }

        // Ожидание команды для начала новой партии
        receive_message_s(player1Socket, buffer);
        receive_message_s(player2Socket, buffer);

    // Закрытие соединений
    close(serverSocket);
    close(player1Socket);
    close(player2Socket);

    return 0;
}