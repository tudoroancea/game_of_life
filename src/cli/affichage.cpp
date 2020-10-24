#include "affichage.h"
#include "motifs.h"
#include "game_of_life.h"
#include "rapidcsv.h"

#include <vector>
#include <iostream>
#include <cstdlib> // pour std::sytem()
#include <stdexcept> // pou std::invalid_argument pour stoi()
#include <thread> // pour std::this_thread::sleep_for();
#include <chrono> // pour les s, ms

#ifdef _WIN32
    #define CLEAR() (std::stystem("cls");)
#endif
#ifndef _WIN32
    #define CLEAR() std::system("clear");
#endif

int CliApp::exec() {
    CLEAR()
    std::cout << "Debut d'une nouvelle simulation." << std::endl;
    std::cout << "Que voulez vous faire ? " << std::endl;
    std::cout << " 1) Simulation pre-enregistrée" << std::endl;
    std::cout << " 2) Nouvelle simulation" << std::endl;
	int input(0);
	std::string line("");
	while (input < 1 and input > 2) {
        std::cin >> input;
        if (input < 1 and input > 2) std::cout << " Choix invalide, ressaisissez vous.";
    }
    CLEAR()
    if (input == 1) {
        // Quelle simulation on veut afficher ?
        std::vector<std::string> loc_sims(existing_local_sims()), saved_sims(existing_presaved_sims());
        if (loc_sims.empty() && saved_sims.empty()) {
            std::cerr << "Aucune simulation enregistrée" << std::endl;
            return 0;
        }
        std::cout << "Les simulations disponibles sont : " << std::endl;
        std::cout << "  Simulations locales :" << std::endl;
        for (size_t i(0); i < loc_sims.size() ; ++i) std::cout << "   " << i+1 << ") " << loc_sims[i] << std::endl;
        std::cout << "   Simulations generales :" << std::endl;
        for (size_t i(0); i < saved_sims.size() ; ++i) std::cout << "   " << i+1+loc_sims.size() << ") " << saved_sims[i] << std::endl;

        input = 0;
        while (input < 1 and input > loc_sims.size()+saved_sims.size()) {
            std::cin >> input;
            if (input < 1 and input > loc_sims.size()+saved_sims.size()) std::cout << " Choix invalide, ressaisissez vous.";
        }
        // On cree le chemin correspondant au dossier
        std::filesystem::path chemin("../../data");
        std::string nom_sim("");
        if (input <= loc_sims.size()) {
            nom_sim = loc_sims[input-1];
            chemin.append("local/sims"+nom_sim);
        } else {
            nom_sim = saved_sims[input-1-loc_sims.size()];
            chemin.append("presaved/sims"+nom_sim);
        }

        // Demande de la vitesse de la simulation


        // Affichage de la simulation
        CLEAR()
        std::cout << "Chargement du fichier de configuration de la simulation" << std::endl;
        if (std::filesystem::exists(chemin.append(nom_sim+"-info.csv"))) {
            rapidcsv::Document info(chemin);
            Motif M(chemin.replace_filename(std::filesystem::path(nom_sim+"0.csv")));
            GameOfLife G(M, info.GetCell<unsigned int>(0,0), info.GetCell<unsigned int>(1,0));
            std::cout << "Grille au depart : " << std::endl;
            G.print();
            std::cout << "Appuyer sur n'importe quelle touche pour commencer."; std::cin >> line;

            unsigned int nbr_gen(info.GetCell<unsigned int>(0,1));
            for (size_t i(0); i < nbr_gen ; ++i) {
                CLEAR()
                if (std::filesystem::exists(chemin.replace_filename(std::filesystem::path(nom_sim+std::to_string(i+1)+".csv")))) {
                    Motif tpr(chemin);
                    G.wipe();
                    G.add_motif(tpr);
                    std::cout << "Generation : " << i+1 << std::endl;
                    G.print();
                } else {
                    std::cerr << " Le fichier de l'etape numero " << i+1 << " n'existe pas. Arret de la simulation." << std::endl;
                    return -1;
                }
                using namespace std;
                std::this_thread::sleep_for(100ms);
            }
            return 0;
        } else {
            std::cerr << " Chargement impossible. Le fichier " << nom_sim << "-info.csv n'existe pas" << std::endl;
            return -1;
        }
    } else {
        std::cout << "Revenez plus tard. Bitch." << std::endl;
        return 0;
    }
}