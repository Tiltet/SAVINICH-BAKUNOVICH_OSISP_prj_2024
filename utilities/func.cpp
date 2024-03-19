//
// Created by nikitabakunovich on 19.03.24.
//

#include "func.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

void errorCatching(int res, const char* serror){
    if (res == -1) {
        perror(serror);
        exit(EXIT_FAILURE);
    }
}

int Socket(int domain, int type, int protocol) {
    int res = socket(domain, type, protocol);
    errorCatching(res, "Socket failure");
    return res;
}

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int res = bind(sockfd, addr, addrlen);
    errorCatching(res, "Bind failure");
}

void Listen(int sockfd, int clnum) {
    int res = listen(sockfd, clnum);
    errorCatching(res, "Listen failure");
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int res = accept(sockfd, addr, addrlen);
    errorCatching(res, "Accept failure");
    return res;
}

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int res = connect(sockfd, addr, addrlen);
    errorCatching(res, "Connect failure");
}

void Inet_pton(int af, const char *src, void *dst) {
    int res = inet_pton(af, src, dst);
    if (res == 0) {
        printf("inet_pton failed: src does not contain a character"
               " string representing a valid network address in the specified"
               " address family\n");
        exit(EXIT_FAILURE);
    }
    errorCatching(res, "inet_pton failure");
}