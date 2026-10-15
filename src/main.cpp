#include "Board.h"
#include <iostream>
using namespace std;

int main() {
    Board b;
    char player = 'X';

    while (true) {
        b.print();
        cout << "Player " << player << ", column (1-7): ";

        int col;
        cin >> col;

        if (!b.dropPiece(col - 1, player)) {
            cout << "Invalid move, try again.\n";
            continue;
        }

        if (b.checkWin(player)) {
            b.print();
            cout << "Wins " << player << "!\n";
            break;
        }

        player = (player == 'X') ? 'O' : 'X';
    }
}