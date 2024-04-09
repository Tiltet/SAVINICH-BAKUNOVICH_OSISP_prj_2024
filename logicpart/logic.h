//
// Created by nikitabakunovich on 20.03.24.
//

#ifndef SAVINICH_BAKUNOVICH_OSISP_PRJ_2024_LOGIC_H
#define SAVINICH_BAKUNOVICH_OSISP_PRJ_2024_LOGIC_H

 //void game();
void place_ships(int board[10][10]);
void logic();
bool isShipsOverlap(int shipBoard[10][10], int startX, int startY, int dir, int length);
void enterPlace(int *x, int *y);
void check_numb(int *a, int start, int end);
//void place_ships_auto(int board[10][10]);
//int isValidPlacement(int board[10][10], int row, int col, int direction, int size);
void sendShotCoordinates(int client_socket);
#endif //SAVINICH_BAKUNOVICH_OSISP_PRJ_2024_LOGIC_H
