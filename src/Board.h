#pragma once

class Board {
public:
    static const int rows = 6;
    static const int cols = 7;
    static const char EMPTY = ' ';
    char at(int r, int c) const;

    Board();              
    bool isValidMove(int col) const;
    bool dropPiece(int col, char piece);
    bool checkWin(char piece) const;
    bool isFull() const;
    void undoMove(int col);
    void print() const;

private:
    char cells[rows][cols];
};