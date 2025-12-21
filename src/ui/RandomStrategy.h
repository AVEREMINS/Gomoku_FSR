#pragma once
#include "IStrategy.h"

class RandomStrategy : public IStrategy {
public:
    Move chooseMove(const GameState& s, Cell aiColor);
};
