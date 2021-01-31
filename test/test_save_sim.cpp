#include <iostream>
#include <iomanip>
#include "game_of_life.h"
#include "motifs.h"
#include "rapidcsv.h"
#include <filesystem>
#include "affichage.h"

using namespace std;

int main() {
    // Enregistrement de simulation
    //GameOfLifeView G;
    //G.add_motif(Motif("lievres", local).translate(20,20));
    //cout << boolalpha << G.save_sim("lievres", 5, local) << std::endl;

    // Ouverture de simulation
    Simulation sim;
    sim.load("lievres", local);
    sim.resize(176,226,176,226);
    sim.print();
    return 0;
}