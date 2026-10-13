#include "Board.h"
#include <iostream>

Board::Board() {
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            cells[r][c] = '.';
}

void Board::print() const {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++)
            std::cout << cells[r][c] << ' ';
        std::cout << '\n';
    }
    for (int c = 1; c <= cols; c++)
        std::cout << c << ' ';
    std::cout << '\n';
}

bool Board::isValidMove(int col) const {
    return col >= 0 && col < cols && cells[0][col] == '.';
}
bool Board::dropPiece(int col, char piece) {
    if (!isValidMove(col))
        return false;

    for (int r = rows - 1; r >= 0; r--) {
        if (cells[r][col] == '.') {
            cells[r][col] = piece;
            return true;
        }
    }
    return false;
}