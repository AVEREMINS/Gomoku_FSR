#pragma once
#include "../core/GameState.h"

class Presenter {
public:
    virtual ~Presenter() {}

    virtual void selectSettings(Cell* humanColor, int* difficulty) = 0;
    virtual void draw(const GameState& s, const char* status) = 0;
    virtual Move pollHumanMove(const GameState& s) = 0;

    virtual void showMessage(const char* msg) = 0;
    virtual int shouldQuit() const = 0;
};
