//
// Created by nikitabakunovich on 19.03.24.
//
#include "server.h"
#include "../utilities/func.h"
#include "../logicpart/logic.h"

#define PORT 8082
#define MAX_BUFFER 1024

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
    char buffer[MAX_BUFFER];
    int n;

    while (1) {
        // Получение сообщения от клиента
        bzero(buffer, MAX_BUFFER);
        n = read(connfd, buffer, MAX_BUFFER);
        if (n > 0) {
            printf("From client: %s\n", buffer);
        }

        // Отправка сообщения клиенту
        printf("Enter message to client: ");
        fgets(buffer, MAX_BUFFER, stdin);
        write(connfd, buffer, strlen(buffer));

        // Проверка на команду выхода
        if (strncmp("exit", buffer, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }

    close(connfd);
    return NULL;
}


int server_v2() {
    int socket = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr, cli = {0};

    char hostname[1024];
    gethostname(hostname, sizeof(hostname));

    adr.sin_family = AF_INET;
    adr.sin_port = htons(PORT);
    adr.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(socket, (struct sockaddr*) &adr, sizeof adr);
    Listen(socket, 1);

    net_scan(hostname);
    printf("Хост: %s\n", hostname);

    std::cout << "IP игры - " << getIpForOS(hostname) << std::endl;
    std::cout << "Ожидание подключения клиента..." << std::endl;

    while (1) {
        socklen_t len = sizeof(cli);
        int fd = Accept(socket, (struct sockaddr *) &cli, &len);
        if (fd < 0) {
            printf("Server accept failed...\n");
            exit(0);
        }
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, &fd);
        game();
    }

    //close(socket);
    close(socket);
    return 0;
}

