#pragma once
#include "IStrategy.h"

class MinimaxStrategy : public IStrategy {
public:
    explicit MinimaxStrategy(int depth) : depth_(depth) {}
    Move chooseMove(const GameState& s, Cell aiColor);

private:
    int depth_;

    int hasAnyStone(const Board& b) const;
    int isNearStone(const Board& b, int x, int y) const;
    int genCandidates(const Board& b, Move* outMoves, int maxCount) const;

    int evaluate(const GameState& s, Cell aiColor) const;
    int minimax(GameState& s, int depth, int alpha, int beta, Cell aiColor, int maximizing);
};
