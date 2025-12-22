#include "Game.h"
#include "../ai/RandomStrategy.h"
#include "../ai/GreedyStrategy.h"
#include "../ai/MinimaxStrategy.h"
#include <stdio.h>
#include <string.h>

Game::Game(int boardSize) : s_(boardSize), pBlack_(0), pWhite_(0) {}

Game::~Game() {
    if (pBlack_ != 0) { delete pBlack_; pBlack_ = 0; }
    if (pWhite_ != 0) { delete pWhite_; pWhite_ = 0; }
}

void Game::setStatus(char* dst, int cap, const char* text) {
    if (cap <= 0) return;
    dst[0] = 0;
    if (text == 0) return;
    snprintf(dst, (size_t)cap, "%s", text);
}

void Game::setupPlayers(Presenter& presenter) {
    Cell humanColor = CELL_BLACK;
    int difficulty = 3;

    presenter.selectSettings(&humanColor, &difficulty);

    if (pBlack_ != 0) { delete pBlack_; pBlack_ = 0; }
    if (pWhite_ != 0) { delete pWhite_; pWhite_ = 0; }

    IStrategy* sBlack = 0;
    IStrategy* sWhite = 0;

    if (difficulty == 1) { sBlack = new RandomStrategy(); sWhite = new RandomStrategy(); }
    if (difficulty == 2) { sBlack = new GreedyStrategy(); sWhite = new GreedyStrategy(); }
    if (difficulty >= 3) { sBlack = new MinimaxStrategy(3); sWhite = new MinimaxStrategy(3); }

    if (humanColor == CELL_BLACK) pBlack_ = new HumanPlayer();
    else pBlack_ = new AiPlayer(CELL_BLACK, sBlack);

    if (humanColor == CELL_WHITE) pWhite_ = new HumanPlayer();
    else pWhite_ = new AiPlayer(CELL_WHITE, sWhite);

    if (humanColor == CELL_BLACK) { if (sBlack != 0) { delete sBlack; sBlack = 0; } }
    if (humanColor == CELL_WHITE) { if (sWhite != 0) { delete sWhite; sWhite = 0; } }
}

void Game::applyMoveOrThrow(const Move& m) {
    if (!s_.board.isLegal(m)) {
        throw InvalidMoveError("Invalid move: out of bounds or cell is occupied.");
    }
    s_.board.place(m, s_.turn);
    s_.history[s_.historyCount] = m;
    s_.historyCount = s_.historyCount + 1;
}

void Game::run(Presenter& presenter) {
    char status[160];

    int quitAll = 0;

    while (!presenter.shouldQuit() && !quitAll) {
        // reset game state for a new match
        {
            int n = s_.board.size();
            s_ = GameState(n);
        }

        // show menu and create players
        try {
            setupPlayers(presenter);
        } catch (const PresenterError&) {
            quitAll = 1;
        }

        if (quitAll) {
            // do nothing, will exit
        } else {
            setStatus(status, 160, "Gomoku: five in a row. LMB - move. ESC - quit.");

            int gameOver = 0;
            while (!presenter.shouldQuit() && !gameOver) {
                presenter.draw(s_, status);

                Player* cur = (s_.turn == CELL_BLACK) ? pBlack_ : pWhite_;

                try {
                    Move m = cur->getMove(s_, presenter);
                    applyMoveOrThrow(m);

                    if (s_.board.checkWinFrom(m, s_.turn)) {
                        if (s_.turn == CELL_BLACK) {
                            setStatus(status, 160, "BLACK wins! ENTER - main menu, ESC - quit.");
                        } else {
                            setStatus(status, 160, "WHITE wins! ENTER - main menu, ESC - quit.");
                        }
                        gameOver = 1;
                    } else if (s_.board.full()) {
                        setStatus(status, 160, "DRAW! ENTER - main menu, ESC - quit.");
                        gameOver = 1;
                    } else {
                        s_.turn = Opponent(s_.turn);
                    }
                } catch (const InvalidMoveError& e) {
                    presenter.showMessage(e.what());
                }
            }

            // end screen: wait ENTER -> menu, ESC -> quit
            if (!presenter.shouldQuit()) {
                int backToMenu = 0;
                while (!presenter.shouldQuit() && !backToMenu) {
                    presenter.draw(s_, status);
                    if (presenter.shouldMenu()) backToMenu = 1;
                }
            }
        }
    }
}
