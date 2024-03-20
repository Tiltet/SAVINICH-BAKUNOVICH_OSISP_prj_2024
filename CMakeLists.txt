cmake_minimum_required(VERSION 3.27)
project(Course_Work)

set(CMAKE_CXX_STANDARD 17)

add_executable(SeaBattle main.cpp
        server/server.cpp
        server/server.h
        client/client.h
        client/client.cpp
        server/server_v2.cpp
        utilities/func.cpp
        utilities/func.h
        client/client_v2.cpp
        logicpart/logic.cpp
        logicpart/logic.h
        interface/interface.cpp
        interface/interface.h
)

include_directories(/usr/local/include)
#set(CMAKE_PREFIX_PATH "/home/nikitabakunovich/Загрузки/SFML-2.5.1-linux-gcc-64-bit/SFML-2.5.1/lib/cmake/SFML" ${CMAKE_PREFIX_PATH})

find_package(SFML 2.5 COMPONENTS system window graphics network REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(SeaBattle sfml-system sfml-window sfml-graphics sfml-network)