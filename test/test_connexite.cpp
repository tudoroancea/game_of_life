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
    GameOfLifeView G;
    G.add_motif(Motif("planeur", presaved));
    //G.add_motif(Motif("planeur", "presaved").translate(5,7));
    //G.add_motif(Motif("planeur", "presaved").translate(12,6));
    //G.add_motif(Motif("planeur", "presaved").translate(15,15));
    G.print();
    std::this_thread::sleep_for(3s);
    CLEAR()
    //std::cout << "calculs des composants connexes : " << std::endl;
    //std::vector<Motif> comp(composants_connexes(G));
    //std::cout << " Il y a " << comp.size() << " composantes connexes" << std::endl;
    //for (auto const& m : comp) {
    //    for (liste::const_iterator it(m.cbegin()); it != m.cend(); ++it) std::cout << it->first << ", " << it->second << std::endl;
    //    std::cout << std::endl;
    //}
    std::vector<size_t> labels(G.sparseCLL());
    std::cout << " Il y a " << labels.back()+1 << " composantes connexes" << std::endl;
    return 0;
}
