#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H
#include <array>
#include <iostream>
#include "motifs.h"


template<unsigned int L, unsigned int C>
class GameOfLife {
private :
    std::array<std::array<bool,C>,L> champ;
    unsigned int nbr_gen;

    bool access(size_t i, size_t j);
    bool next_state(size_t i, size_t j);
public :
    GameOfLife(motifs::Motif const& a_marquer);
    void print(std::ostream& out = std::cout) const;
    void evolve();
    void life(std::ostream& out = std::cout);
};

#endif // GAME_OF_LIFE_H
