#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime>

#define BUFFER_SIZE 1024

void send_message(int socket, const char* message) {
    if (send(socket, message, strlen(message), 0) == -1) {
        printf("Error sending message\n");
        exit(1);
    }
}

void receive_message(int socket, char* buffer) {
    ssize_t bytesReceived = recv(socket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived == -1) {
        printf("Error receiving message\n");
        exit(1);
    }
    buffer[bytesReceived] = '\0';
}

void auto_place_ships(char board[10][10]) {
    // Генерация случайного расположения кораблей
    srand(time(NULL));
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            board[i][j] = '-';
        }
    }
    for (i = 0; i < 5; i++) {
        int dx = rand() % 10;
        int dy = rand() % 10;
        board[dx][dy] = 'S';
    }
}

void print_board(char board[10][10]) {
    int i, j;
    printf("   ");
    for (i = 0; i < 10; i++) {
        printf("%d ", i);
    }
    printf("\n");
    for (i = 0; i < 10; i++) {
        printf("%d  ", i);
        for (j = 0; j < 10; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

int user_client() {
    int clientSocket;
    struct sockaddr_in serverAddress;
    char buffer[BUFFER_SIZE];

    // Создание сокета
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        printf("Could not create socket\n");
        exit(1);
    }

    // Настройка адреса сервера
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Подключение к серверу
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        printf("Connection failed\n");
        exit(1);
    }

    // Ожидание начала игры
    printf("Waiting for game to start...\n");
    receive_message(clientSocket, buffer);
    printf("%s\n", buffer);

    char player1Board[10][10];
    auto_place_ships(player1Board);
    printf("Your board: \n");
    print_board(player1Board);

    // Игровой цикл
    while (1) {
        // Получение результата хода
        //receive_message(clientSocket, buffer);
        //printf("%s\n", buffer);

        // Ход игрока
        printf("Your turn: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        send_message(clientSocket, buffer);

        // Проверка на завершение игры
//        if (strcmp(buffer, "Game over!") == 0) {
//            break;
//        }

        // Ожидание хода противника
        printf("Waiting for opponent's turn...\n");
        receive_message(clientSocket, buffer);
        printf("Opponent's turn: %s\n", buffer);
    }

    // Закрытие соединения
    close(clientSocket);

    return 0;
}