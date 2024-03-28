//
// Created by nikitabakunovich on 20.03.24.
//
#include "logic.h"
#include <ctime>
#include "place/place.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


#define SIZE 10
#define SHIPS 10

bool isShipsOverlap(int shipBoard[10][10], int startX, int startY, int dir, int length) {
    int i, j;
    for (i = 0; i < length; i++) {
        if (dir == 0) { // Вертикальное направление
            if (startY + i >= 10 || shipBoard[startX][startY + i] != '-') {
                return true; // Перекрытие или выход за границы
            }
        } else if (dir == 1) { // Горизонтальное направление
            if (startX + i >= 10 || shipBoard[startX + i][startY] != '-') {
                return true; // Перекрытие или выход за границы
                }
        }
    }
    return false; // Нет перекрытия
}

void logic() {
    int board[10][10] = {0};
    // Инициализация игрового поля
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }

   int chs = 0;
   scanf("%d", &chs);
   printf("%d\n", chs);
   if (chs == 1) {
       place(board);
   } else {
       place_ships(board);
   }
    // Расстановка кораблей
    //place_ships(board);

    // Вывод игрового поля (для демонстрации)
    printf("Your ground: \n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}



//void check_numb(int* size, int start, int end)
//{
//    long long a;
//    scanf("%lld", &a);
//    while (getchar() != '\n' || a < start || a > end || getchar() != ' ')
//    {
//        rewind(stdin);
//        printf("Error,try again\n");
//        scanf("%lld", &a);
//    }
//    *size = a;
//}

void check_numb(int *a, int start, int end) {
    char c;
    while (1) {
        if (scanf("%d", &a) == 1) {
            if (*a >= start && *a <= end) {
                break; // Ввод корректен, выходим из цикла
            } else {
                printf("Число выходит за пределы от 0 до 9. Попробуйте еще раз: ");
            }
        } else {
            // Очистка буфера ввода
            while ((c = getchar()) != '\n' && c != EOF) { }
            printf("Введенное значение не является числом. Попробуйте еще раз: ");
        }
    }

}

void enterPlace(int *x, int *y) {
    printf("Please choose ROW number.\n");
    scanf("%d", x);
    check_numb(x , 0, 9);
    printf("Please choose COLUMN number.\n");
    scanf("%d", y);
    check_numb(y, 0, 9);
}


//void placeCheck(bool status, const int *row, int *col, int board[10][10], int count, int direction, int ship_length) {
//    while (status != true) {
//        enterPlace(&row, &col);
//        status = isShipsOverlap(board, row, col, direction, ship_length);
//    }
//    for (count = 0; count < ship_length; count++) {
//        board[row + count][col] = '4';
//    }
//}

//void place_ships_auto(int board[SIZE][SIZE]) {
//    srand(time(NULL));
//
//    int shipSizes[SHIPS] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1}; // Размеры кораблей
//
//    for (int i = 0; i < SHIPS; i++) {
//        int shipSize = shipSizes[i];
//        int row, col, direction;
//
//        do {
//            row = rand() % SIZE;
//            col = rand() % SIZE;
//            direction = rand() % 2; // 0 - горизонтальное размещение, 1 - вертикальное размещение
//        } while (!isValidPlacement(board, row, col, direction, shipSize));
//
//        for (int j = 0; j < shipSize; j++) {
//            if (direction == 0) {
//                board[row][col + j] = 1; // Размещение корабля горизонтально
//            } else {
//                board[row + j][col] = 1; // Размещение корабля вертикально
//            }
//        }
//    }
//}

//int isValidPlacement(int board[SIZE][SIZE], int row, int col, int direction, int size) {
//    if (direction == 0) {
//        // Проверка, что корабль помещается в пределы доски по горизонтали
//        if (col + size > SIZE) {
//            return 0;
//        }
//
//        // Проверка наличия других кораблей в указанных клетках
//        for (int j = 0; j < size; j++) {
//            if (board[row][col + j] == 1) {
//                return 0;
//            }
//        }
//    } else {
//        // Проверка, что корабль помещается в пределы доски по вертикали
//        if (row + size > SIZE) {
//            return 0;
//        }
//
//        // Проверка наличия других кораблей в указанных клетках
//        for (int j = 0; j < size; j++) {
//            if (board[row + j][col] == 1) {
//                return 0;
//            }
//        }
//    }
//    return 1;
//}
void place_ships(int board[SIZE][SIZE]) {
        int ship_type = 1, ship_length = 0, row = 0, col = 0, direction = 0, count = 0;
        for (ship_type = 1; ship_type < 5; ship_type++) {
            bool status = false;
            switch (ship_type) {
                case 1: {
                    printf("Please enter where you would like to place your Battleship (4 spots).\n\n");
                    ship_length = 4;
                    while(!status) {
                        printf("Choose the direction of your ship. (0 is vertical, 1 is horizontal)\n\n");
                        check_numb(&direction, 0, 1);
                        //scanf("%d", &direction);
                        //check_numb(&direction, 0, 1);
                        enterPlace(&row, &col);
                        status = isShipsOverlap(board, row, col, direction, ship_length);
                        for (count = 0; count < ship_length; count++) {
                            if (direction == 0) {
                                board[row + count][col] = '4';
                            } else {
                                board[row][col + count] = '4';
                            }
                        }
                    }

                    break;
                }
                case 2: {
                    printf("Please enter where you would like to place your Submarine (3 spots).\n\n");
                    ship_length = 3;
                    printf("Choose the direction of your ship. (0 is vertical, 1 is horizontal)\n\n");
                    scanf("%d", &direction);
                    enterPlace(&row, &col);
                    for (count = 0; count < ship_length; count++) {
                        if (direction == 0) {
                            board[row + count][col] = '3';
                        } else {
                            board[row][col + count] = '3';
                        }
                    }
                    break;
                }
                case 3: {
                    printf("Please enter where you would like to place your Destroyer (2 spots).\n\n");
                    ship_length = 2;
                    printf("Choose the direction of your ship. (0 is vertical, 1 is horizontal)\n\n");
                    scanf("%d", &direction);
                    enterPlace(&row, &col);
                    for (count = 0; count < ship_length; count++) {
                        if (direction == 0) {
                            board[row + count][col] = '2';
                        } else {
                            board[row][col + count] = '2';
                        }
                    }
                    break;
                }
                case 4: {
                    printf("Please enter where you would like to place your Boat (1 spots).\n\n");
                    ship_length = 1;
                    printf("Choose the direction of your ship. (0 is vertical, 1 is horizontal)\n\n");
                    scanf("%d", &direction);
                    enterPlace(&row, &col);
                    for (count = 0; count < ship_length; count++) {
                        if (direction == 0) {
                            board[row + count][col] = '1';
                        } else {
                            board[row][col + count] = '1';
                        }
                    }
                    break;
                }
                default: {

                }
            }

            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    printf("%c ", board[i][j]);
                }
                printf("\n");
            }
        }
}
