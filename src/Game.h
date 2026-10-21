#pragma once

class Game {
public:
    void run();                    

private:
    void play(bool vsComputer, int depth = 0);
    int readColumn(char player); 
};