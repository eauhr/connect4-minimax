#include "AI.h"
#include <cstdlib>

AI::AI(char piece) : piece(piece) {}

int AI::chooseMove(const Board& board) const {
    char opponent = (piece == 'X') ? 'O' : 'X';

    // for simulating moves a non-const Board
    Board tempBoard = board;

    for (int c = 0; c < Board::cols; c++) {
        if (!tempBoard.isValidMove(c)) continue;
        tempBoard.dropPiece(c, piece);
        bool win = tempBoard.checkWin(piece);
        tempBoard.undoMove(c);
        if (win) return c;
    }

    // block the opponent's win
    for (int c = 0; c < Board::cols; c++) {
        if (!tempBoard.isValidMove(c)) continue;
        tempBoard.dropPiece(c, opponent);
        bool opponentWins = tempBoard.checkWin(opponent);
        tempBoard.undoMove(c);
        if (opponentWins) return c;
    }

    int valid[Board::cols];
    int count = 0;
    for (int c = 0; c < Board::cols; c++)
        if (tempBoard.isValidMove(c))
            valid[count++] = c;

    return valid[rand() % count];
}