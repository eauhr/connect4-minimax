#include "AI.h"
#include <cstdlib>
#include <climits>
#include <algorithm>

AI::AI(char piece, bool smart, int depth, bool alphaBeta)
    : piece(piece), smart(smart), maxDepth(depth), useAlphaBeta(alphaBeta) {}

int AI::findWinningMove(Board& board, char player) const {
    for (int c = 0; c < Board::cols; c++) {
        if (!board.isValidMove(c))
            continue;

        board.dropPiece(c, player);
        bool win = board.checkWin(player);
        board.undoMove(c);           

        if (win) 
            return c;
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


int AI::evaluate(const Board& board) const {
    char opp = (piece == 'X') ? 'O' : 'X';
    int score = 0;

    for (int r = 0; r < Board::rows; r++)
        if (board.at(r, 3) == piece) score += 3;

    const int dr[] = { 0, 1, 1, -1 };
    const int dc[] = { 1, 0, 1, 1 };

    for (int r = 0; r < Board::rows; r++)
        for (int c = 0; c < Board::cols; c++)
            for (int d = 0; d < 4; d++) {
                int er = r + 3 * dr[d], ec = c + 3 * dc[d];
                if (er < 0 || er >= Board::rows || ec < 0 || ec >= Board::cols)
                    continue;

                int mine = 0, theirs = 0, empty = 0;
                for (int k = 0; k < 4; k++) {
                    char ch = board.at(r + k * dr[d], c + k * dc[d]);
                    if (ch == piece)      mine++;
                    else if (ch == opp)   theirs++;
                    else                  empty++;
                }

                if (mine == 3 && empty == 1)        score += 5;
                else if (mine == 2 && empty == 2)   score += 2;
                if (theirs == 3 && empty == 1)      score -= 4;
            }
    return score;
}

int AI::minimax(Board& board, int remaining, int alpha, int beta, bool maximizing) const {
    nodes++;
    char opp = (piece == 'X') ? 'O' : 'X';

    //search for a win/loss/draw
    if (board.checkWin(piece))
        return 100000 + remaining;
    if (board.checkWin(opp))
        return -100000 - remaining;
    if (board.isFull())
        return 0;
    if (remaining == 0)
        return evaluate(board);

    //the center is with a higher priority
    const int order[] = { 3, 2, 4, 1, 5, 0, 6 };

    if (maximizing) {
        int best = INT_MIN;
        for (int c : order) {
            if (!board.isValidMove(c))
                continue;
            board.dropPiece(c, piece);
            int val = minimax(board, remaining - 1, alpha, beta, false);
            board.undoMove(c);

            best = std::max(best, val);
            if (useAlphaBeta) {
                alpha = std::max(alpha, best);
                if (alpha >= beta) break;
            }
        }
        return best;
    }
    else {
        int best = INT_MAX;
        for (int c : order) {
            if (!board.isValidMove(c)) continue;
            board.dropPiece(c, opp);
            int val = minimax(board, remaining - 1, alpha, beta, true);
            board.undoMove(c);

            best = std::min(best, val);
            if (useAlphaBeta) {
                beta = std::min(beta, best);
                if (alpha >= beta)
                    break;
            }
        }
        return best;
    }
}

int AI::chooseMove(const Board& board) const {
    if (maxDepth > 0) {
        nodes = 0;
        Board work = board;
        const int order[] = { 3, 2, 4, 1, 5, 0, 6 };
        int alpha = INT_MIN, beta = INT_MAX;
        int bestScore = INT_MIN, bestCol = -1;

        for (int c : order) {
            if (!work.isValidMove(c))
                continue;
            work.dropPiece(c, piece);
            int score = minimax(work, maxDepth - 1, alpha, beta, false);
            work.undoMove(c);

            if (score > bestScore)
            {
                bestScore = score;
                bestCol = c;
            }
            if (useAlphaBeta)
                alpha = std::max(alpha, bestScore);
        }
        return bestCol;
    }
        if (!smart)
            return randomMove(board);

        char opponent = (piece == 'X') ? 'O' : 'X';

        // for simulating moves a non-const Board
        Board work = board;

        int move = findWinningMove(work, piece);
        if (move != -1) return move;

        move = findWinningMove(work, opponent);
        if (move != -1) 
            return move;

        return 
            randomMove(work);
    }


   
