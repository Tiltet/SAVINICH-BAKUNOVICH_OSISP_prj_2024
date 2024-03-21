//
// Created by Тимофей Савинич on 17.03.24.
//

#ifndef COURSE_WORK_SERVER_H
#define COURSE_WORK_SERVER_H

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <getopt.h>
#include <ifaddrs.h>
#include <netinet/in.h>

// Created by Nikita Bakunovich

#include <sys/types.h>
#include <cstdio>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>

#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netdb.h>

#include <netinet/if_ether.h>


bool server();
int server_v2();
std::string getIP_1();

#endif //COURSE_WORK_SERVER_H
