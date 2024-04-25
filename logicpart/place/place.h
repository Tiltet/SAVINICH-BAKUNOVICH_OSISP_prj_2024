//
// Created by nikitabakunovich on 22.03.24.
//

#ifndef COURSE_WORK_PLACE_H
#define COURSE_WORK_PLACE_H

typedef struct {
    int size;
    int count;
    int begin;
    bool isHorizontal;
} Ship;

Ship* place(int board[10][10]);
void displayBoard(int board[10][10]);


#endif //COURSE_WORK_PLACE_H
