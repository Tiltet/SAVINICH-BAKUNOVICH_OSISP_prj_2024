#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime>
#include <cctype>

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

void init_ships(char board[10][10]) {
    // Генерация случайного расположения кораблей
    srand(time(NULL));
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            board[i][j] = '-';
        }
    }
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

void print_board(char playerBoard[10][10], char enemyBoard[10][10]) {
    int i, j;

    printf("   A B C D E F G H I J");
    printf("     ");
    printf("   A B C D E F G H I J\n");

    for (i = 0; i < 10; i++) {
        printf("%d  ", i);
        for (j = 0; j < 10; j++) {
            printf("%c ", playerBoard[i][j]);
        }
        printf("    ");
        printf("%d  ", i);
        for (j = 0; j < 10; j++) {
            printf("%c ", enemyBoard[i][j]);
        }
        printf("\n");
    }
}

bool is_valid_coordinate(const char* coordinate) {
    if (coordinate[0] < 'A' || coordinate[0] > 'J')
        return false;

    if (coordinate[1] < '0' || coordinate[1] > '9')
        return false;

    return true;
}

bool parse_shot(const char* coordinates, int* x, int* y) {
    if (coordinates == NULL || coordinates[0] < 'A' || coordinates[0] > 'J' ||
        coordinates[1] < '0' || coordinates[1] > '9') {
        return false; // Некорректный формат координат
    }

    *x = coordinates[0] - 'A'; // Преобразование буквы в индекс (0-9)
    *y = coordinates[1] - '0'; // Преобразование числа в индекс (0-9)

    return true;
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
    char player2Board[10][10];
    auto_place_ships(player1Board);
    init_ships(player2Board);
    printf("Your board: \n");
    print_board(player1Board, player2Board);
    int currentPlayer = 0;
    // Игровой цикл
    while (1) {

        receive_message(clientSocket, buffer);
        printf("%s\n", buffer);
        if (strcmp(buffer, "Your turn: ") == 0) {

            //Shoot
            fgets(buffer, BUFFER_SIZE, stdin);
            while (!is_valid_coordinate(buffer)) {
                printf("Введенные координаты являются недопустимыми.\n", buffer);
                fgets(buffer, BUFFER_SIZE, stdin);
                //buffer[0] = toupper(buffer[0]);
            }
            buffer[strcspn(buffer, "\n")] = '\0';
            send_message(clientSocket, buffer);
            int x, y = 0;
            parse_shot(buffer, &x, &y);

            //Opponent board (your shoot)
            receive_message(clientSocket, buffer);
            printf("Shoot status: %s\n", buffer);
            //parse_shot(buffer, &x, &y);
            if (strcmp("Hit", buffer) == 0) {
                player2Board[y][x] = 'X';
                //send_message(clientSocket, "Hit");
            } else {
                player2Board[y][x] = '0';
                //send_message(clientSocket, "Miss");
            }

//            receive_message(clientSocket, buffer);
//            if (player1Board[y][x] == 'S') {
//                player2Board[y][x] = 'X';
//                send_message(clientSocket, "Hit");
//            } else {
//                player2Board[y][x] = '0';
//                send_message(clientSocket, "Miss");
//            }
            currentPlayer = 1 + currentPlayer;
        } else {
            int x, y = 0;
            receive_message(clientSocket, buffer);
            printf("%s\n", buffer);
            parse_shot(buffer, &x, &y);
            //printf("%d, %d", x, y);
            if (player1Board[y][x] == 'S') {
                player1Board[y][x] = 'X';
                send_message(clientSocket, "Hit");
            } else {
                player1Board[y][x] = '0';
                send_message(clientSocket, "Miss");
            }
            currentPlayer = 1 - currentPlayer;
        }

        // Ход игрока
        //printf("Your turn: ");


//        fgets(buffer, BUFFER_SIZE, stdin);
//        while (!is_valid_coordinate(buffer)) {
//            printf("Введенные координаты являются недопустимыми.\n", buffer);
//            fgets(buffer, BUFFER_SIZE, stdin);
//            buffer[0] = toupper(buffer[0]);
//        }
//        buffer[strcspn(buffer, "\n")] = '\0';
//        send_message(clientSocket, buffer);
//
//        // Ожидание хода противника
//        printf("Waiting for opponent's turn...\n");
//        receive_message(clientSocket, buffer);
//        printf("Opponent's turn: %s\n", buffer);
//        parse_shot(buffer, &x, &y);
//        if (player1Board[y][x] == 'S') {
//            player1Board[y][x] = 'X';
//            send_message(clientSocket, "Hit");
//        } else {
//            player1Board[y][x] = '0';
//            send_message(clientSocket, "Miss");
//        }
//
//        receive_message(clientSocket, buffer);
//        printf("%s\n", buffer);
//        if (strcmp("Hit", buffer) == 0) {
//            player2Board[y][x] = 'X';
//        } else {
//            player2Board[y][x] = '0';
//        }

        print_board(player1Board, player2Board);
    }

    // Закрытие соединения
    close(clientSocket);

    return 0;
}