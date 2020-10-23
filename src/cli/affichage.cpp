#include "affichage.h"
#include "motifs.h"
#include "game_of_life.h"

#include <iostream>
#include <cstdlib>

int CliApp::exec() {
    std::system("clear");
    std::cout << "Debut d'une nouvelle simulation." << std::endl;
    std::cout << "Que voulez vous faire ? " << std::endl;
    std::cout << " 1) Lancer une simulation pre-enregistrée" << std::endl;
    std::cout << " 2) Commencer une nouvelle simulation" << std::endl;

    return 0;
}