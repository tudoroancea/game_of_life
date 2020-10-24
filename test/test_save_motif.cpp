#include <iostream>
#include "motifs.h"
#include "game_of_life.h"

int main() {
    Motif M;
    M.append({{0,1},{1,1},{1,2},{2,0},{2,1}});
    GameOfLife* G(new GameOfLife(M, 10,10));
    G->print();
    G->save_motif("pentominoR", 0, 10, 0, 10);
    delete G;
    G = new GameOfLife(Motif("pentominoR", "local"),10,10);
    G->print();
    delete G;
    return 0;
}