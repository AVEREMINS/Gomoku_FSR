#pragma once
#include "../core/GameState.h"

class IStrategy {
public:
    virtual ~IStrategy() {}
    virtual Move chooseMove(const GameState& s, Cell aiColor) = 0;
};
