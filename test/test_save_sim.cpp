#include <iostream>
#include "game_of_life.h"
#include "motifs.h"

using namespace std;

int main() {
    GameOfLife G(Motif("pentominoR", "local"), 50, 50);
    G.print();
    G.life("pentomino1", 10);
    return 0;
}