#include "Game.h"
#include "Board.h"
#include "AI.h"
#include "Experiments.h"
#include "Random.h"
#include <iostream>
#include <random>

namespace {
    constexpr int kHumanVsMinimaxDepth = 4;
    constexpr int kInputClosed = -100;
}

void Game::run() {
    while (true) {
        std::cout << "\n=== Connect 4 ===\n"
            << "1. Human vs Human\n"
            << "2. Human vs Computer\n"
            << "3. Human vs Minimax(depth " << kHumanVsMinimaxDepth << ")\n"
            << "4. Experiment: random vs win/block AI\n"
			<< "5. Experiment: minimax(depth 4) vs win/block AI\n"
			<< "6. Experiment: Minimax vs Alpha-beta by depth\n"
			<< "7. Experiment: depth vs depth\n"
			<< "8. Experiment: Minimax = Alpha-beta equivalence test\n"
            << "0. Exit\n"
            << "Choice: ";
        int choice;
        std::cin >> choice;

        if (!std::cin) {    
            if (std::cin.eof())
                return;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Please enter a number.\n";
            continue;
        }

        if (choice == 1)      
            play(false);
        else if (choice == 2) 
            play(true);
        else if (choice == 3) 
            play(true, kHumanVsMinimaxDepth);
        else if (choice == 4) 
            runRandomVsSmart(100);
        else if (choice == 5) 
            runMinimaxVsSmart(50, 4);
        else if (choice == 6)
            runDepthComparison(7, 10);
        else if (choice == 7)
        {
            runDepthTournament(200, 4);
        }
        else if (choice == 8)
        {
            runEquivalenceTest(1000, 5);
		}
        else if (choice == 0) 
            return;
        
        else                  
            std::cout << "Invalid choice.\n";
    }
}

int Game::readColumn(char player) {
    std::cout << "Player " << player << ", column (1-7): ";
    int col;
    std::cin >> col;

    if (!std::cin) {
        if (std::cin.eof())
            return kInputClosed;
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return -1;                      
    }
    return col - 1;
}

void Game::play(bool vsComputer, int depth) {
    seedRng(std::random_device{}());

    Board b;
    AI ai('O', true, depth);
    char player = 'X';

    while (true) {
        b.print();

        int col;
        if (vsComputer && player == 'O') {
            col = ai.chooseMove(b);
            std::cout << "The Computer plays column " << col + 1 << "\n";
        }
        else {
            col = readColumn(player);
            if (col == kInputClosed) {
                std::cout << "\nInput closed.\n";
                return;
            }
        }

        if (!b.dropPiece(col, player)) {
            std::cout << "Invalid move, try again.\n";
            continue;
        }

        if (b.checkWin(player)) {
            b.print();
            std::cout << "Wins " << player << "!\n";
            return;
        }

        if (b.isFull()) {
            b.print();
            std::cout << "Draw!\n";
            return;
        }

        player = (player == 'X') ? 'O' : 'X';
    }
}