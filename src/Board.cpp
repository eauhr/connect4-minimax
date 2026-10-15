#include "Board.h"
#include <iostream>
using namespace std;

Board::Board() {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cells[r][c] = ' ';
        }
    }
}

bool Board::isValidMove(int col) const {
    if (col < 0 || col >= cols) return false;
    return cells[0][col] == ' ';
}

bool Board::dropPiece(int col, char piece) {
    if (!isValidMove(col)) return false;
    for (int r = rows - 1; r >= 0; --r) {
        if (cells[r][col] == ' ') {
            cells[r][col] = piece;
            return true;
        }
    }
    return false;
}

bool Board::checkWin(char piece) const {
    // horizontal check
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c <= cols - 4; ++c) {
            if (cells[r][c] == piece && cells[r][c + 1] == piece &&
                cells[r][c + 2] == piece && cells[r][c + 3] == piece) {
                return true;
            }
        }
    }
    // vertical check
    for (int c = 0; c < cols; ++c) {
        for (int r = 0; r <= rows - 4; ++r) {
            if (cells[r][c] == piece && cells[r + 1][c] == piece &&
                cells[r + 2][c] == piece && cells[r + 3][c] == piece) {
                return true;
            }
        }
    }
    // diagonal bottom left to top right
    for (int r = 3; r < rows; ++r) {
        for (int c = 0; c <= cols - 4; ++c) {
            if (cells[r][c] == piece && cells[r - 1][c + 1] == piece &&
                cells[r - 2][c + 2] == piece && cells[r - 3][c + 3] == piece) {
                return true;
            }
        }
    }
    // diagonal top left to bottom right
    for (int r = 0; r <= rows - 4; ++r) {
        for (int c = 0; c <= cols - 4; ++c) {
            if (cells[r][c] == piece && cells[r + 1][c + 1] == piece &&
                cells[r + 2][c + 2] == piece && cells[r + 3][c + 3] == piece) {
                return true;
            }
        }
    }
    return false;
}

void Board::print() const {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            std::cout << "|" << cells[r][c];
        }
        std::cout << "|\n";
    }
    for (int c = 1; c <= cols; ++c) {
        std::cout << " " << c;
    }
    std::cout << "\n";
}


