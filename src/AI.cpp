#include "AI.h"
#include <cstdlib>

AI::AI(char piece, bool smart) : piece(piece), smart(smart) {}

int AI::findWinningMove(Board& board, char player) const {
    for (int c = 0; c < Board::cols; c++) {
        if (!board.isValidMove(c)) continue;

        board.dropPiece(c, player);
        bool win = board.checkWin(player);
        board.undoMove(c);           
        if (win) return c;
    }
    return -1;
}

int AI::randomMove(const Board& board) const {
    int valid[Board::cols];
    int count = 0;
    for (int c = 0; c < Board::cols; c++)
        if (board.isValidMove(c))
            valid[count++] = c;

    return valid[rand() % count];
}

int AI::chooseMove(const Board& board) const {
    if (!smart)
        return randomMove(board);

    char opponent = (piece == 'X') ? 'O' : 'X';

    // for simulating moves a non-const Board
    Board tempBoard = board;

    int move = findWinningMove(tempBoard, piece);
    if (move != -1) return move;

    move = findWinningMove(tempBoard, opponent);
    if (move != -1) return move;

    return randomMove(tempBoard);
}