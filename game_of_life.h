#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H
#include <array>
#include <iostream>
#include "motifs.h"


class GameOfLife {
private :
    unsigned int C,L;   // Partie visible de la grille
    unsigned int nbr_gen;
    std::array<std::array<bool,500>,500> champ;     // Grille de stockage des etats des cellules
    std::vector<std::pair<size_t,size_t>> vivantes;

    bool next_state(size_t i, size_t j);
public :
    // Constructeurs
    GameOfLife(motifs::Motif const& a_marquer, unsigned int const& C = 50,unsigned int const& L = 50);
    GameOfLife(unsigned int const& C = 50, unsigned int const& L = 50);

    // Getter & setters
    bool access(size_t i, size_t j);
    void add_cell(std::pair<size_t,size_t> const& c);
    void suppr_cell(std::pair<size_t,size_t> const& c);
    void add_motif(motifs::Motif const& m);
    void suppr_motif(motifs::Motif const& m);

    // Evolution de la simulaitton
    void evolve();
    std::vector<std::pair<size_t,size_t>> const& life(std::ostream& out = std::cout);

    // Affichage
    void print(std::ostream& out = std::cout) const;
};

#endif // GAME_OF_LIFE_H
