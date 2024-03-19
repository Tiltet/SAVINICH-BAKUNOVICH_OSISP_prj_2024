//
// Created by nikitabakunovich on 19.03.24.
//
#include "server.h"
#include "../utilities/func.h"

#define PORT 8082

int server_v2() {
    int socket = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(PORT);
    Bind(socket, (struct sockaddr*) &adr, sizeof adr);
    Listen(socket, 1);

    std::string adressIP = getIP();
    std::cout << "IP игры - " << adressIP << std::endl;
    std::cout << "Ожидание подключения клиента..." << std::endl;

    socklen_t adrlen = sizeof adr;
    int fd = Accept(socket, (struct sockaddr *) &adr, &adrlen);
    char buff[256];
    ssize_t nread;
    nread = read(fd, buff, 256);
    if (nread == -1) {
        perror("read failed");
        exit(EXIT_FAILURE);
    }
    if (nread == 0) {
        printf("END OF FILE occured\n");
    }
    write(STDOUT_FILENO, buff, nread);
    char answ[] = "Answer from server";
    write(fd, answ, sizeof answ);

    sleep(15);
    close(fd);
    close(socket);
    return 0;
}
