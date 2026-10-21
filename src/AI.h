#pragma once
#include "Board.h"

class AI {
public:
    AI(char piece, bool smart = true, int depth = 0, bool alphaBeta = true);
    int chooseMove(const Board& board) const;
    long long getNodes() const { return nodes; }

private:
    char piece;

    bool smart;
    int maxDepth;

    bool useAlphaBeta;
    mutable long long nodes = 0;

    int findWinningMove(Board& board, char player) const;
    int randomMove(const Board& board) const;

    int minimax(Board& board, int remaining, int alpha, int beta, bool maximizing) const;
    int evaluate(const Board& board) const;
};