#include <iostream>
#include <getopt.h>

#include "server/server.h"
#include "client/client.h"
#include "interface/interface.h"
#include "logicpart/logic.h"
#include "logicpart/place/place.h"


int main(int argc, char *argv[])
{
//    interface(); // начальная функция отрисовки интерфейса
//
//    int option;
//    int s_option = 0;
//    int c_option = 0;
//
//struct option long_options[] =
//{
//    {"s", no_argument, &s_option, 1},
//    {"c", no_argument, &c_option, 1},
//    {nullptr, 0, nullptr, 0}
//};
//
//    while ((option = getopt_long(argc, argv, "sc", long_options, nullptr)) != -1) {
//        switch (option) {
//            case 's':
//                s_option = 1;
//                break;
//            case 'c':
//                c_option = 1;
//                break;
//            default:
//                std::cout << "Введи маркер запуска игра Морской Бой: \n -s - сервер \n -c - клиент";
//                break;
//        }
//    }
//
//    if (s_option == 1 && c_option == 1)
//    {
//        std::cerr << "Ошибка: Нельзя указывать оба маркера -s и -c одновременно." << std::endl;
//        return 1;
//    }
//
//    if (s_option == 1)
//    {
//        std::cout << "Запускаем сервер..." << std::endl;
//        int num = server_v2();
//        return num;
//    }
//    else if (c_option == 1)
//    {
//        int num = client_v2();
//        return num;
//    }
//    else
//    {
//        std::cerr << "Ошибка";
//        return 1;
//    }
        logic();
        //place();
}

