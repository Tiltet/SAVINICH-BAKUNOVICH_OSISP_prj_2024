//
// Created by nikitabakunovich on 20.03.24.
//
#include "../server/server.h"
#include "logic.h"

void game() {
    int ground[9][9] = {0};
    for (int i = 0; i < 9; i++) {
        printf("---");
        for (int j = 0; j < 9; j++) {
            printf("%d", ground[i][j]);
        }
        printf("\n");
    }
}