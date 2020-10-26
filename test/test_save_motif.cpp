#include <iostream>
#include <filesystem>

#include "motifs.h"
#include "game_of_life.h"

int main() {
    Motif M;
    GameOfLife* G(new GameOfLife(M.append({{0,4},{0,6},{1,0},{1,2},{1,5},{2,1},{2,5},{3,1},{3,7}}), 10,10));
    G->print();
    std::string line;
    std::cout << std::endl;
    std::cin >> line;
    if(line[0] == 'y') {
        G->save_motif("lievres", 0, 10, 0, 10, "local");
        std::cout << "enregistre" << std::endl;
    }
    delete G;
    return 0;
}