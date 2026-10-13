#pragma once

class Board {
public:
    static const int rows = 6;
    static const int cols = 7;

    Board();              
    bool isValidMove(int col) const;
    bool dropPiece(int col, char piece);
    void print() const;

private:
    char cells[rows][cols];
};