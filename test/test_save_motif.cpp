#include <iostream>
#include "motifs.h"
#include "game_of_life.h"

int main() {
    Motif M;
    GameOfLife* G(new GameOfLife(M.append({{0,1},{1,1},{1,2},{2,0},{2,1}}), 10,10));
    G->print();
    G->save_motif("pentominoR", 0, 10, 0, 10);
    delete G;

    //M = Motif();
    //G  = new GameOfLife(M.append({{}}),10,10);
    //G->print();
    //delete G;
    return 0;
}