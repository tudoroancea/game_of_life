#include "game_of_life.h"
#include "motifs.h"
#include <string>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm> //
#include <filesystem> // pour trouver les simulations existantes

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
void GameOfLife::verif(size_t const& i, size_t const& j, liste& v, liste& v_visibles) {
    if(!access(i,j) && std::find<liste::iterator, coord>(v.begin(),v.end(),{i,j}) == v.end()) {
        if (next_state(i,j) && i<L+100 && j<C+100) {
			v.push_back({i,j});
			size_t itranslate(i-50), jtranslate(j-50);
			if (0 <= itranslate && itranslate < L && 0 <= jtranslate && jtranslate < C) v_visibles.push_back({itranslate, jtranslate});
		}
	}
}
// Constructeurs ========================================================================================
GameOfLife::GameOfLife(Motif const& a_marquer, unsigned int const& C, unsigned int const& L) : C(C%401), L(L%401), nbr_gen(0) {
	for (size_t i(0); i < L+100; ++i) {
		for (size_t j(0); j < C+100; ++j) champ[i][j] = false;
	}
	for (liste::const_iterator it(a_marquer.cbegin()); it != a_marquer.cend(); ++it) {
		if (it->first < L+100 && it->second < C+100) {
			champ[(it->first)+50][(it->second)+50] = true;
			vivantes.push_back({it->first + 50, it->second + 50});
			vivantes_visibles.push_back({it->first, it->second});
		}
	}
}
GameOfLife::GameOfLife(unsigned int const& C, unsigned int const& L) : C(C), L(L), nbr_gen(0) {
	for (size_t i(0); i < L+100; ++i) {
		for (size_t j(0); j < C+100; ++j) champ[i][j] = false;
	}
}

// Getters & Setters ========================================================================================

bool GameOfLife::access(size_t i, size_t j) {
	if (i>=L+100 || j>=C+100) return false;
	else return champ[i][j];
}
liste const& GameOfLife::get_viv() const {return vivantes_visibles;}

void GameOfLife::add_cell(coord const& c) {
	coord c_translate({c.first+50,c.second+50});
	if((!access(c_translate.first, c_translate.second)) && ((c_translate.first < L+50) && (c_translate.second < C+50))) {
		vivantes.push_back(c_translate);
		vivantes_visibles.push_back(c);
        champ[c_translate.first][c_translate.second] = true;
	}
}
void GameOfLife::add_cell(size_t const& i, size_t const& j) {add_cell({i,j});}
void GameOfLife::suppr_cell(coord const& c) {
	coord c_translate({c.first+50,c.second+50});
	if (access(c_translate.first, c_translate.second)) {
		champ[c_translate.first][c_translate.second] = false;
		vivantes.erase(std::find<liste::iterator, coord>(vivantes.begin(),	vivantes.end(), c_translate));
		vivantes_visibles.erase(std::find<liste::iterator, coord>(vivantes_visibles.begin(), vivantes_visibles.end(), c));
	}
}
void GameOfLife::suppr_cell(size_t const& i, size_t const& j) {suppr_cell({i,j});}
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
void GameOfLife::inv_cell(size_t const& i, size_t const& j) {inv_cell({i,j});}
void GameOfLife::add_motif(Motif const& m) {
	for (liste::const_iterator it(m.cbegin()); it != m.cend(); ++it) add_cell(*it);
}
void GameOfLife::suppr_motif(Motif const& m) {
	for (liste::const_iterator it(m.cbegin()); it != m.cend(); ++it) suppr_cell(*it);
}
void GameOfLife::wipe() {
	for (liste::iterator it(vivantes.begin()); it!= vivantes.end();) {
		champ[it->first][it->second] = false;
		vivantes.erase(it);
	}
	vivantes_visibles.erase(vivantes_visibles.begin(), vivantes_visibles.end());
}

// Affichage ========================================================================================
void GameOfLife::print(std::ostream& out) const {
	out << "Debut grille" << std::endl;
	for (size_t i(50); i < L+50; ++i) {
		for (size_t j(50); j < C+50; ++j) {
			if (champ[i][j]) out << 0;
			else out << '-';
		}
		out << std::endl;
	}
	out << " Fin grille | Generation num " << nbr_gen << std::endl;
}

