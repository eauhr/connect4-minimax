#pragma once
#include "Board.h"

class AI {
public:
    AI(char piece, bool smart = true);
    int chooseMove(const Board& board) const; 

private:
    char piece;
    int findWinningMove(Board& board, char player) const;
    int randomMove(const Board& board) const;
    bool smart;
};