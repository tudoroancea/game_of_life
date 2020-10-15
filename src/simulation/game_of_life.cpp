#include "game_of_life.h"
#include "motifs.h"

#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

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
for (	vec::const_iterator it(a_marquer.cbegin()); it != a_marquer.cend(); ++it) champ[(it->first)+50][(it->second)+50] = true;
}
GameOfLife::GameOfLife(unsigned int const& C, unsigned int const& L) : C(C), L(L), nbr_gen(0) {
for (	auto& ligne : champ) for (auto& el : ligne) el = false;
}

// Getters & Setters ========================================================================================
bool GameOfLife::access(size_t i, size_t j) {
	if (i>=L+100 || j>=C+100) return false;
	else return champ[i][j];
}

/*
   @brief   Verifie si la cellule indiquee est pas deja dans la grille et sinon l'y insere
   @param   c  coordonnes par rapport a grille affichee (entre 0 et L et entre 0 et C)
 */
void GameOfLife::add_cell(std::pair<size_t,size_t> const& c) {
	std::pair<size_t,size_t> c_translate({c.first+50,c.second+50});
	if((!access(c_translate.first, c_translate.second)) && ((c_translate.first < L+100) && (c_translate.second < C+100))) {
		vivantes.push_back(c_translate);
        champ[c_translate.first][c_translate.second] = true;
	}
}
/*
   @brief   Verifie si la cellule indiquee est bien vivante et si oui la supprime de la grille (et de vivantes)
   @param   c   coordonnes par rapport a grille affichee (entre 0 et L et entre 0 et C)
 */
void GameOfLife::suppr_cell(std::pair<size_t,size_t> const& c) {
	std::pair<size_t,size_t> c_translate({c.first+50,c.second+50});
	if (access(c_translate.first, c_translate.second)) {
		champ[c_translate.first][c_translate.second] = false;
		std::vector<std::pair<size_t,size_t>>::iterator it(std::find<std::vector<std::pair<size_t,size_t>>::iterator, std::pair<size_t,size_t>>(vivantes.begin(), vivantes.end(), c_translate));
		if (it != vivantes.end()) vivantes.erase(it);
	}
}
/*
   @brief   Verifie si la cellule indiquee est bien dans la grille et si oui inverse sa valeur
   @param   c  coordonnes par rapport a grille affichee (entre 0 et L et entre 0 et C)
 */
void GameOfLife::inv_cell(std::pair<size_t,size_t> const& c) {
	std::pair<size_t,size_t> c_translate({c.first+50,c.second+50});
    bool in_(!access(c_translate.first, c_translate.second));
    if((in_) && ((c_translate.first < L+100) && (c_translate.second < C+100))) {vivantes.push_back(c_translate); champ[c_translate.first][c_translate.second] = true;}
	else if (in_) {
        champ[c_translate.first][c_translate.second] = false;
        std::vector<std::pair<size_t,size_t>>::iterator it(std::find<std::vector<std::pair<size_t,size_t>>::iterator, std::pair<size_t,size_t>>(vivantes.begin(), vivantes.end(), c_translate));
        if (it != vivantes.end()) vivantes.erase(it);
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
void GameOfLife::verif(size_t const& i, size_t const& j, std::vector<std::pair<size_t,size_t>>& v) {
    if(!access(i,j) && std::find<std::vector<std::pair<size_t,size_t>>::iterator, std::pair<size_t,size_t>>(v.begin(),v.end(),{i,j}) == v.end()) if (next_state(i,j)) {v.push_back({i,j});}
}

void GameOfLife::evolve() {
	// On crée une nouvelle liste qui contiendra les nouvelles vivantes
	std::vector<std::pair<size_t,size_t>> nouvelles;

	// On cheque chaque cellule déjà vivante pour voir si elle le restte
	for (std::vector<std::pair<size_t,size_t>>::iterator it(vivantes.begin()); it != vivantes.end(); ++it) {
		if (next_state(it->first,it->second)) nouvelles.push_back(*it);

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
	for (auto a : vivantes) { std::cout << a.first << " " << a.second << " | ";}
	std::cout << std::endl;
	++nbr_gen;
   // On enlève les anciennes cellules
   for (auto const& el : vivantes) champ[el.first][el.second] = false;
   // On rajoute les nouvelles
   for (auto const& el : nouvelles) champ[el.first][el.second] = true;
   // On update la liste des vivantes et le nombre de generatitons
   vivantes = nouvelles;
   ++nbr_gen;

}

std::vector<std::pair<size_t,size_t>> GameOfLife::life(std::ostream& out) {
	evolve();
	std::vector<std::pair<size_t,size_t>> res;
	for (auto const& el : vivantes) if (50 <= el.first && el.first < L+50 && 50 <= el.second && el.second < C+50) res.push_back({el.first-50, el.second-50});
	return res;
}
