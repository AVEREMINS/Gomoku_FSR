# Gomoku_FSR
Project_Gomoku_3sem_FSR
Код делал без STL, STL умных строк и контейнеров (они идут сам знаешь куда), break и auto.
GUI делаем через raylib (это C-библиотека; в C++ спокойно используется и не требует STL).

Структура проекта:
gomoku/
  src/
    main.cpp
    core/
      Error.h
      Types.h
      Board.h
      Board.cpp
      GameState.h
      Player.h
      Game.h
      Game.cpp
    ai/
      IStrategy.h
      RandomStrategy.h
      RandomStrategy.cpp
      GreedyStrategy.h
      GreedyStrategy.cpp
      MinimaxStrategy.h
      MinimaxStrategy.cpp
    ui/
      Presenter.h
      RaylibPresenter.h
      RaylibPresenter.cpp


Как устроен GUI по ТЗ:
По ТЗ “общение с пользователем/ОС только через Presenter” — значит:
1)Game, Board, AI не знают про окно/мышь
2)они вызывают только методы интерфейса Presenter
3)конкретная реализация GUI — RaylibPresenter


Код (все файлы):
src/core/Error.h
src/core/Types.h
src/core/Board.h
src/core/Board.cpp
src/core/GameState.h
src/ui/Presenter.h
src/core/Player.h
src/ai/IStrategy.h
src/ai/RandomStrategy.h
src/ai/RandomStrategy.cpp
src/ai/GreedyStrategy.h
src/ai/GreedyStrategy.cpp
src/ai/MinimaxStrategy.h
src/ai/MinimaxStrategy.cpp
src/core/Game.h
src/core/Game.cpp
src/ui/RaylibPresenter.h
src/ui/RaylibPresenter.cpp
src/main.cpp



Компиляция “одной командой” (пример):
g++ -std=c++17 -O2 \
src/main.cpp \
src/core/Board.cpp src/core/Game.cpp \
src/ai/RandomStrategy.cpp src/ai/GreedyStrategy.cpp src/ai/MinimaxStrategy.cpp \
src/ui/RaylibPresenter.cpp \
-o gomoku \
-lraylib -lGL -lm -lpthread -ldl -lrt -lX11
