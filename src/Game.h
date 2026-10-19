#pragma once

class Game {
public:
    void run();                    

private:
    void play(bool vsComputer);    
    int readColumn(char player); 
};