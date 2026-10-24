#include "AI.h"
#include "Random.h"
#include <cstdlib>
#include <climits>
#include <algorithm>

namespace {
    constexpr int WIN_SCORE = 100000;

    constexpr int CENTER_BONUS = 3;   // own piece in the centre column
    constexpr int THREE_BONUS = 5;   // window: 3 own + 1 empty
    constexpr int TWO_BONUS = 2;   // window: 2 own + 2 empty
    constexpr int OPP_THREE_PENALTY = 4;   // window: 3 opponent + 1 empty

    constexpr int kOrder[Board::cols] = { 3, 2, 4, 1, 5, 0, 6 };

    char opponentOf(char p) { return (p == 'X') ? 'O' : 'X'; }
}


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

    return valid[randInt(count)];
}


int AI::evaluate(const Board& board) const {
    char opp = (piece == 'X') ? 'O' : 'X';
    int score = 0;

    for (int r = 0; r < Board::rows; r++)
        if(board.at(r, Board::cols / 2) == piece)
        score += CENTER_BONUS;

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
                    if (ch == piece)     
                        mine++;
                    else if (ch == opp)   
                        theirs++;
                    else                  
                        empty++;
                }

                if (mine == 3 && empty == 1)        
                    score += THREE_BONUS;
                else if (mine == 2 && empty == 2)  
                    score += TWO_BONUS;
                if (theirs == 3 && empty == 1)     
                    score -= OPP_THREE_PENALTY;
            }
    return score;
}

int AI::minimax(Board& board, int remaining, int alpha, int beta, bool maximizing) const {
    nodes++;
    char opp = opponentOf(piece);

    //search for a win/loss/draw
    if (board.checkWin(piece))
        return WIN_SCORE + remaining;
    if (board.checkWin(opp))
        return -WIN_SCORE - remaining;
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
                if (alpha >= beta) 
                    break;
            }
        }
        return best;
    }
    else {
        int best = INT_MAX;
        for (int c : order) {
            if (!board.isValidMove(c)) 
                continue;
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

int AI::searchMove(const Board& board) const {
    nodes = 0;
    Board work = board;
    int alpha = INT_MIN, beta = INT_MAX;
    int bestScore = INT_MIN, bestCol = -1;

    for (int c : kOrder) {
        if (!work.isValidMove(c))
            continue;
        work.dropPiece(c, piece);
        int score = minimax(work, maxDepth - 1, alpha, beta, false);
        work.undoMove(c);

        if (score > bestScore) {
            bestScore = score;
            bestCol = c;
        }
        if (useAlphaBeta)
            alpha = std::max(alpha, bestScore);
    }
    lastScore = bestScore;
    return bestCol;
}

int AI::chooseMove(const Board& board) const {
    if (maxDepth > 0)   return searchMove(board);

    if (!smart)
        return randomMove(board);

    const char opponent = opponentOf(piece);
    Board work = board;

    int move = findWinningMove(work, piece);
    if (move != -1)
        return move;

    move = findWinningMove(work, opponent);
    if (move != -1)
        return move;

    return randomMove(work);
}

   
