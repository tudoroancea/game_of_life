#include "affichage.h"
#include "motifs.h"
#include "game_of_life.h"
#include "rapidcsv.h"

#include <vector>
#include <iostream>
#include <cstdlib> // pour std::sytem()
#include <stdexcept> // pou std::invalid_argument pour stoi()
#include <chrono> // pour les s, ms
#include <thread> // pour std::this_thread::sleep_for();
#include <future>

#define SWITCH_SIMULATION_ERROR(err) {\
    switch (err) {\
        case Simulation::Error::NON_EXISTING_SIM:\
            std::cerr << "[ERROR : Dossier <" << sim.get_nom() << "> n'existe pas.]" << std::endl;\
            return -1;\
        case Simulation::Error::NON_EXISTING_INFO: {\
            std::cerr << "[ERROR : Fichier de configuration <" << sim.get_nom() << "-info.csv> n'existe pas.]" << std::endl;\
            return -1;\
        }\
        case Simulation::Error::INCOMPLETE_INFO: {\
            std::cerr << "[ERROR : Fichier de configuration <" << sim.get_nom() << "-info.csv> incomplet.]" << std::endl;\
            return -1;\
        }\
        case Simulation::Error::NON_EXISTING_CONTENT: {\
            std::cerr << "[ERROR : Fichier de contenu <" << sim.get_nom() << "-contenu.csv> n'existe pas.]" << std::endl;\
            return -1;\
        }\
        case Simulation::Error::INCOMPLETE_CONTENT: {\
            std::cerr << "[ERROR : Fichier de contenu <" << sim.get_nom() << "-content.csv> incomplet.]" << std::endl;\
            return -1;\
        }\
        default:\
            std::cerr << "[UNEXPECTED ERROR]" << std::endl;\
            break;\
    }\
}

/*
int CliApp::exec() {
    CLEAR()
    std::cout << "Debut d'une nouvelle simulation." << std::endl;
    std::cout << "Que voulez vous faire ? " << std::endl;
    std::cout << " 1) Simulation pre-enregistrée" << std::endl;
    std::cout << " 2) Nouvelle simulation" << std::endl;
	int input(0);
	std::string line("");
	while (input < 1 || input > 2) {
        std::cin >> input;
        if (input < 1 || input > 2) std::cout << " Choix invalide, ressaisissez vous.";
    }
    CLEAR()
    if (input == 1) { // Simulation pre-enregistree
        // Quelle simulation on veut afficher ?
        // Liste des simulations existentes
        std::vector<std::string> loc_sims(existing_local_sims()), saved_sims(existing_presaved_sims());
        if (loc_sims.empty() && saved_sims.empty()) {
            std::cerr << " ERROR : Aucune simulation enregistrée" << std::endl;
            return -1;
        }
        std::cout << "Les simulations disponibles sont : " << std::endl;
        std::cout << "  Simulations locales :" << std::endl;
        for (size_t i(0); i < loc_sims.size() ; ++i) std::cout << "   " << i+1 << ") " << loc_sims[i] << std::endl;
        std::cout << "   Simulations generales :" << std::endl;
        for (size_t i(0); i < saved_sims.size() ; ++i) std::cout << "   " << i+1+loc_sims.size() << ") " << saved_sims[i] << std::endl;
        // Selection de la simulation
        input = 0;
        while (input < 1 || size_t(input) > loc_sims.size()+saved_sims.size()) {
            std::cin >> input;
            if (input < 1 || size_t(input) > loc_sims.size()+saved_sims.size()) std::cout << " Choix invalide, ressaisissez vous.";
        }

        // On charge la simulation correspondante
        Simulation sim;
        std::cout << "Chargement du fichier de configuration de la simulation" << std::endl;
        if (size_t(input) <= loc_sims.size()) {
            try {
                sim.load(loc_sims[input-1], local);
            }
            catch (Simulation::Error const& err) {
                SWITCH_SIMULATION_ERROR(err)
            }
        } else {
            try {
                sim.load(saved_sims[input-1-loc_sims.size()], presaved);
            }
            catch (Simulation::Error const& err) {
                SWITCH_SIMULATION_ERROR(err)
            }
        }
        std::cout << "Chargement réussi. Veuillez appuyer n'importe sur quelle touche puis ENTRÉE pour continuer ";
        std::cin >> line;

        // Demande de la vitesse de la simulation
        CLEAR()
        std::cout << "Veuillez entrer la vitesse de simulation (durée d'une génération en ms (entier positif)) : ";
        input = 0;
        while (input < 1) {
            std::cin >> input;
            if (input < 1) std::cout << " Choix invalide, ressaisissez vous.";
        }

        // Affichage de la simulation
        CLEAR()
        std::cout << "Grille au début de la simulation : " << std::endl;
        sim.print();
        std::cout << "Appuyez sur n'importe quelle touche puis ENTRÉE pour continuer ";
        std::cin >> line;
        try {
            while (!sim.finie()) {
                CLEAR()
                sim.evolve();
                sim.print();
                std::this_thread::sleep_for(std::chrono::duration<size_t, std::milli>(input));
            }
        }
        catch (Simulation::Error const& err) {
            switch (err) {
                case Simulation::Error::INCOMPLETE_INFO:
                    std::cerr << "[ERROR : Fichier de configuration <" << sim.get_nom() << "-info.csv> incomplet.]" << std::endl;
                    return -1;
                case Simulation::Error::INCOMPLETE_CONTENT:
                    std::cerr << "[ERROR : Fichier de contenu <" << sim.get_nom() << "-content.csv> incomplet.]" << std::endl;
                    return -1;
                default:
                    std::cerr << "[UNEXPECTED ERROR]" << std::endl;
                    return -1;
            }
        }
        std::cout << "Simulation terminée avec succès" << std::endl;
        return 0;
    } else { // Nouvelle simulation
        // On demande la vitesse de simulation
        unsigned int vitesse(100);
        bool test(true);
        GameOfLife G;
        auto affichage = std::async(std::launch::deferred, [&G, test, vitesse]() {
            while (test) {
                G.evolve();
                G.print();
                std::this_thread::sleep_for(std::chrono::milliseconds(vitesse));
            }
        });
        return 0;
    }
}
*/