#include "MinimaxStrategy.h"

int MinimaxStrategy::hasAnyStone(const Board& b) const {
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

int MinimaxStrategy::isNearStone(const Board& b, int x, int y) const {
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

int MinimaxStrategy::genCandidates(const Board& b, Move* outMoves, int maxCount) const {
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

static int lineScoreOne(const Board& b, Cell who) {
    int dirs[4][2] = { {1,0}, {0,1}, {1,1}, {1,-1} };
    int n = b.size();
    int total = 0;

    int y = 0;
    while (y < n) {
        int x = 0;
        while (x < n) {
            if (b.at(x, y) == who) {
                int d = 0;
                while (d < 4) {
                    int dx = dirs[d][0];
                    int dy = dirs[d][1];

                    int px = x - dx;
                    int py = y - dy;
                    if (!(b.inside(px, py) && b.at(px, py) == who)) {
                        int len = 0;
                        int cx = x;
                        int cy = y;
                        while (b.inside(cx, cy) && b.at(cx, cy) == who) {
                            len = len + 1;
                            cx = cx + dx;
                            cy = cy + dy;
                        }

                        int open1 = (b.inside(px, py) && b.at(px, py) == CELL_EMPTY) ? 1 : 0;
                        int open2 = (b.inside(cx, cy) && b.at(cx, cy) == CELL_EMPTY) ? 1 : 0;

                        int sc = 0;
                        if (len >= 5) sc = 1000000;
                        else if (len == 4 && open1 && open2) sc = 100000;
                        else if (len == 4 && (open1 || open2)) sc = 20000;
                        else if (len == 3 && open1 && open2) sc = 5000;
                        else if (len == 3 && (open1 || open2)) sc = 800;
                        else if (len == 2 && open1 && open2) sc = 200;
                        else if (len == 2 && (open1 || open2)) sc = 50;
                        else sc = 10;

                        total = total + sc;
                    }

                    d = d + 1;
                }
            }
            x = x + 1;
        }
        y = y + 1;
    }

    return total;
}

int MinimaxStrategy::evaluate(const GameState& s, Cell aiColor) const {
    int my = lineScoreOne(s.board, aiColor);
    int op = lineScoreOne(s.board, Opponent(aiColor));
    return my - op;
}

int MinimaxStrategy::minimax(GameState& s, int depth, int alpha, int beta, Cell aiColor, int maximizing) {
    if (s.historyCount > 0) {
        Move last = s.history[s.historyCount - 1];
        Cell prev = Opponent(s.turn);
        if (s.board.checkWinFrom(last, prev)) {
            if (prev == aiColor) return 900000 - (depth_ - depth);
            return -900000 + (depth_ - depth);
        }
    }

    if (depth <= 0) return evaluate(s, aiColor);
    if (s.board.full()) return evaluate(s, aiColor);

    Move cand[Board::MAX_N * Board::MAX_N];
    int cnt = genCandidates(s.board, cand, Board::MAX_N * Board::MAX_N);

    int best;
    if (maximizing) best = -2147483647;
    else best = 2147483647;

    int i = 0;
    while (i < cnt && alpha < beta) {
        Move m = cand[i];

        if (s.board.isLegal(m)) {
            s.board.place(m, s.turn);
            s.history[s.historyCount] = m;
            s.historyCount = s.historyCount + 1;
            s.turn = Opponent(s.turn);

            int val = minimax(s, depth - 1, alpha, beta, aiColor, maximizing ? 0 : 1);

            s.turn = Opponent(s.turn);
            s.historyCount = s.historyCount - 1;
            s.board.undo(m);

            if (maximizing) {
                if (val > best) best = val;
                if (best > alpha) alpha = best;
            } else {
                if (val < best) best = val;
                if (best < beta) beta = best;
            }
        }

        i = i + 1;
    }

    return best;
}

Move MinimaxStrategy::chooseMove(const GameState& s0, Cell aiColor) {
    GameState s = s0;

    Move cand[Board::MAX_N * Board::MAX_N];
    int cnt = genCandidates(s.board, cand, Board::MAX_N * Board::MAX_N);

    Move bestMove;
    bestMove.x = cand[0].x;
    bestMove.y = cand[0].y;

    int bestScore = -2147483647;

    int i = 0;
    while (i < cnt) {
        Move m = cand[i];

        if (s.board.isLegal(m)) {
            s.board.place(m, s.turn);
            s.history[s.historyCount] = m;
            s.historyCount = s.historyCount + 1;
            s.turn = Opponent(s.turn);

            int score = minimax(s, depth_ - 1, -2147483000, 2147483000, aiColor, 0);

            s.turn = Opponent(s.turn);
            s.historyCount = s.historyCount - 1;
            s.board.undo(m);

            if (score > bestScore) {
                bestScore = score;
                bestMove.x = m.x;
                bestMove.y = m.y;
            }
        }

        i = i + 1;
    }

    return bestMove;
}
