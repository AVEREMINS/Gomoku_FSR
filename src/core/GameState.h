#pragma once
#include "Board.h"

struct GameState {
    Board board;
    Cell turn;

    Move history[Board::MAX_N * Board::MAX_N];
    int historyCount;

    GameState(int n = 15) : board(n), turn(CELL_BLACK), historyCount(0) {}
};
