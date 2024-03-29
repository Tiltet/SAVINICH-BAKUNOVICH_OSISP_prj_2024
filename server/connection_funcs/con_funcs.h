//
// Created by nikitabakunovich on 22.03.24.
//

#ifndef COURSE_WORK_CON_FUNCS_H
#define COURSE_WORK_CON_FUNCS_H
#include <stdio.h>
#include <stdlib.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <string>
#include <netinet/if_ether.h>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <dirent.h>
#include <cstring>
int Socket(int domain, int type, int protocol);

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void Listen(int sockfd, int backlog);

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void Inet_pton(int af, const char *src, void *dst);

void Hostname(char* hostname);

std::string getIPLinux(char *hostname);

std::string getIPMAC();

void getIpForOS(char* hostname);
#endif //COURSE_WORK_CON_FUNCS_H
