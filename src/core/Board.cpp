#include "Board.h"

Board::Board(int n) : n_(n) {
    int y = 0;
    while (y < MAX_N) {
        int x = 0;
        while (x < MAX_N) {
            g_[y][x] = CELL_EMPTY;
            x = x + 1;
        }
        y = y + 1;
    }
}

int Board::size() const { return n_; }

int Board::inside(int x, int y) const {
    if (x < 0) return 0;
    if (y < 0) return 0;
    if (x >= n_) return 0;
    if (y >= n_) return 0;
    return 1;
}

Cell Board::at(int x, int y) const {
    if (!inside(x, y)) return CELL_EMPTY;
    return g_[y][x];
}

int Board::isLegal(const Move& m) const {
    if (!inside(m.x, m.y)) return 0;
    if (at(m.x, m.y) != CELL_EMPTY) return 0;
    return 1;
}

void Board::place(const Move& m, Cell who) {
    g_[m.y][m.x] = who;
}

void Board::undo(const Move& m) {
    g_[m.y][m.x] = CELL_EMPTY;
}

int Board::full() const {
    int y = 0;
    while (y < n_) {
        int x = 0;
        while (x < n_) {
            if (g_[y][x] == CELL_EMPTY) return 0;
            x = x + 1;
        }
        y = y + 1;
    }
    return 1;
}

int Board::countDir(const Move& from, Cell who, int dx, int dy) const {
    int x = from.x + dx;
    int y = from.y + dy;
    int cnt = 0;

    while (inside(x, y) && at(x, y) == who) {
        cnt = cnt + 1;
        x = x + dx;
        y = y + dy;
    }
    return cnt;
}

int Board::checkWinFrom(const Move& last, Cell who) const {
    int dirs[4][2] = { {1,0}, {0,1}, {1,1}, {1,-1} };

    int i = 0;
    while (i < 4) {
        int dx = dirs[i][0];
        int dy = dirs[i][1];
        int a = countDir(last, who, dx, dy);
        int b = countDir(last, who, -dx, -dy);
        if (1 + a + b >= 5) return 1;
        i = i + 1;
    }
    return 0;
}
