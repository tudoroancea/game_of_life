#include "game_of_life.h"
#include "motifs.h"

#include <fstream>
#include <vector>
#include <array>

bool GameOfLife::next_state(size_t i, size_t j) {
    int S(0);
    if (access(i - 1, j - 1)) ++S;
    if (access(i - 1, j)) ++S;
    if (access(i - 1, j + 1)) ++S;
    if (access(i, j - 1)) ++S;
    if (access(i, j + 1)) ++S;
    if (access(i + 1, j - 1)) ++S;
    if (access(i + 1, j)) ++S;
    if (access(i + 1, j + 1)) ++S;
    return (S==3) || (access(i, j) && S == 2);
}

// Constructeurs ========================================================================================
GameOfLife::GameOfLife(motifs::Motif const& a_marquer, unsigned int const& C, unsigned int const& L) : nbr_gen(0), C(C), L(L) {
   for (size_t i(0); i < L+100; ++i) {
      for (size_t j(0); j < C+100; ++j) champ[i][j] = false;
   }
   for (vec::const_iterator it(a_marquer.cbegin()); it != a_marquer.cend(); ++it) champ[(it->first)+50][(it->second)+50] = true;
}
GameOfLife::GameOfLife(unsigned int const& C, unsigned int const& L) : C(C), L(L), nbr_gen(0) {
   for (auto& ligne : champ) for (auto& el : ligne) el = false;
}

// Getters & Setters ========================================================================================
bool GameOfLife::access(size_t i, size_t j) {
    if (i>=L+100 || j>=C+100) return false;
    else return champ[i][j];
}
void GameOfLife::add_cell(std::pair<size_t,size_t> const& c) {
   if(!access(c.first, c.second) and c.first < L+100 and c.second < C+100) {
      vivantes.push_back(c);
      champ[c.first][c.second] = true;
   }
}
void GameOfLife::suppr_cell(std::pair<size_t,size_t> const& c) {
   if (access(c.first, c.second)) {
      champ[c.first][c.second] = false;
      std::vector<std::pair<size_t,size_t>>::iterator it(vivantes.begin());
      while (*it != c && it != vivantes.end()) {
          ++it;
      }
      vivantes.erase(it);
   }
}
void GameOfLife::add_motif(motifs::Motif const& m) {
   for (std::vector<std::pair<size_t,size_t>>::const_iterator it(m.cbegin()); it != m.cend(); ++it) add_cell(*it);
}
void GameOfLife::suppr_motif(motifs::Motif const& m) {
   for (std::vector<std::pair<size_t,size_t>>::const_iterator it(m.cbegin()); it != m.cend(); ++it) suppr_cell(*it);
}

// Affichage ========================================================================================
void GameOfLife::print(std::ostream& out) const {
      out << "Debut" << std::endl;
      for (size_t i(50); i < L+50; ++i) {
      for (size_t j(50); j < C+50; ++j) {
         if (champ[i][j]) out << 1;
         else out << ' ';
      }
      out << std::endl;
   }
      out << " Fin | Generation num " << nbr_gen << std::endl;
}

// Gestion des motifs ========================================================================================
// On fera la gestion du nom (s'il existe dejà) en dehors, dans qt
void GameOfLife::save_motif(std::string const& nom_motif) const {
   std::ofstream out(nom_motif);
   for (auto const& el : vivantes) out << el.first << ',' << el.second << '\n';
   out.close();
}
void GameOfLife::save_motif(std::string const& nom_motif, size_t imin, size_t imax, size_t jmin, size_t jmax) const {
   std::ofstream out(nom_motif);
   for (auto const& el : vivantes) if (imin <= el.first && el.first < imax && jmin <= el.second && el.second < jmax) out << el.first << ',' << el.second << '\n';
   out.close();
}

// Evolution de la simulation  ========================================================================================
void GameOfLife::evolve() {
   // On crée une nouvelle liste qui contiendra les nouvelles vivantes
   std::vector<std::pair<size_t,size_t>> nouvelles;

   // On cheque chaque cellule déjà vivante pour voir si elle le restte
   for (std::vector<std::pair<size_t,size_t>>::iterator it(vivantes.begin()); it != vivantes.end(); ++it) {
      if (next_state(it->first, it->second)) {
         nouvelles.push_back(*it);
      }
      // On vérifie dans ses voisines lesquelles étaient mortes et deviendraient potentiellement vivantes
      if (access(it->first - 1, it->second - 1)) if (next_state(it->first -1, it->second - 1)) nouvelles.push_back(*it);
      if (access(it->first - 1, it->second)) if (next_state(it->first - 1, it->second)) nouvelles.push_back(*it);
      if (access(it->first - 1, it->second + 1)) if (next_state(it->first - 1, it->second + 1)) nouvelles.push_back(*it);
      if (access(it->first, it->second - 1)) if (next_state(it->first, it->second - 1)) nouvelles.push_back(*it);
      if (access(it->first, it->second + 1)) if (next_state(it->first, it->second + 1)) nouvelles.push_back(*it);
      if (access(it->first + 1, it->second - 1)) if (next_state(it->first + 1, it->second - 1)) nouvelles.push_back(*it);
      if (access(it->first + 1, it->second)) if (next_state(it->first + 1, it->second)) nouvelles.push_back(*it);
      if (access(it->first + 1, it->second + 1)) if (next_state(it->first + 1, it->second + 1)) nouvelles.push_back(*it);
   }
   // On enlève les anciennes cellules
   for (auto const& el : vivantes) champ[el.first][el.second] = false;
   // On rajoute les nouvelles
   for (auto const& el : nouvelles) champ[el.first][el.second] = true;
   // On update la liste des vivantes et le nombre de generatitons
   vivantes = nouvelles;
   ++nbr_gen;
}

std::vector<std::pair<size_t,size_t>> const& GameOfLife::life(std::ostream& out) {
   evolve();
   return vivantes;
}
