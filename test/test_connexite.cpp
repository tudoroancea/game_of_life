#include "game_of_life.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <vector>
#include <iterator>
using namespace std::chrono;

int main() {
    GameOfLifeView G(0,20,0,20);
    G.add_motif(Motif("lievres", local));
    std::vector<Motif> tab(G.find_CC_ite());
    ////std::cout << tab.size() << std::endl;
    ////tab[1].translate(-50,-50);
    //for (liste::iterator it(tab[2].begin()); it != tab[2].end(); ++it) std::cout << it->first << "," << it->second << "|";
    ////std::cout << std::endl;

    GameOfLifeView g0(0,20,0,20);
    g0.add_motif(tab[0].translate(-50,-50));
    GameOfLifeView g1(0,20,0,20);
    g1.add_motif(tab[1].translate(-50,-50));
    GameOfLifeView g2(0,20,0,20);
    g2.add_motif(tab[2].translate(-50,-50));
    g0.print();
    g1.print();
    g2.print();


    //G.add_cell(10,10);
    //std::vector<Motif> tab(G.find_CC_ite());
    //std::cout << std::endl << tab.size() << std::endl;
    //for (liste::iterator it(tab[0].begin()); it != tab[0].end(); ++it) std::cout << it->first << "," << it->second << "|";
    //std::cout << std::endl;

    return 0;
}
