#include "Game.h"
#include "Board.h"
#include "AI.h"
#include <iostream>

void Game::run() {
    while (true) {
        std::cout << "\n=== Connect 4 ===\n"
            << "1. Human vs Human\n"
            << "2. Human vs Computer\n"
            << "0. Exit\n"
            << "Choice: ";
        int choice;
        std::cin >> choice;

        if (!std::cin) {                
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Please enter a number.\n";
            continue;
        }

        if (choice == 1)      play(false);
        else if (choice == 2) play(true);
        else if (choice == 0) return;
        else                  std::cout << "Invalid choice.\n";
    }
}

int Game::readColumn(char player) {
    std::cout << "Player " << player << ", column (1-7): ";
    int col;
    std::cin >> col;

    if (!std::cin) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return -1;                      
    }
    return col - 1;
}

void Game::play(bool vsComputer) {
    Board b;
    AI ai('O');
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