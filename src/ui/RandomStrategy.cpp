#include "RandomStrategy.h"
#include <stdlib.h>

Move RandomStrategy::chooseMove(const GameState& s, Cell) {
    Move empty[Board::MAX_N * Board::MAX_N];
    int cnt = 0;

    int n = s.board.size();
    int y = 0;
    while (y < n) {
        int x = 0;
        while (x < n) {
            if (s.board.at(x, y) == CELL_EMPTY) {
                empty[cnt].x = x;
                empty[cnt].y = y;
                cnt = cnt + 1;
            }
            x = x + 1;
        }
        y = y + 1;
    }

    if (cnt <= 0) { Move m; m.x = 0; m.y = 0; return m; }

    int k = rand() % cnt;
    return empty[k];
}
