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
    GameOfLifeView G(0,10,0,10);
    G.add_motif(Motif("planeur", presaved));
    G.add_motif(Motif("planeur", presaved).translate(0,5));
    G.add_motif(Motif("planeur", presaved).translate(5,5));
    G.add_motif(Motif("planeur", presaved).translate(5,0));
    G.print();
    //std::cout << " Il y a " << G.nbr_CC_2() << " composantes connexes" << std::endl;
    #ifdef TEST_DEF
        std::cout << "hey" << std::endl;
    #endif

    return 0;
}
