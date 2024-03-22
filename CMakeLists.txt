cmake_minimum_required(VERSION 3.27)
project(Course_Work)

set(CMAKE_CXX_STANDARD 17)

add_executable(SeaBattle main.cpp
        server/server.h
        client/client.h
        utilities/func.h
        logicpart/logic.cpp
        logicpart/logic.h
        server/hosting/server_host_version.cpp
        client/user_client/user_client.cpp
        server/connection_funcs/con_funcs.cpp
        interface/interface.cpp
        logicpart/place/place.cpp
        logicpart/place/place.h
)

include_directories(/usr/local/include)

#путь к папке с SFML на Fedora39
set(CMAKE_PREFIX_PATH "/home/lib64/cmake/SFML" ${CMAKE_PREFIX_PATH})

find_package(SFML 2.5 COMPONENTS system window graphics network REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(SeaBattle sfml-system sfml-window sfml-graphics sfml-network)