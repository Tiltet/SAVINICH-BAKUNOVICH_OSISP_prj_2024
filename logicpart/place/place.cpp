//
// Created by nikitabakunovich on 22.03.24.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 10
#define SHIPS_COUNT 10

typedef struct {
    int size;
    int count;
} Ship;

void initializeBoard(int board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = 0;
        }
    }
}

int isValidPlacement2(int board[BOARD_SIZE][BOARD_SIZE], int row, int col, int direction, int size) {
    if (direction == 0) {
        // Проверка, что корабль не выходит за пределы поля по горизонтали
        if (col + size > BOARD_SIZE) {
            return 0;
        }

        // Проверка наличия других кораблей в указанных клетках и их окрестностях
        for (int j = 0; j < size; j++) {
            if (board[row][col + j] == 1) {
                return 0;
            }

            for (int i = row - 1; i <= row + 1; i++) {
                for (int k = col - 1; k < col + j + 2; k++) {
                    if (i >= 0 && i < BOARD_SIZE && k >= 0 && k < BOARD_SIZE) {
                        if (board[i][k] == 1) {
                            return 0;
                        }
                    }
                }
            }
        }
    } else {
        // Проверка, что корабль не выходит за пределы поля по вертикали
        if (row + size > BOARD_SIZE) {
            return 0;
        }

        // Проверка наличия других кораблей в указанных клетках и их окрестностях
        for (int j = 0; j < size; j++) {
            if (board[row + j][col] == 1) {
                return 0;
            }

            for (int i = row - 1; i < row + j + 2; i++) {
                for (int k = col - 1; k <= col + 1; k++) {
                    if (i >= 0 && i < BOARD_SIZE && k >= 0 && k < BOARD_SIZE) {
                        if (board[i][k] == 1) {
                            return 0;
                        }
                    }
                }
            }
        }
    }

    return 1;
}

void placeShip(int board[BOARD_SIZE][BOARD_SIZE], int row, int col, int direction, int size) {
    if (direction == 0) {
        for (int j = 0; j < size; j++) {
            board[row][col + j] = 1;
        }
    } else {
        for (int j = 0; j < size; j++) {
            board[row + j][col] = 1;
        }
    }
}

void placeShips(int board[BOARD_SIZE][BOARD_SIZE], Ship ships[]) {
    srand(time(NULL));

    for (int i = 0; i < SHIPS_COUNT; i++) {
        int shipSize = ships[i].size;
        int shipCount = ships[i].count;

        for (int j = 0; j < shipCount; j++) {
            int row, col, direction;

            do {
                row = rand() % BOARD_SIZE;
                col = rand() % BOARD_SIZE;
                direction = rand() % 2; // 0 - горизонтальное размещение, 1 - вертикальное размещение
            } while (!isValidPlacement2(board, row, col, direction, shipSize));

            placeShip(board, row, col, direction, shipSize);
        }
    }
}

void displayBoard(int board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

void place(int board[BOARD_SIZE][BOARD_SIZE]) {
    //int board[BOARD_SIZE][BOARD_SIZE];
    initializeBoard(board);

    Ship ships[SHIPS_COUNT] = {
            {4, 1},
            {3, 2},
            {2, 3},
            {1, 4}
    };

    placeShips(board, ships);
    //displayBoard(board);
}