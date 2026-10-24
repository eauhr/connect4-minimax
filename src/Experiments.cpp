#include "Experiments.h"
#include "Board.h"
#include "Random.h"
#include "AI.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <vector>

namespace {

    char otherPlayer(char p) { return (p == 'X') ? 'O' : 'X'; }

    struct Position {
        Board board;
        char toMove;
    };

    Position randomPosition(int plies) {
        while (true) {
            Position pos{ Board(), 'X' };
            bool ok = true;
            for (int i = 0; i < plies && ok; i++) {
                int col;
                do {
                    col = randInt(Board::cols);
                } while (!pos.board.isValidMove(col));

                pos.board.dropPiece(col, pos.toMove);

                if (pos.board.checkWin(pos.toMove) || pos.board.isFull())
                    ok = false;
                else
                    pos.toMove = otherPlayer(pos.toMove);
            }
            if (ok)
                return pos;
        }
    }

static char playMatch(const AI& first, const AI& second, int randomPlies = 0) {
    Board b;
    char player = 'X';

    for (int ply = 0;; ply++) {
        int col;
        if (ply < randomPlies) {
            do {
                col = randInt(Board::cols);
            } while (!b.isValidMove(col));
        }
        else {
            const AI& current = (player == 'X') ? first : second;
            col = current.chooseMove(b);
        }

        b.dropPiece(col, player);

        if (b.checkWin(player)) 
            return player;
        if (b.isFull())         
            return 'D';

        player = otherPlayer(player);
    }
}

struct PairStats {
    int winsA = 0, winsB = 0, draws = 0, firstPlayerWins = 0;
};

PairStats playPair(int games, int depthA, int depthB, int randomPlies) {
    PairStats s;

    for (int g = 0; g < games; g++) {
        bool aFirst = (g % 2 == 0);

        AI a(aFirst ? 'X' : 'O', true, depthA, true);
        AI b(aFirst ? 'O' : 'X', true, depthB, true);

        char winner = aFirst ? playMatch(a, b, randomPlies)
            : playMatch(b, a, randomPlies);

        if (winner == 'D')                  
            s.draws++;
        else if ((winner == 'X') == aFirst) 
            s.winsA++;
        else                                
            s.winsB++;

        if (winner == 'X') 
            s.firstPlayerWins++;
    }
    return s;
}
}

static char playMatchWithOpening(const AI& first, const AI& second, int randomPlies) {
    Board b;
    char player = 'X';
    int ply = 0;

    while (true) {
        int col;
        if (ply < randomPlies) {
            do {
                col = rand() % Board::cols;
            } while (!b.isValidMove(col));
        }
        else {
            const AI& current = (player == 'X') ? first : second;
            col = current.chooseMove(b);
        }

        b.dropPiece(col, player);

        if (b.checkWin(player))
            return player;
        if (b.isFull())       
            return 'D';

        player = (player == 'X') ? 'O' : 'X';
        ply++;
    }
}


void runDepthTournament(int games, int randomPlies) {
    seedRng(kDefaultSeed);
    const int pairs[][2] = { {1, 3}, {2, 4}, {3, 5}, {4, 4} };

    std::ofstream csv("depth_tournament.csv");
    csv << "depthA,depthB,games,winsA,winsB,draws,firstPlayerWins\n";

    std::cout << "\n" << games << " games per pair, "
        << randomPlies << " random opening moves\n"
        << "A vs B   winsA  winsB  draws  first player wins\n";

    for (const auto& p : pairs) {
        PairStats s = playPair(games, p[0], p[1], randomPlies);

        std::cout << p[0] << " vs " << p[1] << "     "
            << s.winsA << "      " << s.winsB << "      "
            << s.draws << "      " << s.firstPlayerWins << "\n";

        csv << p[0] << "," << p[1] << "," << games << ","
            << s.winsA << "," << s.winsB << "," << s.draws << ","
            << s.firstPlayerWins << "\n";
    }
    std::cout << "\nSaved to depth_tournament.csv\n";

}