// Evolution de la simulation  ========================================================================================
void GameOfLife::evolve() {
	// On crée une nouvelle liste qui contiendra les nouvelles vivantes
	liste nouvelles;
	liste nouvelles_visibles;
	// On check chaque cellule déjà vivante pour voir si elle le reste
	for (liste::iterator it(vivantes.begin()); it != vivantes.end(); ++it) {
		if (next_state(it->first,it->second)) {
			nouvelles.push_back(*it);
			if (50 <= it->first && it->first < L+50 && 50 <= it->second && it->second < C+50) nouvelles_visibles.push_back({it->first - 50, it->second - 50});
		}

		// On vérifie dans ses voisines lesquelles étaient mortes et deviendraient potentiellement vivantes
		verif(it->first - 1, it->second - 1, nouvelles, nouvelles_visibles);
		verif(it->first - 1, it->second, nouvelles, nouvelles_visibles);
		verif(it->first - 1, it->second + 1, nouvelles, nouvelles_visibles);
		verif(it->first, it->second - 1, nouvelles, nouvelles_visibles);
		verif(it->first, it->second + 1, nouvelles, nouvelles_visibles);
		verif(it->first + 1, it->second - 1, nouvelles, nouvelles_visibles);
		verif(it->first + 1, it->second, nouvelles, nouvelles_visibles);
		verif(it->first - 1, it->second + 1, nouvelles, nouvelles_visibles);
	}
	// On enlève les anciennes cellules
	for (auto const& el : vivantes) champ[el.first][el.second] = false;

	// On rajoute les nouvelles
	for (auto const& el : nouvelles) champ[el.first][el.second] = true;
	// On update la liste des vivantes et le nombre de generatitons
	vivantes = nouvelles;
	vivantes_visibles = nouvelles_visibles;
	++nbr_gen;
}
std::vector<std::string> existing_local_sims() {
    std::vector<std::string> res;
    std::filesystem::path sims("../../data/local/sims");
    for (auto const& entry : std::filesystem::directory_iterator(sims)) {
        if (entry.is_directory()) {
            res.push_back(entry.path().stem().string());
        }
    }
    return res;
}
std::vector<std::string> existing_presaved_sims() {
    std::vector<std::string> res;
    std::filesystem::path sims("../../data/presaved/sims");
    for (auto const& entry : std::filesystem::directory_iterator(sims)) {
        if (entry.is_directory()) {
            res.push_back(entry.path().stem().string());
        }
    }
    return res;
}

bool GameOfLife::life(std::string const& nom_simulation, unsigned int const& duree_sim, std::string const& categorie) {
	// On verifie dans quel dossier on doit enregistrer la simulation
	std::filesystem::path chemin;
	if (categorie != "local") chemin = std::filesystem::path("../../data/presaved/sims/"+nom_simulation);
    else chemin = std::filesystem::path("../../data/local/sims/"+nom_simulation);

	// On verifie si un dossier du même nom existe deja, et sinon en enregistre la simulation
	if (!std::filesystem::exists(chemin)) {
		std::filesystem::create_directory(chemin);
		std::ofstream out;

		// On crée les informations de la simulation (dimensions de la grille, nombre de generations)
		out.open(chemin.string()+"/"+nom_simulation+"-info.csv");
		out << L << ',' << C << '\n' << duree_sim << ",\n";
		out.close();
		// On cree un premier fichier pour la config au depart (avant lacement de la simulation)
		out.open(chemin.string()+"/"+nom_simulation+"-0.csv");
		for (auto const& el : vivantes_visibles) out << el.first << ',' << el.second << '\n';
		out.close();

		// On enregistre un nouveau fichier pour chaque nouvelle generation
		for (size_t i(0); i < duree_sim ; ++i) {
			evolve();
			out.open(chemin.string()+"/"+nom_simulation+"-"+std::to_string(this->nbr_gen)+".csv");
			for (auto const& el : vivantes_visibles) out << el.first << ',' << el.second << '\n';
			out.close();
		}
		return true;
	} else return false;
}
// Gestion des motifs ========================================================================================
void GameOfLife::save_motif(std::string const& nom_motif, std::string const& dossier) const {
	save_motif(nom_motif, 0, L, 0, C, dossier);
}
void GameOfLife::save_motif(std::string const& nom_motif, size_t imin, size_t imax, size_t jmin, size_t jmax, std::string const& dossier) const {
	std::ofstream out;
	if (dossier != "local") out = std::ofstream("../../data/presaved/motifs/"+nom_motif+".csv");
	else out = std::ofstream("../../data/local/motifs/"+nom_motif+".csv");
	out << "ligne,colonne\n";
	for (auto const& el : vivantes_visibles) if (imin <= el.first && el.first < imax && jmin <= el.second && el.second < jmax) out << el.first << ',' << el.second << '\n';
	out.close();
}
