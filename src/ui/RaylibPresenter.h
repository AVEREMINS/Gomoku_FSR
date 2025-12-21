#pragma once
#include "Presenter.h"
#include "../core/Error.h"

#include "raylib.h"

class RaylibPresenter : public Presenter {
public:
    RaylibPresenter();
    ~RaylibPresenter();

    void selectSettings(Cell* humanColor, int* difficulty);
    void draw(const GameState& s, const char* status);
    Move pollHumanMove(const GameState& s);

    void showMessage(const char* msg);
    int shouldQuit() const;

private:
    int winW_;
    int winH_;
    int margin_;
    int cellPx_;

    char msg_[256];
    int msgFrames_;

    void computeLayout(int n);
    Vector2 cellCenter(int x, int y) const;
    int mouseToCell(const GameState& s, Move* out) const;
};
