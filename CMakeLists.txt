cmake_minimum_required(VERSION 3.27)
project(Course_Work)

set(CMAKE_CXX_STANDARD 17)

add_executable(SeaBattle main.cpp
        server/server.cpp
        server/server.h
        client/client.h
        client/client.cpp
)
