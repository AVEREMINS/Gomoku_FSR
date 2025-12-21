#include "GreedyStrategy.h"

int GreedyStrategy::hasAnyStone(const Board& b) const {
    int n = b.size();
    int y = 0;
    while (y < n) {
        int x = 0;
        while (x < n) {
            if (b.at(x, y) != CELL_EMPTY) return 1;
            x = x + 1;
        }
        y = y + 1;
    }
    return 0;
}

int GreedyStrategy::isNearStone(const Board& b, int x, int y) const {
    int dy = -2;
    while (dy <= 2) {
        int dx = -2;
        while (dx <= 2) {
            int nx = x + dx;
            int ny = y + dy;
            if (b.inside(nx, ny)) {
                if (b.at(nx, ny) != CELL_EMPTY) return 1;
            }
            dx = dx + 1;
        }
        dy = dy + 1;
    }
    return 0;
}

int GreedyStrategy::genCandidates(const Board& b, Move* outMoves, int maxCount) const {
    int n = b.size();
    int cnt = 0;

    if (!hasAnyStone(b)) {
        outMoves[0].x = n / 2;
        outMoves[0].y = n / 2;
        return 1;
    }

    int y = 0;
    while (y < n) {
        int x = 0;
        while (x < n) {
            if (b.at(x, y) == CELL_EMPTY) {
                if (isNearStone(b, x, y)) {
                    if (cnt < maxCount) {
                        outMoves[cnt].x = x;
                        outMoves[cnt].y = y;
                        cnt = cnt + 1;
                    }
                }
            }
            x = x + 1;
        }
        y = y + 1;
    }

    if (cnt <= 0) {
        outMoves[0].x = n / 2;
        outMoves[0].y = n / 2;
        return 1;
    }
    return cnt;
}

int GreedyStrategy::scoreMove(const GameState& s, const Move& m, Cell who) const {
    GameState tmp = s;
    tmp.board.place(m, who);

    if (tmp.board.checkWinFrom(m, who)) return 1000000;

    int dirs[4][2] = { {1,0}, {0,1}, {1,1}, {1,-1} };
    int total = 0;

    int i = 0;
    while (i < 4) {
        int dx = dirs[i][0];
        int dy = dirs[i][1];

        int a = 0;
        int x1 = m.x + dx;
        int y1 = m.y + dy;
        while (tmp.board.inside(x1, y1) && tmp.board.at(x1, y1) == who) {
            a = a + 1;
            x1 = x1 + dx;
            y1 = y1 + dy;
        }

        int b = 0;
        int x2 = m.x - dx;
        int y2 = m.y - dy;
        while (tmp.board.inside(x2, y2) && tmp.board.at(x2, y2) == who) {
            b = b + 1;
            x2 = x2 - dx;
            y2 = y2 - dy;
        }

        int len = 1 + a + b;
        if (len >= 4) total = total + 10000;
        else if (len == 3) total = total + 1000;
        else if (len == 2) total = total + 100;
        else total = total + 10;

        i = i + 1;
    }

    return total;
}

Move GreedyStrategy::chooseMove(const GameState& s, Cell aiColor) {
    Move cand[Board::MAX_N * Board::MAX_N];
    int cnt = genCandidates(s.board, cand, Board::MAX_N * Board::MAX_N);

    Cell op = Opponent(aiColor);

    Move best;
    best.x = cand[0].x;
    best.y = cand[0].y;
    int bestScore = -2147483647;

    int i = 0;
    while (i < cnt) {
        Move m = cand[i];

        int scSelf = scoreMove(s, m, aiColor);
        int scOp   = scoreMove(s, m, op);

        int sc = scSelf;
        if (scOp > sc) sc = scOp + 1;  // приоритет блокировки, если опасно

        if (sc > bestScore) {
            bestScore = sc;
            best.x = m.x;
            best.y = m.y;
        }

        i = i + 1;
    }

    return best;
}
