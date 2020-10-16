#include "game_of_life.h"
#include "motifs.h"

#include <fstream>
#include <vector>
#include <array>
#include <algorithm>


// Methodes utilitaies  ========================================================================================
bool GameOfLife::next_state(size_t i, size_t j) {
	short int S(0);
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
void GameOfLife::verif(size_t const& i, size_t const& j, liste& v) {
    if(!access(i,j) && std::find<liste::iterator, coord>(v.begin(),v.end(),{i,j}) == v.end()) {
		if (next_state(i,j)) {
			v.push_back({i,j});
			if (50 <= i && i < L+50 && 50 <= j && j < C+50) vivantes_visibles.push_back({i-50,j-50});
		}
	}
}
// Constructeurs ========================================================================================
GameOfLife::GameOfLife(motifs::Motif const& a_marquer, unsigned int const& C, unsigned int const& L) : nbr_gen(0), C(C), L(L) {
	for (size_t i(0); i < L+100; ++i) {
		for (size_t j(0); j < C+100; ++j) champ[i][j] = false;
	}
	for (liste::const_iterator it(a_marquer.cbegin()); it != a_marquer.cend(); ++it) {
		champ[(it->first)+50][(it->second)+50] = true;
		vivantes.push_back({it->first + 50, it->second + 50});
		vivantes_visibles.push_back({it->first, it->second});
	}
}
GameOfLife::GameOfLife(unsigned int const& C, unsigned int const& L) : C(C), L(L), nbr_gen(0) {
	for (auto& ligne : champ) for (auto& el : ligne) el = false;
}

// Getters & Setters ========================================================================================

bool GameOfLife::access(size_t i, size_t j) {
	if (i>=L+100 || j>=C+100) return false;
	else return champ[i][j];
}


void GameOfLife::add_cell(coord const& c) {
	coord c_translate({c.first+50,c.second+50});
	if((!access(c_translate.first, c_translate.second)) && ((c_translate.first < L+100) && (c_translate.second < C+100))) {
		vivantes.push_back(c_translate);
		vivantes_visibles.push_back(c);
        champ[c_translate.first][c_translate.second] = true;
	}
}

void GameOfLife::suppr_cell(coord const& c) {
	coord c_translate({c.first+50,c.second+50});
	if (access(c_translate.first, c_translate.second)) {
		champ[c_translate.first][c_translate.second] = false;
		vivantes.erase(std::find<liste::iterator, coord>(vivantes.begin(),	vivantes.end(), c_translate));
		vivantes_visibles.erase(std::find<liste::iterator, coord>(vivantes_visibles.begin(), vivantes_visibles.end(), c));
	}
}
void GameOfLife::inv_cell(coord const& c) {
	coord c_translate({c.first+50,c.second+50});
    if (access(c_translate.first, c_translate.second)) {
		champ[c_translate.first][c_translate.second] = false;
		vivantes.erase(std::find<liste::iterator, coord>(vivantes.begin(), vivantes.end(), c_translate));
		vivantes_visibles.erase(std::find<liste::iterator, coord>(vivantes_visibles.begin(), vivantes_visibles.end(), c));
	} else {
		if((c_translate.first < L+100) && (c_translate.second < C+100)) {
			champ[c_translate.first][c_translate.second] = true;
			vivantes.push_back(c_translate);
			vivantes_visibles.push_back(c);
		}
	}
}
void GameOfLife::add_motif(motifs::Motif const& m) {
	for (liste::const_iterator it(m.cbegin()); it != m.cend(); ++it) add_cell(*it);
}
void GameOfLife::suppr_motif(motifs::Motif const& m) {
	for (liste::const_iterator it(m.cbegin()); it != m.cend(); ++it) suppr_cell(*it);
}

// Affichage ========================================================================================
void GameOfLife::print(std::ostream& out) const {
	out << "Debut" << std::endl;
	for (size_t i(50); i < L+50; ++i) {
		for (size_t j(50); j < C+50; ++j) {
			if (champ[i][j]) out << 0;
			else out << ' ';
		}
		out << std::endl;
	}
	out << " Fin | Generation num " << nbr_gen << std::endl;
}

// Evolution de la simulation  ========================================================================================
void GameOfLife::evolve() {
	// On crée une nouvelle liste qui contiendra les nouvelles vivantes
	liste nouvelles;

	// On check chaque cellule déjà vivante pour voir si elle le reste
	for (liste::iterator it(vivantes.begin()); it != vivantes.end(); ++it) {
		if (next_state(it->first,it->second)) {
			nouvelles.push_back(*it);
			if (50 <= it->first && it->first < L+50 && 50 <= it->second && it->second < C+50) vivantes_visibles.push_back(*it);
		}

		// On vérifie dans ses voisines lesquelles étaient mortes et deviendraient potentiellement vivantes
		verif(it->first - 1, it->second - 1, nouvelles);
		verif(it->first - 1,it->second, nouvelles);
		verif(it->first - 1,it->second + 1, nouvelles);
		verif(it->first,it->second - 1, nouvelles);
		verif(it->first,it->second + 1, nouvelles);
		verif(it->first + 1,it->second - 1, nouvelles);
		verif(it->first + 1,it->second, nouvelles);
		verif(it->first - 1,it->second + 1, nouvelles);
	}
	// On enlève les anciennes cellules
	for (auto const& el : vivantes) champ[el.first][el.second] = false;
	// On rajoute les nouvelles
	for (auto const& el : nouvelles) champ[el.first][el.second] = true;
	// On update la liste des vivantes et le nombre de generatitons
	vivantes = nouvelles;
	++nbr_gen;
}

liste const& GameOfLife::life() {
	evolve();
	return vivantes_visibles;
}


// Gestion des motifs ========================================================================================
// On fera la gestion du nom (s'il existe dejà) en dehors, dans qt
void GameOfLife::save_motif(std::string const& nom_motif) const {
	std::ofstream out("../../data/"+nom_motif+".csv");
	for (auto const& el : vivantes_visibles) out << el.first << ',' << el.second << '\n';
	out.close();
}
void GameOfLife::save_motif(std::string const& nom_motif, size_t imin, size_t imax, size_t jmin, size_t jmax) const {
	std::ofstream out(nom_motif);
	for (auto const& el : vivantes_visibles) if (imin <= el.first && el.first < imax && jmin <= el.second && el.second < jmax) out << el.first << ',' << el.second << '\n';
	out.close();
}