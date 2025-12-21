#pragma once
#include "../ui/Presenter.h"
#include "GameState.h"
#include "Player.h"
#include "Error.h"

class Game {
public:
    explicit Game(int boardSize);
    ~Game();

    void run(Presenter& presenter);

private:
    GameState s_;
    Player* pBlack_;
    Player* pWhite_;

    void setupPlayers(Presenter& presenter);
    void applyMoveOrThrow(const Move& m);

    void setStatus(char* dst, int cap, const char* text);
};
