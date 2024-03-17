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

bool server();
std::string getIP();

#endif //COURSE_WORK_SERVER_H
