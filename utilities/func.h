//
// Created by nikitabakunovich on 19.03.24.
//

#ifndef COURSE_WORK_FUNC_H
#define COURSE_WORK_FUNC_H
#include <sys/types.h>
#include <sys/socket.h>
#include "../server/server.h"
int Socket(int domain, int type, int protocol);

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void Listen(int sockfd, int backlog);

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void Inet_pton(int af, const char *src, void *dst);

void getIPAddress(char *ipAddress);

#endif //COURSE_WORK_FUNC_H
