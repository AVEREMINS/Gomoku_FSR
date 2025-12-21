#include <time.h>
#include <stdlib.h>
#include "core/Game.h"
#include "ui/RaylibPresenter.h"
#include "core/Error.h"

int main() {
    srand((unsigned)time(0));

    try {
        RaylibPresenter presenter;
        Game game(15);
        game.run(presenter);
        return 0;
    } catch (const Error&) {
        return 1;
    } catch (...) {
        return 2;
    }
}
