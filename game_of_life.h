#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H
#include <array>
#include <iostream>
#include "motifs.h"


class GameOfLife {
private :
    unsigned int C,L;
    std::array<std::array<bool,500>,500> champ;
    unsigned int nbr_gen;

    bool access(size_t i, size_t j);
    bool next_state(size_t i, size_t j);
public :
    GameOfLife(motifs::Motif const& a_marquer, unsigned int const& C = 50,unsigned int const& L = 50);
    void print(std::ostream& out = std::cout) const;
    void evolve();
    void life(std::ostream& out = std::cout);
};

#endif // GAME_OF_LIFE_H
