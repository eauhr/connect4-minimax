#pragma once
#include "Board.h"

class AI {
public:
    AI(char piece);
    int chooseMove(const Board& board) const; 

private:
    char piece;
};