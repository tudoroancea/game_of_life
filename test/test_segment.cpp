#include "game_of_life.h"
#include "motifs.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <cstdlib>
using namespace std;

int main() {
    GameOfLife G(51,51);

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
    //Motif M(segment(4,0,4,4));
    //M.append(segment(4,6,4,9));

    //G.add_motif(M);
    //std::system("clear");
    //G.print();
    //G.suppr_motif(M);
    //M.rotate();
    //G.add_motif(M);
    //std::this_thread::sleep_for(3s);
    //std::system("clear");
    //G.print();
    return 0;
}