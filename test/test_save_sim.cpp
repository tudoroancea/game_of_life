#include <iostream>
#include <iomanip>
#include "GameOfLife.hpp"
#include "Motif.hpp"
#include "rapidcsv.h"
#include <filesystem>
#include "affichage.h"
#include "termcolor.hpp"
#include <thread>
#include <chrono>
using namespace std;

int main() {
    // Enregistrement de simulation
    //GameOfLifeView G;
    //G.add_motif(Motif("lievres", local).translate(20,20));
    //cout << boolalpha << G.save_sim("lievres", 5, presaved) << std::endl;

    // Ouverture de simulation
    Simulation sim;
    sim.load("lievres", presaved);
    sim.resize(190,206,190,206);
    do {
        sim.print();
        this_thread::sleep_for(std::chrono::milliseconds(500));
        std::system("clear");

    } while (sim.evolve());
    return 0;
}