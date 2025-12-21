#pragma once
#include "Types.h"

class Board {
public:
    enum { MAX_N = 15 };

    Board(int n = 15);

    int size() const;

    int inside(int x, int y) const;
    Cell at(int x, int y) const;

    int isLegal(const Move& m) const;
    void place(const Move& m, Cell who);
    void undo(const Move& m);

    int full() const;
    int checkWinFrom(const Move& last, Cell who) const;

private:
    int n_;
    Cell g_[MAX_N][MAX_N];

    int countDir(const Move& from, Cell who, int dx, int dy) const;
};
