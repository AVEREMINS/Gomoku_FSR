#pragma once
#include "../ui/Presenter.h"
#include "../ai/IStrategy.h"

class Player {
public:
    virtual ~Player() {}
    virtual Move getMove(const GameState& s, Presenter& p) = 0;
};

class HumanPlayer : public Player {
public:
    Move getMove(const GameState& s, Presenter& p) {
        return p.pollHumanMove(s);
    }
};

class AiPlayer : public Player {
public:
    AiPlayer(Cell aiColor, IStrategy* strat) : ai_(aiColor), strat_(strat) {}
    ~AiPlayer() { if (strat_ != 0) { delete strat_; strat_ = 0; } }

    Move getMove(const GameState& s, Presenter&) {
        return strat_->chooseMove(s, ai_);
    }

private:
    Cell ai_;
    IStrategy* strat_;
};
