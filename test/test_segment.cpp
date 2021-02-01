#include "game_of_life.h"
#include "motifs.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <cstdlib>
using namespace std;

int main() {
    GameOfLifeView G;

    Motif M(segment(25,25,0,0));
    M.append(segment(25,25,0,13));
    M.append(segment(25,25,0,25));
    M.append(segment(25,25,0,38));
    M.append(segment(25,25,0,50));
    M.append(segment(25,25,13,50));
    M.append(segment(25,25,25,50));
    M.append(segment(25,25,38,50));
    M.append(segment(25,25,50,50));
    M.append(segment(25,25,50,38));
    M.append(segment(25,25,50,25));
    M.append(segment(25,25,50,13));
    M.append(segment(25,25,50,0));
    M.append(segment(25,25,38,0));
    M.append(segment(25,25,25,0));
    M.append(segment(25,25,13,0));
    G.add_motif(M);
    G.print();
    return 0;
}