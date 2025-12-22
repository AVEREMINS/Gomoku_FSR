GOMOKU/
  gomoku/
    src/...
  third_party/
    raylib/
      include/raylib.h
      lib/...
      (bin/raylib.dll)  // если есть
  assets/
    fonts/DejaVuSans.ttf   // если используешь кириллицу через шрифт


То есть raylib лежит внутри проекта, и пути будут относительные, не C:\Users\....

Что нужно на новом компе (минимум)

Компилятор g++ (MinGW-w64) или MSVC (cl).

Твоя папка проекта + папка third_party/raylib нужной разрядности (win64 под x64 g++).

JSON-файлы VS Code не нужны для сборки вообще. Они только для подсветки/IntelliSense.

Сборка одной командой (без json, без cmake)

Открываешь терминал в корне GOMOKU (там где папка gomoku), и выполняешь:

Вариант под g++ (MinGW)

g++ -std=c++17 -O2 -I "third_party/raylib/include" gomoku/src/main.cpp gomoku/src/core/Board.cpp gomoku/src/core/Game.cpp gomoku/src/ai/RandomStrategy.cpp gomoku/src/ai/GreedyStrategy.cpp gomoku/src/ai/MinimaxStrategy.cpp gomoku/src/ui/RaylibPresenter.cpp -o gomoku.exe -L "third_party/raylib/lib" -lraylib -lopengl32 -lgdi32 -lwinmm


.\gomoku.exe

Если при запуске скажет “не найден raylib.dll”

Это бывает, если в твоей сборке raylib динамическая.

Тогда просто сделай:

найди third_party/raylib/bin/raylib.dll

скопируй рядом с gomoku.exe (в корень проекта)

Команда:

copy third_party\raylib\bin\raylib.dll .


