#include "Board.h"
#include "AI.h"
#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    Game game;
    game.run();

    return 0;
}