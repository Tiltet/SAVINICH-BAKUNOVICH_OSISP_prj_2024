//
// Created by nikitabakunovich on 19.03.24.
//
#include "server.h"
#include "../utilities/func.h"
#include "../logicpart/logic.h"
#include "../logicpart/place/place.h"

#define PORT 8082
#define MAX_BUFFER 1024
#define SIZE 10


void func(int connfd) {
    char buff[MAX_BUFFER];
    int n;
    for (;;) {
        bzero(buff, MAX_BUFFER);
        read(connfd, buff, sizeof(buff));
        printf("From client: %s\t To client : ", buff);
        bzero(buff, MAX_BUFFER);
        n = 0;
        while ((buff[n++] = getchar()) != '\n');
        write(connfd, buff, sizeof(buff));
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

void* handle_client(void* arg) {
    int connfd = *(int*)arg;

    int board_player1[SIZE][SIZE] = {
            // Инициализируйте поле морского боя здесь или используйте вашу собственную логику
    };

    // Преобразование поля морского боя в строку
    char board_str[SIZE * SIZE + 1];  // +1 для завершающего нулевого символа
    int index = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board_str[index++] = '0' + board_player1[i][j];  // Предполагая, что значения на поле могут быть от 0 до 9
        }
    }
    board_str[index] = '\0';  // Завершающий нулевой символ```cpp
    // Отправка поля морского боя клиенту
    send(connfd, board_str, SIZE * SIZE, 0);
    char strData[MAX_BUFFER];
    while (1) {
        bzero(strData, MAX_BUFFER);
        recv(connfd, strData, MAX_BUFFER, 0);
        printf("Received message: %s\n", strData);

        // Обработка полученного сообщения
        // ...

        // Отправка ответа клиенту
        send(connfd, strData, strlen(strData), 0);

        if (strncmp("exit", strData, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
    close(connfd);
    pthread_exit(NULL);
}


int server_v2() {
    //Начало игрового процесса
    int board_player1[SIZE][SIZE];
    int chs = 0;
    int fl = 0;
    // while(fl != 1) {
//        printf("Enter numb:\n"
//               "1.Auto place\n"
//               "2.Manual place\n"
//               "3.Exit\n");
//        scanf("%d", chs);
//        check_numb(&chs, 1, 3);
//        if (chs == 0) {
//            place_ships(board_player1);
//        } else if (chs == 1) {
//            place(board_player1);
//        } else {
//            printf("Exiting...\n");
//            exit(EXIT_SUCCESS);
//        }
    place(board_player1);
    printf("Your ground: \n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", board_player1[i][j]);
        }
        printf("\n");
    }
//        printf("Are you ready to play ?\n"
//               "1.Yes\n"
//               "2.No\n");
//        scanf("%d", fl);
    //check_numb(&fl, 1, 2);
    //}


    int socket = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr, cli = {0};

    char hostname[1024];
    gethostname(hostname, sizeof(hostname));

    adr.sin_family = AF_INET;
    adr.sin_port = htons(PORT);
    adr.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(socket, (struct sockaddr *) &adr, sizeof adr);
    Listen(socket, 1);

    net_scan(hostname);
    printf("Хост: %s\n", hostname);

    std::cout << "IP игры - " << getIpForOS(hostname) << std::endl;
    std::cout << "Ожидание подключения клиента..." << std::endl;



    while (1) {
        struct sockaddr_in cliaddr = {0};
        socklen_t clilen = sizeof(cliaddr);
        int connfd = accept(socket, (struct sockaddr*)&cliaddr, &clilen);
        if (connfd < 0) {
            perror("Accepting failed");
            exit(EXIT_FAILURE);
        }

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, &connfd);
    }

    return 0;
}