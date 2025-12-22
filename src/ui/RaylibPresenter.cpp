#include "RaylibPresenter.h"
#include <string.h>
#include <stdio.h>

RaylibPresenter::RaylibPresenter()
: winW_(1000), winH_(820), margin_(70), cellPx_(0), msgFrames_(0) {
    msg_[0] = 0;
    InitWindow(winW_, winH_, "Gomoku (no STL / no break / no auto)");
    SetTargetFPS(60);
}

RaylibPresenter::~RaylibPresenter() {
    CloseWindow();
}

int RaylibPresenter::shouldQuit() const {
    if (WindowShouldClose()) return 1;
    if (IsKeyPressed(KEY_ESCAPE)) return 1;
    return 0;
}

int RaylibPresenter::shouldMenu() const {
    if (IsKeyPressed(KEY_ENTER)) return 1;
    return 0;
}

void RaylibPresenter::showMessage(const char* msg) {
    if (msg == 0) { msg_[0] = 0; msgFrames_ = 0; return; }
    snprintf(msg_, sizeof(msg_), "%s", msg);
    msgFrames_ = 180;
}

void RaylibPresenter::computeLayout(int n) {
    int usableW = winW_ - 2 * margin_;
    int usableH = winH_ - 2 * margin_;
    int usable = (usableW < usableH) ? usableW : usableH;
    cellPx_ = usable / (n - 1);
}

Vector2 RaylibPresenter::cellCenter(int x, int y) const {
    Vector2 v;
    v.x = (float)(margin_ + x * cellPx_);
    v.y = (float)(margin_ + y * cellPx_);
    return v;
}

int RaylibPresenter::mouseToCell(const GameState& s, Move* out) const {
    Vector2 m = GetMousePosition();
    int n = s.board.size();

    float fx = (m.x - (float)margin_) / (float)cellPx_;
    float fy = (m.y - (float)margin_) / (float)cellPx_;

    int x = (int)(fx + 0.5f);
    int y = (int)(fy + 0.5f);

    if (x < 0 || y < 0 || x >= n || y >= n) return 0;

    Vector2 c = cellCenter(x, y);
    float dx = m.x - c.x;
    float dy = m.y - c.y;
    float r = (float)cellPx_ * 0.45f;
    if (dx*dx + dy*dy > r*r) return 0;

    out->x = x;
    out->y = y;
    return 1;
}

void RaylibPresenter::selectSettings(Cell* humanColor, int* difficulty) {
    *humanColor = CELL_BLACK;
    *difficulty = 3;

    int done = 0;
    while (!shouldQuit() && !done) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("GOMOKU", 420, 70, 40, DARKGRAY);

        DrawText("Color:", 200, 160, 24, GRAY);
        DrawText("[B] Black (first move)", 240, 200, 22, DARKGRAY);
        DrawText("[W] White", 240, 230, 22, DARKGRAY);

        DrawText("AI Difficulty:", 200, 300, 24, GRAY);
        DrawText("[1] Easy   (Random)", 240, 340, 22, DARKGRAY);
        DrawText("[2] Medium (Greedy)", 240, 370, 22, DARKGRAY);
        DrawText("[3] Hard   (Minimax)", 240, 400, 22, DARKGRAY);

        char buf1[64];
        char buf2[64];

        if (*humanColor == CELL_BLACK) snprintf(buf1, sizeof(buf1), "Selected: Black");
        else snprintf(buf1, sizeof(buf1), "Selected: White");

        snprintf(buf2, sizeof(buf2), "Difficulty: %d", *difficulty);

        DrawText(buf1, 240, 470, 22, MAROON);
        DrawText(buf2, 240, 500, 22, MAROON);

        DrawText("ENTER - start, ESC - quit", 240, 570, 22, DARKGREEN);

        EndDrawing();

        if (IsKeyPressed(KEY_B)) *humanColor = CELL_BLACK;
        if (IsKeyPressed(KEY_W)) *humanColor = CELL_WHITE;

        if (IsKeyPressed(KEY_ONE)) *difficulty = 1;
        if (IsKeyPressed(KEY_TWO)) *difficulty = 2;
        if (IsKeyPressed(KEY_THREE)) *difficulty = 3;

        if (IsKeyPressed(KEY_ENTER)) done = 1;
    }

    if (shouldQuit()) throw PresenterError("Exit from menu.");
}

void RaylibPresenter::draw(const GameState& s, const char* status) {
    computeLayout(s.board.size());

    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (status != 0) DrawText(status, 20, 15, 20, DARKGRAY);

    const char* turnText = (s.turn == CELL_BLACK) ? "Turn: BLACK" : "Turn: WHITE";
    DrawText(turnText, 20, 42, 20, DARKGRAY);

    if (msgFrames_ > 0) {
        DrawText(msg_, 20, 68, 20, MAROON);
        msgFrames_ = msgFrames_ - 1;
    }

    int n = s.board.size();

    int i = 0;
    while (i < n) {
        Vector2 a = cellCenter(0, i);
        Vector2 b = cellCenter(n - 1, i);
        DrawLineV(a, b, GRAY);

        Vector2 c = cellCenter(i, 0);
        Vector2 d = cellCenter(i, n - 1);
        DrawLineV(c, d, GRAY);

        i = i + 1;
    }

    int y = 0;
    while (y < n) {
        int x = 0;
        while (x < n) {
            Cell c = s.board.at(x, y);
            if (c != CELL_EMPTY) {
                Vector2 p = cellCenter(x, y);
                float r = (float)cellPx_ * 0.38f;
                if (c == CELL_BLACK) {
                    DrawCircleV(p, r, BLACK);
                } else {
                    DrawCircleV(p, r, RAYWHITE);
                    DrawCircleLines((int)p.x, (int)p.y, r, BLACK);
                }
            }
            x = x + 1;
        }
        y = y + 1;
    }

    Move hover;
    if (mouseToCell(s, &hover)) {
        Vector2 p2 = cellCenter(hover.x, hover.y);
        DrawCircleLines((int)p2.x, (int)p2.y, (float)cellPx_ * 0.40f, DARKGREEN);
    }

    EndDrawing();
}

Move RaylibPresenter::pollHumanMove(const GameState& s) {
    Move m;
    m.x = 0; m.y = 0;

    int got = 0;
    while (!shouldQuit() && !got) {
        draw(s, "Gomoku: five in a row. LMB - move. ESC - quit.");

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Move tmp;
            if (mouseToCell(s, &tmp)) {
                m.x = tmp.x;
                m.y = tmp.y;
                got = 1;
            }
        }
    }

    if (shouldQuit()) throw PresenterError("Exit while waiting for human move.");
    return m;
}
