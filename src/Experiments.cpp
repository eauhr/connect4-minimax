#include "Experiments.h"
#include "Board.h"
#include "AI.h"
#include <iostream>

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