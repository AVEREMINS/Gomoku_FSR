#pragma once
#include "IStrategy.h"

class GreedyStrategy : public IStrategy {
public:
    Move chooseMove(const GameState& s, Cell aiColor);

private:
    int hasAnyStone(const Board& b) const;
    int isNearStone(const Board& b, int x, int y) const;
    int genCandidates(const Board& b, Move* outMoves, int maxCount) const;

    int scoreMove(const GameState& s, const Move& m, Cell who) const;
};
