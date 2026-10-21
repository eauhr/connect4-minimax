#include "Experiments.h"
#include "Board.h"
#include "AI.h"
#include <iostream>
#include <chrono>
#include <fstream>

static char playMatch(const AI& first, const AI& second) {
    Board b;
    char player = 'X';

    while (true) {
        const AI& current = (player == 'X') ? first : second;
        int col = current.chooseMove(b);
        b.dropPiece(col, player);

        if (b.checkWin(player)) 
            return player;
        if (b.isFull())         
            return 'D';

        player = (player == 'X') ? 'O' : 'X';
    }
}

void runRandomVsSmart(int games) {
    int smartWins = 0, randomWins = 0, draws = 0;

    for (int g = 0; g < games; g++) {

        bool smartFirst = (g % 2 == 0);
        AI smart(smartFirst ? 'X' : 'O', true);
        AI rnd(smartFirst ? 'O' : 'X', false);

        char winner = smartFirst ? playMatch(smart, rnd)
            : playMatch(rnd, smart);

        if (winner == 'D')                          
            draws++;
        else if ((winner == 'X') == smartFirst)     
            smartWins++;
        else                                        
            randomWins++;
    }

    std::cout << "\nResults after " << games << " games:\n"
        << "Win/block AI wins: " << smartWins << "\n" << "Random AI wins: " << randomWins << "\n" << "Draws: " << draws << "\n";
}

void runMinimaxVsSmart(int games, int depth) {
    int minimaxWins = 0, smartWins = 0, draws = 0;

    for (int g = 0; g < games; g++) {
        bool minimaxFirst = (g % 2 == 0);        
        AI mm(minimaxFirst ? 'X' : 'O', true, depth);
        AI sm(minimaxFirst ? 'O' : 'X', true, 0); 

        char winner = minimaxFirst ? playMatch(mm, sm)
            : playMatch(sm, mm);

        if (winner == 'D')                        
            draws++;
        else if ((winner == 'X') == minimaxFirst) 
            minimaxWins++;
        else                                      
            smartWins++;
    }

    std::cout << "\nMinimax (depth " << depth << ") vs win/block, "
        << games << " games:\n" << "Minimax wins:   " << minimaxWins << "\n" << "Win/block wins: " << smartWins << "\n" << "Draws: " << draws << "\n";
}

void runDepthComparison(int maxDepth) {
    Board b;
    std::ofstream csv("depth_comparison.csv");
    csv << "depth,mode,nodes,ms,move\n";
    std::cout << "\n depth  mode nodes ms move\n";

    for (int d = 1; d <= maxDepth; d++) {
        for (int ab = 0; ab <= 1; ab++) {
            AI ai('X', true, d, ab == 1);

            auto t0 = std::chrono::steady_clock::now();
            int move = ai.chooseMove(b);
            auto t1 = std::chrono::steady_clock::now();

            double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
            const char* mode = ab ? "alphabeta" : "minimax";

            std::cout << d << "      " << mode << "   " << ai.getNodes()
                << "   " << ms << "   " << move + 1 << "\n";
            csv << d << "," << mode << "," << ai.getNodes() << ","
                << ms << "," << move + 1 << "\n";
        }
    }
    std::cout << "\nSaved to depth_comparison.csv\n";
}