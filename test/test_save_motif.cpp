#include <iostream>
#include "motifs.h"
#include "game_of_life.h"

int main() {
    GameOfLife* G(new GameOfLife(50,50));
    liste l(segment(3,26,35,6));
    for (auto const& el : l) G->add_cell(el);
    G->print();
    G->save_motif("ligne-oblique-2");
    delete G;

    std::cout << "hey" << std::endl;
    G = new GameOfLife(Motif("ligne-oblique-2"), 50, 50);
    std::cout << "hey" << std::endl;
    G->print();
    std::cout << "hey" << std::endl;
    return 0;
}