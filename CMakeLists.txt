cmake_minimum_required(VERSION 3.27)
project(Course_Work)

set(CMAKE_CXX_STANDARD 17)

add_executable(SeaBattle main.cpp
        logicpart/logic.cpp
        logicpart/logic.h
        hosting/server_host_version.cpp
        user_client/user_client.cpp
        connection_funcs/con_funcs.cpp
        interface/interface.cpp
        logicpart/place/place.cpp
        logicpart/place/place.h
        interface/menuBlock/menu.h
        interface/menuBlock/menu.cpp
        interface/preparationWindow/preparation.cpp
        interface/preparationWindow/preparation.h
        interface/global/global.cpp
        interface/global/global.h
        interface/gameWindow/game.h
        interface/gameWindow/game.cpp
        hosting/server_host_version.cpp
        interface/ipInputWindow/inputIp.cpp
        interface/ipInputWindow/inputIp.h
        interface/victoryWindow/victoryWindow.cpp
        interface/victoryWindow/vicrotyWindow.h
)

include_directories(/usr/local/include)

#путь к папке с SFML на Fedora39
set(CMAKE_PREFIX_PATH "/home/lib64/cmake/SFML" ${CMAKE_PREFIX_PATH})

find_package(SFML 2.5 COMPONENTS system window audio graphics network REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(SeaBattle sfml-system sfml-window sfml-audio sfml-graphics sfml-network)