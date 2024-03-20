//
// Created by nikitabakunovich on 19.03.24.
//

#include "func.h"
#include "../server/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <ifaddrs.h>
#include <netinet/in.h>
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

void getIPAddress(char *ipAddress) {
    int sockfd;
    struct ifreq ifr;
    struct sockaddr_in *sin;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    struct ifconf ifc;
    char buf[4096];

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;

    if (ioctl(sockfd, SIOCGIFCONF, &ifc) == -1) {
        perror("Unable to get interface configuration");
        close(sockfd);
        exit(1);
    }

    struct ifreq *ifrArray = (struct ifreq *)ifc.ifc_req;
    int numInterfaces = ifc.ifc_len / sizeof(struct ifreq);

    int i;
    for (i = 0; i < numInterfaces; i++) {
        strncpy(ifr.ifr_name, ifrArray[i].ifr_name, IFNAMSIZ);

        if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) == 0) {
            if (ifr.ifr_flags & IFF_UP) {
                if (ioctl(sockfd, SIOCGIFADDR, &ifr) < 0) {
                    perror("Unable to get IP address");
                    close(sockfd);
                    exit(1);
                }

                sin = (struct sockaddr_in *)&ifr.ifr_addr;
                strcpy(ipAddress, inet_ntoa(sin->sin_addr));
                break;
            }
        } else {
            perror("Unable to get interface flags");
            close(sockfd);
            exit(1);
        }
    }

    close(sockfd);
}

char *get_ip_address(char *hostname) {
    struct hostent *host_info;
    char **ip_list;
    char *ip_address = NULL;

    host_info = gethostbyname(hostname);
    if (host_info == NULL) {
        printf("Ошибка при получении информации о хосте.\n");
        return NULL;
    }

    ip_list = host_info->h_addr_list;
    for (int i = 0; ip_list[i] != NULL; i++) {
        struct in_addr ip;
        memcpy(&ip, ip_list[i], sizeof(ip));
        ip_address = strdup(inet_ntoa(ip));
        break; // Выбираем первый IP-адрес из списка
    }

    return ip_address;
}

void net_scan(char *hostname) {
    struct hostent *host_info;
    char **ip_list;
    char *ip_address = NULL;

    host_info = gethostbyname(hostname);
    if (host_info == NULL) {
        printf("Ошибка при получении информации о хосте.\n");
        //return NULL;
    }

    ip_list = host_info->h_addr_list;
    for (int i = 0; ip_list[i] != NULL; i++) {
        struct in_addr ip;
        memcpy(&ip, ip_list[i], sizeof(ip));
        ip_address = strdup(inet_ntoa(ip));
        printf("Доступные IP адреса для подключения: %s\n", ip_address);
        //break; // Выбираем первый IP-адрес из списка
    }
}
