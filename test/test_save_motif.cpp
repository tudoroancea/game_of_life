#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <thread>

#include "Motif.hpp"
#include "GameOfLife.hpp"

int main() {
    Motif M;
    M.append({{0,4},{0,6},{1,0},{1,2},{1,5},{2,1},{2,5},{3,1},{3,7}});
    GameOfLifeView G(0,30,0,30);
    G.addMotif(M);
    G.print();
	G.saveMotif("lievres", local);

    std::cout << std::endl;

    G.wipe();
    G.addMotif(Motif("lievres", local));
    G.print();

    return 0;
}