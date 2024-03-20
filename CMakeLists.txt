cmake_minimum_required(VERSION 3.17)
project(Course_Work)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)

add_executable(SeaBattle main.cpp
        server/server.cpp
        server/server.h
        client/client.h
        client/client.cpp
        interface/interface.cpp
)

include_directories(/usr/local/include)

set(CMAKE_PREFIX_PATH "/home/nikitabakunovich/Загрузки/SFML-2.5.1-linux-gcc-64-bit/SFML-2.5.1/lib/cmake/SFML" ${CMAKE_PREFIX_PATH})



find_package(SFML 2.5 COMPONENTS system window graphics network REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(SeaBattle sfml-system sfml-window sfml-graphics sfml-network)