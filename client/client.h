//
// Created by Тимофей Савинич on 17.03.24.
//

#ifndef COURSE_WORK_CLIENT_H
#define COURSE_WORK_CLIENT_H

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <getopt.h>


// includes by Nikita Bakunovich
#include <sys/types.h>
#include <cstdio>
#include <stdlib.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

bool client();
int client_v2();

#endif //COURSE_WORK_CLIENT_H
