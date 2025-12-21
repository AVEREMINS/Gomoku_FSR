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
1)src/core/Error.h

2)src/core/Types.h

3)src/core/Board.h

4)src/core/Board.cpp

5)src/core/GameState.h

6)src/ui/Presenter.h

7)src/core/Player.h

8)src/ai/IStrategy.h

9)src/ai/RandomStrategy.h

10)src/ai/RandomStrategy.cpp

11)src/ai/GreedyStrategy.h

12)src/ai/GreedyStrategy.cpp

13)src/ai/MinimaxStrategy.h

14)src/ai/MinimaxStrategy.cpp

15)src/core/Game.h

16)src/core/Game.cpp

17)src/ui/RaylibPresenter.h

18)src/ui/RaylibPresenter.cpp

19)src/main.cpp



Компиляция “одной командой” (пример):
g++ -std=c++17 -O2 \
src/main.cpp \
src/core/Board.cpp src/core/Game.cpp \
src/ai/RandomStrategy.cpp src/ai/GreedyStrategy.cpp src/ai/MinimaxStrategy.cpp \
src/ui/RaylibPresenter.cpp \
-o gomoku \
-lraylib -lGL -lm -lpthread -ldl -lrt -lX11
