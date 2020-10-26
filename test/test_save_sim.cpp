#include <iostream>
#include <iomanip>
#include "game_of_life.h"
#include "motifs.h"
#include "rapidcsv.h"
#include <filesystem>
#include "affichage.h"

using namespace std;

int main() {
    GameOfLife G(Motif("pentominoR", "local"), 20, 20);
    //G.print();
    cout << boolalpha << G.save_sim("pentominoR1", 5, "local") << std::endl;
    return 0;
}