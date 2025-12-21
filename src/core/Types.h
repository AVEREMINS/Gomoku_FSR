#pragma once
#include <stdint.h>

enum Cell {
    CELL_EMPTY = 0,
    CELL_BLACK = 1,
    CELL_WHITE = 2
};

static inline Cell Opponent(Cell c) {
    if (c == CELL_BLACK) return CELL_WHITE;
    if (c == CELL_WHITE) return CELL_BLACK;
    return CELL_EMPTY;
}

struct Move {
    int x;
    int y;
};