void runRandomVsSmart(int games) {
    seedRng(kDefaultSeed);
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

void runMinimaxVsSmart(int games, int depth) {
    seedRng(kDefaultSeed);
    int minimaxWins = 0, smartWins = 0, draws = 0;

    for (int g = 0; g < games; g++) {
        bool minimaxFirst = (g % 2 == 0);        
        AI mm(minimaxFirst ? 'X' : 'O', true, depth);
        AI sm(minimaxFirst ? 'O' : 'X', true, 0); 

        char winner = minimaxFirst ? playMatch(mm, sm)
            : playMatch(sm, mm);

        if (winner == 'D')                        
            draws++;
        else if ((winner == 'X') == minimaxFirst) 
            minimaxWins++;
        else                                      
            smartWins++;
    }

    std::cout << "\nMinimax (depth " << depth << ") vs win/block, "
        << games << " games:\n" << "Minimax wins:   " << minimaxWins << "\n" << "Win/block wins: " << smartWins << "\n" << "Draws: " << draws << "\n";

    std::cout << "Saved to minimax_vs_smart.csv\n";
}

void runDepthComparison(int maxDepth, int repeats) {
    seedRng(kDefaultSeed);

    const int pliesList[] = { 0, 8, 16, 24 };
    std::vector<Position> positions;
    for (int plies : pliesList)
        positions.push_back(randomPosition(plies));

    std::ofstream csv("depth_comparison.csv");
    csv << "position_plies,depth,mode,nodes,ms_mean,ms_sd,move\n";

    std::cout << "\nTiming: mean +- sd over " << repeats << " runs (after 1 warm-up)\n"
        << "plies depth mode        nodes        ms_mean   ms_sd   move\n"
        << std::fixed << std::setprecision(3);


        for (size_t i = 0; i < positions.size(); i++) {
            const Position& pos = positions[i];

            for (int d = 1; d <= maxDepth; d++) {
                for (int ab = 0; ab <= 1; ab++) {
                    AI ai(pos.toMove, true, d, ab == 1);

                    int move = ai.chooseMove(pos.board);
                    long long nodes = ai.getNodes();

                    std::vector<double> times;
                    for (int r = 0; r < repeats; r++) {
                        auto t0 = std::chrono::steady_clock::now();
                        move = ai.chooseMove(pos.board);
                        auto t1 = std::chrono::steady_clock::now();
                        times.push_back(std::chrono::duration<double, std::milli>(t1 - t0).count());
                    }

                    double mean = 0;
                    for (double t : times) mean += t;
                    mean /= times.size();
                    double var = 0;
                    for (double t : times) var += (t - mean) * (t - mean);
                    double sd = (times.size() > 1) ? std::sqrt(var / (times.size() - 1)) : 0.0;

                    const char* mode = ab ? "alphabeta" : "minimax  ";
                    std::cout << std::setw(4) << pliesList[i] << "  " << d << "     " << mode
                        << std::setw(10) << nodes << "  "
                        << std::setw(10) << mean << "  " << std::setw(7) << sd
                        << "   " << move + 1 << "\n";
                    csv << pliesList[i] << "," << d << "," << (ab ? "alphabeta" : "minimax") << ","
                        << nodes << "," << mean << "," << sd << "," << move + 1 << "\n";
                }
            }
        }
    std::cout << "\nSaved to depth_comparison.csv\n";
}
void runEquivalenceTest(int positions, int maxDepth) {
    seedRng(kDefaultSeed);

    std::vector<long long> nodesMm(maxDepth + 1, 0), nodesAb(maxDepth + 1, 0);
    std::vector<int> mismatches(maxDepth + 1, 0);

    for (int p = 0; p < positions; p++) {
        Position pos = randomPosition(randInt(31));

        for (int d = 1; d <= maxDepth; d++) {
            AI mm(pos.toMove, true, d, false);
            AI ab(pos.toMove, true, d, true);

            int moveMm = mm.chooseMove(pos.board);
            int moveAb = ab.chooseMove(pos.board);

            if (moveMm != moveAb || mm.getLastScore() != ab.getLastScore())
                mismatches[d]++;

            nodesMm[d] += mm.getNodes();
            nodesAb[d] += ab.getNodes();
        }
    }

    std::ofstream csv("equivalence_test.csv");
    csv << "depth,positions,mismatches,nodesMinimax,nodesAlphaBeta,reduction\n";

    std::cout << "\nEquivalence test: " << positions << " random positions, seed "
        << kDefaultSeed << "\n"
        << "depth  mismatches  nodes(minimax)  nodes(alpha-beta)  reduction\n"
        << std::fixed << std::setprecision(1);

    int total = 0;
    for (int d = 1; d <= maxDepth; d++) {
        double red = static_cast<double>(nodesMm[d]) / static_cast<double>(nodesAb[d]);
        std::cout << std::setw(5) << d << std::setw(12) << mismatches[d]
            << std::setw(16) << nodesMm[d] << std::setw(19) << nodesAb[d]
            << std::setw(9) << red << "x\n";
        csv << d << "," << positions << "," << mismatches[d] << ","
            << nodesMm[d] << "," << nodesAb[d] << "," << red << "\n";
        total += mismatches[d];
    }
    std::cout << "\nTotal mismatches: " << total << "\nSaved to equivalence_test.csv\n";
}
