#include "game_of_life.h"
#include "motifs.h"

template<unsigned int L, unsigned int C>
bool GameOfLife<L,C>::access(size_t i, size_t j) {
    if (i>=L or j>=C) return false;
    else return champ[i][j];
}
template<unsigned int L, unsigned int C>
bool GameOfLife<L,C>::next_state(size_t i, size_t j) {
    int S(0);
    if (access(i - 1, j - 1)) ++S;
    if (access(i - 1, j)) ++S;
    if (access(i - 1, j + 1)) ++S;
    if (access(i, j - 1)) ++S;
    if (access(i, j + 1)) ++S;
    if (access(i + 1, j - 1)) ++S;
    if (access(i + 1, j)) ++S;
    if (access(i + 1, j + 1)) ++S;
    return (S==3) or (access(i, j) and S == 2);
}

template<unsigned int L, unsigned int C>
GameOfLife<L,C>::GameOfLife(motifs::Motif const& a_marquer) : nbr_gen(0) {
   for (size_t i(0); i < L ; ++i) {
      for (size_t j(0); j < C ; ++j) champ[i][j] = false;
   }
   for (vec::const_iterator it(a_marquer.cbegin()); it != a_marquer.cend(); ++it) champ[it->first][it->second] = true;
}

template<unsigned int L, unsigned int C>
void GameOfLife<L,C>::print(std::ostream& out) const {
      out << "Debut" << std::endl;
      for (size_t i(0); i < L; ++i) {
      for (size_t j(0); j < C; ++j) {
         if (champ[i][j]) out << 1;
         else out << ' ';
      }
      out << std::endl;
   }
      out << " Fin | Generation num " << nbr_gen << std::endl;
}

template<unsigned int L, unsigned int C>
void GameOfLife<L,C>::evolve() { // todo : optimiser les zones vides
      std::array<std::array<bool,C>,L> res;
   for (size_t i(0); i < L ; ++i) {
      for (size_t j = 0; j < C ; ++j) {
         res[i][j]=next_state(i,j);
      }
   }
   champ=res;
   ++nbr_gen;
}

template<unsigned int L, unsigned int C>
void GameOfLife<L,C>::life(std::ostream& out) {
   print(out);
   evolve();
}

