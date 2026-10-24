# Connect 4 – Minimax

[![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus&logoColor=white)](https://en.cppreference.com/w/cpp/17)
[![CMake](https://img.shields.io/badge/CMake-3.15%2B-064F8C?logo=cmake&logoColor=white)](https://cmake.org/)
[![Visual Studio](https://img.shields.io/badge/Visual%20Studio-2022-5C2D91?logo=visualstudio&logoColor=white)](https://visualstudio.microsoft.com/)

A console Connect 4 game written in C++17. Play against a friend or against a computer opponent powered by **Minimax with alpha-beta pruning**.

## Features

- Classic 6×7 board with win and draw detection
- Human vs Human and Human vs Computer modes
- Three computer opponents:
  - **Random** – plays any valid column
  - **Win or Block** – takes a winning move, blocks the opponent's, otherwise plays randomly
  - **Minimax** – depth-limited search with alpha-beta pruning
- Built-in experiments that compare search depths and log results to CSV

## Getting Started

You'll need a C++17 compiler and CMake 3.15+.

```bash
git clone https://github.com/eauhr/connect4-minimax.git
cd connect4-minimax
cmake -S . -B build
cmake --build build
```

Run it:

```bash
./build/connect4            # Linux / macOS
.\build\Debug\connect4.exe  # Windows
```

## How to Play

Pick a mode from the menu, then enter a column number (1–7) on your turn. The first player to connect four pieces horizontally, vertically or diagonally wins.

## How the AI Works

The Minimax opponent looks several moves ahead, assuming you always reply with your best move. At the depth limit it scores the board with a simple heuristic that rewards center control and open three-in-a-row windows, and penalizes your threats. Alpha-beta pruning skips branches that can't change the outcome, so it returns the same move much faster.

Search depth sets the difficulty: deeper means stronger but slower.

### Pruning Benchmark

Nodes visited for the first move from an empty board:

| Depth | Minimax | Alpha-beta | Speedup |
|------:|--------:|-----------:|--------:|
| 3 | 399 | 81 | 4.9× |
| 5 | 19,607 | 640 | 30.6× |
| 7 | 960,792 | 6,845 | 140.4× |

At depth 7 the move takes about 1,174 ms without pruning and about 7 ms with it (Debug build, single run).

## Project Structure

```
src/
├── Board.h / .cpp         # Board, rules, win/draw checks
├── AI.h / .cpp            # Opponents, evaluation, Minimax
├── Game.h / .cpp          # Menu and game loop
├── Experiments.h / .cpp   # Benchmarks and matches, CSV output
└── main.cpp               # Entry point
CMakeLists.txt
```


## References

- Russell & Norvig, *Artificial Intelligence: A Modern Approach*
- Knuth & Moore, *An Analysis of Alpha-Beta Pruning* (1975)
- Allis, *A Knowledge-based Approach of Connect-Four* (1988)
