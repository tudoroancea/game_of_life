#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <thread>

#include "motifs.h"
#include "game_of_life.h"

int main() {
    Motif M;
    M.append({{0,4},{0,6},{1,0},{1,2},{1,5},{2,1},{2,5},{3,1},{3,7}});
    GameOfLifeView G(0,30,0,30);
    G.add_motif(M);
    G.print();
	G.save_motif("lievres", local, false);

    std::cout << std::endl;

    G.wipe();
    G.add_motif(Motif("lievres", local));
    G.print();

    return 0;
}