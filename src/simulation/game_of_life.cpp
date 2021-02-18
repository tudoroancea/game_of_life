#include "game_of_life.h"
#include "motifs.h"
#include "rapidcsv.h"
#include "termcolor.hpp"

#include <queue>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <array>
#include <functional>
#include <algorithm> //
#include <typeinfo> // for typeid
#include <filesystem> // pour trouver les simulations existantes
#include <chrono>

template<class T1, class T2>
std::size_t pair_hash::operator()(std::pair<T1,T2> const& p) const {
	if (p.first == p.second) {
		size_t first = p.first, second = (p.first << 2)|(p.first >> (8*sizeof(size_t)-2));;
		return std::hash<T1>()(first) ^ std::hash<T2>()(second);
	} else {
		return std::hash<T1>()(p.first) ^ std::hash<T2>()(p.second);
	}
}

// Constructeurs ========================================================================================
GameOfLife::GameOfLife() : nbr_gen(0) {
	for (size_t i(0); i < MAX_LIGNES+100; ++i) {
		for (size_t j(0); j < MAX_COLONNES+100; ++j) grille[i][j] = false;
	}
}
GameOfLife::GameOfLife(Motif const& a_marquer) : nbr_gen(0) {
	for (size_t i(0); i < MAX_LIGNES+100; ++i) {
		for (size_t j(0); j < MAX_COLONNES+100; ++j) grille[i][j] = false;
	}
	for (liste::const_iterator it(a_marquer.cbegin()); it != a_marquer.cend(); ++it) {
		if (it->first < MAX_LIGNES+100 && it->second < MAX_COLONNES+100) {
			grille[(it->first)+50][(it->second)+50] = true;
			vivantes.push_back({it->first, it->second});
		}
	}
}

// Getters & Setters ========================================================================================
bool GameOfLife::access(size_t const& i, size_t const& j) const {
	if (i < MAX_LIGNES+100 && j < MAX_COLONNES+100) return grille[i][j];
	else {
		#ifdef OVERFLOW_WARNINGS
			std::cerr << termcolor::yellow << "[GameOfLife::access(" << i << "," << j << ") a renvoyé false car les coordonées étaient trop grandes]" << termcolor::reset;
		#endif
		return false;
	}
}
liste const& GameOfLife::get_viv() const {return vivantes;}
std::array<std::array<bool,MAX_LIGNES+100>,MAX_COLONNES+100> const& GameOfLife::get_grille() const {return grille;}

// Setters ================================================
GameOfLife& GameOfLife::add_cell(size_t const& i, size_t const& j) {
	#ifdef OVERFLOW_WARNINGS
	if (i < MAX_LIGNES && j < MAX_COLONNES) {
		if (!access(i+50, j+50)) {
			vivantes.push_back({i+50, j+50});
			grille[i+50][j+50] = true;
		}
	} else {
		std::cerr << termcolor::yellow << "[GameOfLife::add_cell(" << i << "," << j << ") n'a rien fait car les coordonnées étaient trop grandes]" << termcolor::reset;
	}
	#else
	if (!access(i+50,j+50) && i < MAX_LIGNES && j < MAX_COLONNES) {
		vivantes.push_back({i+50, j+50});
		grille[i+50][j+50] = true;
	}
	#endif
	return *this;
}
GameOfLife& GameOfLife::add_cell(coord const& c) {return add_cell(c.first, c.second);}
GameOfLife& GameOfLife::suppr_cell(size_t const& i, size_t const& j) {
	#ifdef OVERFLOW_WARNINGS
	if (i < MAX_LIGNES && j < MAX_COLONNES) {
		if (access(i+50, j+50)) {
			grille[i+50][j+50] = false;
			liste::iterator a_effacer(std::find<liste::iterator, coord>(vivantes.begin(),	vivantes.end(), {i+50,j+50}));
			if (a_effacer != vivantes.end()) vivantes.erase(a_effacer);
		}
	} else {
		std::cerr << termcolor::yellow << "[GameOfLife::suppr_cell(" << i << "," << j << ") n'a rien fait car les coordonnées étaient trop grandes]" << termcolor::reset;
	}
	#else
	if (access(i+50, j+50)) {
		grille[i+50][j+50] = false;
		liste::iterator a_effacer(std::find<liste::iterator, coord>(vivantes.begin(),	vivantes.end(), {i+50,j+50}));
		if (a_effacer != vivantes.end()) vivantes.erase(a_effacer);
	}
	#endif
	return *this;
}
GameOfLife& GameOfLife::suppr_cell(coord const& c) {return suppr_cell(c.first, c.second);}
GameOfLife& GameOfLife::inv_cell(size_t const& i, size_t const& j) {
	#ifdef OVERFLOW_WARNINGS
	if (i < MAX_LIGNES && j < MAX_COLONNES) {
		if (access(i+50, j+50)) {
			grille[i+50][j+50] = false;
			liste::iterator a_effacer(std::find<liste::iterator, coord>(vivantes.begin(),	vivantes.end(), {i+50, j+50}));
			if (a_effacer != vivantes.end()) vivantes.erase(a_effacer);
		} else {
			grille[i+50][j+50] = true;
			vivantes.push_back({i+50,j+50});
		}
	} else {
		std::cerr << termcolor::yellow << "[GameOfLife::inv_cell(" << i << "," << j << ") n'a rien fait car les coordonnées étaient trop grandes]" << termcolor::reset;
	}
	#else
	if (access(i+50, j+50)) {
		grille[i+50][j+50] = false;
		liste::iterator a_effacer(std::find<liste::iterator, coord>(vivantes.begin(),	vivantes.end(), {i+50, j+50}));
		if (a_effacer != vivantes.end()) vivantes.erase(a_effacer);
	} else {
		if((i < MAX_LIGNES) && (j < MAX_COLONNES)) {
			grille[i+50][j+50] = true;
			vivantes.push_back({i+50,j+50});
		}
	}
	#endif
	return *this;
}
GameOfLife& GameOfLife::inv_cell(coord const& c) {return inv_cell(c.first, c.second);}
GameOfLife& GameOfLife::add_motif(Motif const& m) {
	for (liste::const_iterator it(m.cbegin()); it != m.cend(); ++it) add_cell(*it);
	return *this;
}
GameOfLife& GameOfLife::suppr_motif(Motif const& m) {
	for (liste::const_iterator it(m.cbegin()); it != m.cend(); ++it) suppr_cell(*it);
	return *this;
}
GameOfLife& GameOfLife::wipe() {
	for (liste::iterator it(vivantes.begin()); it!= vivantes.end();) {
		grille[it->first][it->second] = false;
		vivantes.erase(it);
	}
	return *this;
}

unsigned int& GameOfLife::get_nbr_gen() {return nbr_gen;}
// Affichage ========================================================================================
void GameOfLife::print(std::ostream& out) const {
	out << "Génération n° " << nbr_gen << "\n";
	for (size_t i(50); i < MAX_LIGNES+50; ++i) {
		for (size_t j(50); j < MAX_COLONNES+50; ++j) {
			if (grille[i][j]) out << 0;
			else out << ' ';
		}
		out << std::endl;
	}
	out << "\n=================================\n";
}

// Evolution de la simulation  ========================================================================================
bool GameOfLife::next_state(size_t const& i, size_t const& j) const {
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
    #ifdef OVERFLOW_WARNINGS
	if (i < MAX_LIGNES+100 && j < MAX_COLONNES+100) {
		if(!access(i,j) && std::find<liste::iterator, coord>(v.begin(),v.end(),{i,j}) == v.end()) {
			if (next_state(i,j)) {
				v.push_back({i,j});
			}
		}
	} else {
		std::cerr << termcolor::yellow << "[verif(" << i << "," << j << ",vivantes) n'a rien fait car les coords étaient trop grandes]" << termcolor::reset;
	}
	#else
	if(!access(i,j) && std::find<liste::iterator, coord>(v.begin(),v.end(),{i,j}) == v.end()) {
		if (next_state(i,j) && i < MAX_LIGNES+100 && j < MAX_COLONNES+100) {
			v.push_back({i,j});
		}
	}
	#endif
}
void GameOfLife::evolve() {
	// On crée une nouvelle liste qui contiendra les nouvelles vivantes
	liste nouvelles;
	// On check chaque cellule déjà vivante pour voir si elle le reste
	for (liste::iterator it(vivantes.begin()); it != vivantes.end(); ++it) {
		if (next_state(it->first,it->second)) {
			nouvelles.push_back(*it);
		}

		// On vérifie dans ses voisines lesquelles étaient mortes et deviendraient potentiellement vivantes
		verif(it->first - 1, it->second - 1, nouvelles);
		verif(it->first - 1, it->second, nouvelles);
		verif(it->first - 1, it->second + 1, nouvelles);
		verif(it->first, it->second - 1, nouvelles);
		verif(it->first, it->second + 1, nouvelles);
		verif(it->first + 1, it->second - 1, nouvelles);
		verif(it->first + 1, it->second, nouvelles);
		verif(it->first - 1, it->second + 1, nouvelles);
	}
	// On enlève les anciennes cellules
	for (auto const& el : vivantes) grille[el.first][el.second] = false;

	// On rajoute les nouvelles
	for (auto const& el : nouvelles) grille[el.first][el.second] = true;
	// On update la liste des vivantes et le nombre de generatitons
	vivantes = nouvelles;
	++nbr_gen;
}



// Enregistrement de motifs et simulations ========================================================================================
bool GameOfLife::save_motif(std::string const& nom_motif, FILE_CATEGORY const& categorie) const {
	std::filesystem::path chemin;
	if (categorie == presaved) chemin = std::filesystem::path(std::string(PRESAVED_PATH)+"/motifs");
	else chemin = std::filesystem::path(std::string(LOCAL_PATH)+"/motifs");
	if (std::filesystem::exists(chemin)) {
		chemin /= (nom_motif+".csv");
		if (!std::filesystem::exists(chemin)) {
			std::ofstream out(chemin);
			out.close();
			rapidcsv::Document file(chemin, rapidcsv::LabelParams(-1,-1));
			for (size_t cell(0); cell < vivantes.size(); ++cell) {
				file.SetCell<size_t>(2*cell, 0, X(vivantes[cell])-50);
				file.SetCell<size_t>(2*cell+1, 0, Y(vivantes[cell])-50);
			}
			file.Save();
			return true;
		} else {
			std::cerr << termcolor::cyan << "GameOfLife::save_motif(" << nom_motif << "," << categorie << ") a renvoyé " << termcolor::bold << "FALSE" << termcolor::reset << termcolor::cyan << " car un motif du même nom existait déjà]" << termcolor::reset;
			return false;
		}
	} else {
		#ifdef NON_EXISTING_PATH_WARNINGS
			std::cerr << termcolor::red << "[GameOfLife::save_motif(" << nom_motif << "," << categorie << ") a renvoyé " << termcolor::bold << "FALSE" << termcolor::reset << termcolor::red << " car /" DATA_PATH;
			if (categorie == presaved) std::cerr << "presaved";
			else std::cerr << "local";
			std::cerr << "/motifs n'existe pas]" << termcolor::reset;
		#endif
		return false;
	}
}
bool GameOfLife::save_sim(std::string const& nom_simulation, unsigned int const& duree_sim, FILE_CATEGORY const& categorie) {
	// On verifie dans quel dossier on doit enregistrer la simulation
	std::filesystem::path chemin;
	if (categorie == presaved) chemin = std::filesystem::path(std::string(PRESAVED_PATH)+"/sims");
    else chemin = std::filesystem::path(std::string(LOCAL_PATH)+"/sims");
	#ifdef NON_EXISTING_PATH_WARNINGS
	if (!std::filesystem::exists(chemin)) {
			std::cerr << termcolor::red << "[GameOfLife::save_sim(" << nom_simulation << "," << duree_sim << "," << categorie << ") a renvoyé " << termcolor::bold << "FALSE" << termcolor::reset << termcolor::red << " car " DATA_PATH;
			if (categorie == presaved) std::cerr << "/presaved/sims n'existe pas]";
			else std::cerr << "/local/sims n'existe pas]";
			std::cerr << termcolor::reset;
		return false;
	}
	#endif
	// On verifie si une simulation du même nom existe déjà, et si ce n'est pas le cas on crée la simulation
	chemin /= ("sim-"+nom_simulation+".csv");
	if (!std::filesystem::exists(chemin)) {
		std::ofstream file(chemin);
		file.close();
		rapidcsv::Document res(chemin.string(), rapidcsv::LabelParams(-1,0));

		res.SetCell<unsigned int>(0,0,MAX_LIGNES);
		res.SetCell<unsigned int>(1,0,MAX_COLONNES);
		res.SetRowName(0,"dimensions (lignes x colonnes)");
		res.Save();

		res.SetCell<int>(0,1,duree_sim);
		res.SetRowName(1,"nombre de générations");
		res.Save();

		size_t gen(0);
		do {
			// On ajoute les cellules vivantes à la génétation i
			for (size_t cell(0); cell < vivantes.size(); ++cell) {
				res.SetCell<size_t>(2*cell, 2+gen, X(vivantes[cell])-50);
				res.SetCell<size_t>(2*cell+1, 2+gen, Y(vivantes[cell])-50);
				res.Save();
			}
			res.SetRowName(2+gen,"Génération n°"+std::to_string(gen));
			res.Save();
			this->evolve();
			++gen;
		} while (gen <= duree_sim);
		return true;
	} else {
		std::cerr << termcolor::cyan << "GameOfLife::save_sim(" << nom_simulation << "," << duree_sim << "," << categorie << ") a renvoyé " << termcolor::bold << "FALSE" << termcolor::reset << termcolor::cyan << " car une sim du même nom existait déjà]" << termcolor::reset;
		return false;
	}
}
std::vector<std::string> existing_local_sims() {
    std::vector<std::string> res;
	std::filesystem::path local_sims(std::string(LOCAL_PATH)+"/sims");
	if (std::filesystem::exists(std::filesystem::path(std::string(LOCAL_PATH)+"/sims"))) {
		for (auto const& entry : std::filesystem::directory_iterator(local_sims)) {
			if (entry.path().extension().string() == ".csv" && entry.path().stem().string().substr(0,4) == "sim-") {
				res.push_back(entry.path().stem().string());
			}
		}
	}
    return res;
}
std::vector<std::string> existing_presaved_sims() {
	std::vector<std::string> res;
	std::filesystem::path presaved_sims(std::string(PRESAVED_PATH)+"/sims");
    if (std::filesystem::exists(presaved_sims)) {
		for (auto const& entry : std::filesystem::directory_iterator(presaved_sims)) {
			if (entry.path().extension().string() == ".csv" && entry.path().stem().string().substr(0,4) == "sim-") {
				res.push_back(entry.path().stem().string());
			}
		}
	}
    return res;
}

// Recherche et analyse des structures et composantes connexes de la grille ============================================================

void GameOfLife::dfs(std::unordered_map<coord, size_t, pair_hash>& labels, size_t x, size_t y, size_t label) const {
	if (access(x,y) && labels[{x,y}] == 0) {
		// mark the current cell
        labels[{x,y}] = label;

        // recursively mark the neighbors
        for (int direction = 0; direction < 8; ++direction) {
            dfs(labels, x + dx[direction], y + dy[direction], label);
        }
	}
}
size_t GameOfLife::nbr_CC_rec() const {
	std::unordered_map<coord, size_t, pair_hash> labels;
	for (auto const& cell : vivantes) {
		labels[cell] = 0;
	}
	size_t current_label(0);
	for (auto const& cell : vivantes) {
		if (access(X(cell), Y(cell)) && labels[cell] == 0) {
			dfs(labels, X(cell), Y(cell), ++current_label);
		}
	}
	return current_label;
}
size_t GameOfLife::nbr_CC_ite() const {
	std::unordered_map<coord, size_t, pair_hash> labels;
	for (auto const& cell : vivantes) {
		labels[cell] = 0;
	}
	size_t current_label(0);
	std::queue<coord> q;
	for(auto const& cell : vivantes) {
		if (access(X(cell), Y(cell)) && labels[cell] == 0) {
			++current_label;
			labels[cell] = current_label;
			q.push(cell);
		} else {
			continue;
		}
		while (!q.empty()) {
			coord c(q.front());
			q.pop();
			for (size_t direction(0); direction < 8; ++direction) {
				if (access(X(c)+dx[direction], Y(c)+dy[direction]) && labels[{X(c)+dx[direction], Y(c)+dy[direction]}] == 0) {
					labels[{X(c)+dx[direction], Y(c)+dy[direction]}] = current_label;
					q.push({X(c)+dx[direction], Y(c)+dy[direction]});
				}
			}
		}
	}
	return current_label;
}
std::vector<Motif> GameOfLife::find_CC_rec() const {
	std::unordered_map<coord, size_t, pair_hash> labels;
	for (auto const& cell : vivantes) {
		labels[cell] = 0;
	}
	size_t current_label(0);
	for (auto const& cell : vivantes) {
		if (access(X(cell), Y(cell)) && labels[cell] == 0) {
			dfs(labels, X(cell), Y(cell), ++current_label);
		}
	}
	std::vector<Motif> res(current_label);
	for (size_t i(0); i < current_label ; ++i) {
		for (auto const& pair : labels) {
			if (pair.second == i) {
				res[i].push_back(pair.first);
			}
		}
	}
	return res;
}
std::vector<Motif> GameOfLife::find_CC_ite() const {
	std::unordered_map<coord, size_t, pair_hash> labels;
	for (auto const& cell : vivantes) {
		labels[cell] = 0;
	}
	size_t current_label(0);
	std::queue<coord> q;
	for(auto const& cell : vivantes) {
		if (access(X(cell), Y(cell)) && labels[cell] == 0) {
			++current_label;
			labels[cell] = current_label;
			q.push(cell);
		} else {
			continue;
		}
		while (!q.empty()) {
			coord c(q.front());
			q.pop();
			for (size_t direction(0); direction < 8; ++direction) {
				if (access(X(c)+dx[direction], Y(c)+dy[direction]) && labels[{X(c)+dx[direction], Y(c)+dy[direction]}] == 0) {
					labels[{X(c)+dx[direction], Y(c)+dy[direction]}] = current_label;
					q.push({X(c)+dx[direction], Y(c)+dy[direction]});
				}
			}
		}
	}
	std::vector<Motif> res(current_label);
	for (size_t i(0); i < current_label ; ++i) {
		for (auto const& pair : labels) {
			if (pair.second == i+1) {
				res[i].push_back(pair.first);
			}
		}
	}
	return res;
}

// GameOfLifeView ================================================

// Constructeurs ==================================================================================================================
GameOfLifeView::GameOfLifeView(unsigned int const& lmin, unsigned int const& lmax, unsigned int const& cmin, unsigned int const& cmax)
: GameOfLife(),
	Lmin(lmin%(MAX_LIGNES+1) < lmax%(MAX_LIGNES+1) ? lmin%(MAX_LIGNES+1) : lmax%(MAX_LIGNES+1)),
	Lmax(lmin%(MAX_LIGNES+1) < lmax%(MAX_LIGNES+1) ? lmax%(MAX_LIGNES+1) : lmin%(MAX_LIGNES+1)),
	Cmin(cmin%(MAX_COLONNES+1) < cmax%(MAX_COLONNES+1) ? cmin%(MAX_COLONNES+1) : cmax%(MAX_COLONNES+1)),
	Cmax(cmin%(MAX_COLONNES+1) < cmax%(MAX_COLONNES+1) ? cmax%(MAX_COLONNES+1) : cmin%(MAX_COLONNES+1))
{}

bool GameOfLifeView::save_motif(std::string const& nom_motif, FILE_CATEGORY const& categorie) const {
	std::filesystem::path chemin;
	if (categorie == presaved) chemin = std::filesystem::path(std::string(PRESAVED_PATH)+"/motifs");
	else chemin = std::filesystem::path(std::string(LOCAL_PATH)+"/motifs");
	if (std::filesystem::exists(chemin)) {
		chemin /= (nom_motif+".csv");
		if (!std::filesystem::exists(chemin)) {
			std::ofstream out(chemin);
			out.close();
			rapidcsv::Document file(chemin, rapidcsv::LabelParams(-1,-1));
			for (size_t cell(0); cell < vivantes_visibles.size(); ++cell) {
				file.SetCell<size_t>(2*cell, 0, X(vivantes_visibles[cell])-Lmin-50);
				file.SetCell<size_t>(2*cell+1, 0, Y(vivantes_visibles[cell])-Cmin-50);
			}
			file.Save();
			return true;
		} else {
			std::cerr << termcolor::cyan << "GameOfLifeView::save_motif(" << nom_motif << "," << categorie << ") a renvoyé " << termcolor::bold << "FALSE" << termcolor::reset << termcolor::cyan << " car un motif du même nom existait déjà]" << termcolor::reset;
			return false;
		}
	} else {
		#ifdef NON_EXISTING_PATH_WARNINGS
			std::cerr << termcolor::red << "[GameOfLifeView::save_motif(" << nom_motif << "," << categorie << ") a renvoyé " << termcolor::bold << "FALSE" << termcolor::reset << termcolor::red << " car /" DATA_PATH;
			if (categorie == presaved) std::cerr << "presaved";
			else std::cerr << "local";
			std::cerr << "/motifs n'existe pas]" << termcolor::reset;
		#endif
		return false;
	}
}
// Setters du jeu GameOfLifeView ==========================================================================================================================================================================
GameOfLifeView& GameOfLifeView::add_cell(size_t const& i, size_t const& j) {
	if (i < Lmax-Lmin && j < Cmax-Cmin && !access(i+Lmin+50, j+Cmin+50)) {
		vivantes.push_back({i+Lmin+50,j+Cmin+50});
		vivantes_visibles.push_back({i,j});
		grille[i+Lmin+50][j+Cmin+50] = true;
	} else {
		if (access(i+Lmin+50,j+Cmin+50)) {
			#ifdef ALREADY_MODIFIED_WARNINGS
			std::cerr << "[GameOfLifeView::add_cell(" << i << "," << j << ") n'a rien fait car cell déjà vivante]";
			#endif
		} else {
			#ifdef OVERFLOW_WARNINGS
			std::cerr << "[GameOfLifeView::add_cell(" << i << "," << j << ") n'a rien fait car coords trop grandes]";
			#endif
		}
	}
	return *this;
}
GameOfLifeView& GameOfLifeView::add_cell(coord const& c) {return add_cell(c.first, c.second);}
GameOfLifeView& GameOfLifeView::suppr_cell(size_t const& i, size_t const& j) {
	if (i < Lmax-Lmin && j < Cmax-Cmin && !access(i+Lmin+50, j+Cmin+50)) {
		liste::iterator a_effacer(std::find<liste::iterator, coord>(vivantes.begin(),	vivantes.end(), {i+Lmin+50,j+Cmin+50}));
		if (a_effacer != vivantes.end()) vivantes.erase(a_effacer);
		a_effacer = std::find<liste::iterator, coord>(vivantes_visibles.begin(), vivantes_visibles.end(), {i, j});
		if (a_effacer != vivantes_visibles.end()) vivantes_visibles.erase(a_effacer);
		grille[i+Lmin+50][j+Cmin+50] = false;
	} else {
		if (access(i+Lmin+50, j+Cmin+50)) {
			#ifdef ALREADY_MODIFIED_WARNINGS
			std::cerr << "[GameOfLifeView::suppr_cell(" << i << "," << j << ") n'a rien fait car cell déjà morte]";
			#endif
		} else {
			#ifdef OVERFLOW_WARNINGS
			std::cerr << "[GameOfLifeView::suppr_cell(" << i << "," << j << ") n'a rien fait car coords trop grandes]";
			#endif
		}
	}
	return *this;
}
GameOfLifeView& GameOfLifeView::suppr_cell(coord const& c) {return suppr_cell(c.first, c.second);}
GameOfLifeView& GameOfLifeView::inv_cell(size_t const& i, size_t const& j) {
	if (i < Lmax-Lmin && j < Cmax-Cmin) {
		if (access(i+Lmin+50, j+Cmin+50)) {
			this->suppr_cell(i,j);
		} else {
			this->add_cell(i,j);
		}
	} else {
		#ifdef OVERFLOW_WARNINGS
		std::cerr << "[GameOfLifeView::inv_cell(" << i << "," << j << ") n'a rien fait car coords trop grandes";
		#endif
	}
	return *this;
}
GameOfLifeView& GameOfLifeView::inv_cell(coord const& c) {return inv_cell(c.first, c.second);}

GameOfLifeView& GameOfLifeView::add_motif(Motif const& m) {
	for (liste::const_iterator it(m.cbegin()); it != m.cend(); ++it) this->add_cell(*it);
	return *this;
}
GameOfLifeView& GameOfLifeView::suppr_motif(Motif const& m) {
	for (liste::const_iterator it(m.cbegin()); it != m.cend(); ++it) this->suppr_cell(*it);
	return *this;
}
GameOfLifeView& GameOfLifeView::wipe() {
	for (liste::iterator it(vivantes_visibles.begin()); it != vivantes_visibles.end();) {
		grille[it->first+Lmin+50][it->second+Cmin+50] = false;
		liste::iterator a_effacer(std::find<liste::iterator,coord>(vivantes.begin(), vivantes.end(), {it->first+Lmin+50, it->second+Cmin+50}));
		if (a_effacer != vivantes.end()) vivantes.erase(a_effacer);
		vivantes_visibles.erase(it);
	}
	return *this;
}

// Evolution du jeu ==========================================================================================================================================================================
void GameOfLifeView::verif(size_t const& i, size_t const& j, liste& v, liste& v_visibles) {
    #ifdef OVERFLOW_WARNINGS
	if (i < MAX_LIGNES+100 && j < MAX_COLONNES+100) {
		if(!access(i,j) && std::find<liste::iterator, coord>(v.begin(),v.end(),{i,j}) == v.end()) {
			if (next_state(i,j)) {
				v.push_back({i,j});
				if (Lmin+50 <= i && i < Lmax+50 && Cmin+50 <= j && j < Cmax+50) v_visibles.push_back({i-Lmin-50, j-Cmin-50});
			}
		}
	}
	#else
	if(!access(i,j) && std::find<liste::iterator, coord>(v.begin(),v.end(),{i,j}) == v.end()) {
        if (next_state(i,j) && i<MAX_LIGNES+100 && j<MAX_COLONNES+100) {
			v.push_back({i,j});
			if (Lmin+50 <= i && i < Lmax+50 && Cmin+50 <= j && j < Cmax+50) v_visibles.push_back({i-Lmin-50, j-Cmin-50});
		}
	}
	#endif
}
void GameOfLifeView::evolve() {
	// On crée une nouvelle liste qui contiendra les nouvelles vivantes
	liste nouvelles;
	liste nouvelles_visibles;
	// On check chaque cellule déjà vivante pour voir si elle le reste
	for (liste::iterator it(vivantes.begin()); it != vivantes.end(); ++it) {
		if (next_state(it->first,it->second)) {
			nouvelles.push_back(*it);
			if (Lmin+50 <= it->first && it->first < Lmax+50 && Cmin+50 <= it->second && it->second < Cmax+50) nouvelles_visibles.push_back({it->first-Lmin-50, it->second-Cmin-50});
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
	for (auto const& el : vivantes) grille[el.first][el.second] = false;

	// On rajoute les nouvelles
	for (auto const& el : nouvelles) grille[el.first][el.second] = true;
	// On update la liste des vivantes et le nombre de generatitons
	vivantes = nouvelles;
	vivantes_visibles = nouvelles_visibles;
	++nbr_gen;
}

// Getters GameOfLifeView ==========================================================================================================================================================================
liste const& GameOfLifeView::get_viv() const {return vivantes_visibles;}
unsigned int GameOfLifeView::nbr_cell() const {return vivantes_visibles.size();}
unsigned int GameOfLifeView::nbr_lignes() const {return Lmax - Lmin;}
unsigned int GameOfLifeView::nbr_colonnes() const {return Cmax - Cmin;}
unsigned int GameOfLifeView::get_Lmin() const {return Lmin;}
unsigned int GameOfLifeView::get_Cmin() const {return Cmin;}
unsigned int GameOfLifeView::get_Lmax() const {return Lmax;}
unsigned int GameOfLifeView::get_Cmax() const {return Cmax;}

GameOfLifeView& GameOfLifeView::resize(unsigned int const& lmin, unsigned int const& lmax, unsigned int const& cmin, unsigned int const& cmax) {
	if (lmin < MAX_LIGNES && lmax <= MAX_LIGNES && cmin < MAX_COLONNES && cmax <= MAX_COLONNES) {
		if (lmin != Lmin || lmax != Lmax || cmin != Cmin || cmax != Cmax) {
			Lmin = lmin;
			Lmax = lmax;
			Cmin = cmin;
			Cmax = cmax;
			vivantes_visibles.erase(vivantes_visibles.begin(), vivantes_visibles.end());
			for (auto const& el : vivantes) if (Lmin+50 <= el.first && el.first < Lmax+50 && Cmin+50 <= el.second && el.second < Cmax+50)vivantes_visibles.push_back({el.first-Lmin-50, el.second-Cmin-50});
		}
	} else {
		#ifdef OVERFLOW_WARNINGS
		std::cerr << "[GameOfLifeView::resize(" << lmin << "," << lmax << "," << cmin << "," << cmax << ") n'a rien fait car bords dépassent la grille]";
		#endif
	}
	return *this;
}
GameOfLifeView& GameOfLifeView::translate(int const& l, int const& c) {
	int maxl(MAX_LIGNES-Lmax), maxc(MAX_COLONNES-Cmax), minl(-Lmin), minc(-Cmin);
	if (minl <= l && l <= maxl && minc <= c && c <= maxc) {
		if (l != 0 || c != 0) {
			Lmin += l;
			Lmax += l;
			Cmin += c;
			Cmax += c;
			vivantes_visibles.erase(vivantes_visibles.begin(), vivantes_visibles.end());
			for (auto const& el : vivantes) if (Lmin+50 <= el.first && el.first < Lmax+50 && Cmin+50 <= el.second && el.second < Cmax+50)vivantes_visibles.push_back({el.first-Lmin-50, el.second-Cmin-50});
		}
	} else {
		#ifdef OVERFLOW_WARNINGS
		std::cerr << termcolor::yellow << "[GameOfLifeView::translate(" << l << "," << c << ") n'a rien fait car bords dépassent la grille]" << termcolor::reset;
		#endif
	}
	return *this;
}
GameOfLifeView& GameOfLifeView::zoom(double const& grossissement) {
	if (grossissement >= 0) {
		if (Lmax - Lmin >= 20 && Cmax-Cmin >= 20) this->resize(Lmin+10, Lmax-10, Cmin+10, Cmax-10);
	} else {
		if (Lmin >= 10 && Lmax <= MAX_LIGNES-10 && Cmin >= 10 && Cmax <= MAX_COLONNES-10) this->resize(Lmin-10, Lmax+10, Cmin-10, Cmax+10);
	}
	return *this;
}
GameOfLifeView& GameOfLifeView::zoom(coord const& centre, double grossissement) {
	if (X(centre) < Lmax-Lmin && Y(centre) < Cmax-Cmin && 0<=grossissement && grossissement != 1) {
		unsigned int nbr_l(Lmax-Lmin), nbr_c(Cmax-Cmin);
		nbr_l *= grossissement;
		nbr_c *= grossissement;
		unsigned int Lminprime(X(centre)+Lmin-grossissement*X(centre)), Cminprime(Y(centre)+Cmin-grossissement*Y(centre));
		this->resize(Lminprime, nbr_l+Lminprime, Cminprime, nbr_c+Cminprime);
	}
	return *this;
}
GameOfLifeView& GameOfLifeView::zoom(size_t const& x, size_t const& y, double grossissement) {
	if (x < Lmax-Lmin && y < Cmax-Cmin && 0<=grossissement && grossissement != 1) {
		//auto start(std::chrono::high_resolution_clock::now());
		unsigned int nbr_l(Lmax-Lmin), nbr_c(Cmax-Cmin);
		nbr_l *= grossissement;
		nbr_c *= grossissement;
		unsigned int Lminprime(x+Lmin-grossissement*x), Cminprime(y+Cmin-grossissement*y);
		this->resize(Lminprime, nbr_l+Lminprime, Cminprime, nbr_c+Cminprime);
		//auto stop(std::chrono::high_resolution_clock::now());
		//std:: cerr << termcolor::red << std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start).count() << " ns" << termcolor::reset << std::endl;
	}
	return *this;
}
void GameOfLifeView::print(std::ostream& out, bool avec_grille) const {
	if (avec_grille) {
		out << "\nGénération n° " << nbr_gen << "\n";
		for (size_t i(50); i < Lmin; ++i) {
			// ON affiche la grille normale
			for (size_t j(50); j < MAX_COLONNES+50; ++j) {
				if (grille[i][j]) out << 0;
				else out << ' ';
			}
			out << std::endl;
		}
		for (size_t i(Lmin); i < Lmax ; ++i) {
			for (size_t j(0); j < MAX_COLONNES+50 ; ++j) {
				if (grille[i][j]) out << 0;
				else out << ' ';
			}
			out << "\t\t";
			for (size_t j(0); j < Lmax-Lmin ; ++j) {
				if(grille[i+Lmin][j+Lmin]) out << 0;
				else out << ' ';
			}
			out << "\n";
		}
		for (size_t i(Lmax); i < MAX_LIGNES+50; ++i) {
			// ON affiche la grille normale
			for (size_t j(50); j < MAX_COLONNES+50; ++j) {
				if (grille[i][j]) out << 0;
				else out << ' ';
			}
			out << std::endl;
		}
		out << "\n=================================\n";
	} else {
		out << "\nSans grille | Génération n° " << nbr_gen << "\n";
		for (size_t i(0); i < Lmax-Lmin ; ++i) {
			for (size_t j(0); j < Cmax-Cmin ; ++j) {
				if (grille[i+Lmin+50][j+Cmin+50]) out << "0";
				else out << ' ';
			}
			out << "|\n";
		}
		out << "=================================\n";
	}
}


// SIMULATION ====================================================================================================================================


Simulation::Simulation()
	: grille(nullptr), config_path(std::filesystem::path(std::string(DATA_PATH))), config_file(nullptr) {}

Simulation::~Simulation() {
	delete grille;
	delete config_file;
}

unsigned int Simulation::duree_sim() const {
	if (config_path.extension().string() == ".csv") return config_file->GetCell<unsigned int>(0,"nombre de générations");
	else return 0;
}

Simulation& Simulation::load(std::string const& nom_sim, FILE_CATEGORY const& categorie) {
	if (categorie == presaved) config_path /= "presaved/sims/sim-"+nom_sim+".csv";
	else config_path /= "local/sims/sim-"+nom_sim+".csv";
	if (std::filesystem::exists(config_path)) {
		config_file = new rapidcsv::Document(config_path, rapidcsv::LabelParams(-1,0));
		// on vérifie que le fichier a l'air complet
		if (config_file->GetRowCount() >= 3 && config_file->GetColumnCount() >= 2 && config_file->GetCell<unsigned int>(0,"nombre de générations")+3 == config_file->GetRowCount() && config_file->GetColumnCount()%2 == 0) {
			// On vérifie que les dimensions de la simulation correspondent aux dimensions actuelles
			if (config_file->GetCell<unsigned int>(0,"dimensions (lignes x colonnes)") == MAX_LIGNES && config_file->GetCell<unsigned int>(1,"dimensions (lignes x colonnes)") == MAX_COLONNES) {
				// On ouvre tout ma gueule, c'est open bar
				grille = new GameOfLifeView(0,MAX_LIGNES,0,MAX_COLONNES);
				grille->add_motif(this->get_motif(0));
			} else {
				std::cerr << termcolor::red << "[Simulation::load(" << nom_sim << "," << categorie << ") n'a rien fait car les dimensions de la simulation ne correspondaient pas aux dimensions actuelles de << " << MAX_LIGNES << "x" << MAX_COLONNES << "]" << termcolor::reset;
				config_path = std::filesystem::path(std::string(DATA_PATH));
				//throw INCOMPATIBLE_DIMENSIONS;
			}
		} else {
			std::cerr << termcolor::red << "[Simulation::load(" << nom_sim << "," << categorie << ") n'a rien fait car le fichier de configuration était incomplet]" << termcolor::reset;
			config_path = std::filesystem::path(std::string(DATA_PATH));
			//throw INCOMPLETE_SIM;
		}
	} else {
		#ifdef NON_EXISTING_PATH_WARNINGS
			std::cerr << termcolor::red << "[Simulation::load(" << nom_sim << "," << categorie << ") n'a rien fait car la simulation indiquée n'existe pas]" << termcolor::reset;
		#endif
		config_path = std::filesystem::path(std::string(DATA_PATH));
		//throw NON_EXISTING_SIM;
	}
	return *this;
}

Simulation& Simulation::release() {
	delete grille;
	grille = nullptr;
	config_path = std::filesystem::path(std::string(DATA_PATH));
	delete config_file;
	config_file = nullptr;
	return *this;
}


Motif Simulation::get_motif(unsigned int const& num_gen) const {
	Motif res;
	if (config_path.extension().string() == ".csv" && num_gen <= this->duree_sim()) {

		size_t j(0);
		std::vector<size_t> row;
		while (j < config_file->GetColumnCount()) {
			try {
				row.push_back(config_file->GetCell<size_t>(j,2+num_gen));
				++j;
			}
			catch (...) {
				std::cerr << termcolor::yellow << "[Simulation::get_motif(" << num_gen << ") n'a pas terminé de lire la ligne car il y a eu un problème de coversion à la position " << j << "]" << termcolor::reset;
				break;
			}
		}

		if (row.size()%2 == 0) {
			for (size_t i(0); i < row.size() ; i+=2) {
				res.push_back({row[i], row[i+1]});
			}
		} else {
			std::cerr << termcolor::red << "[Simulation::get_motif(" << num_gen << ") n'a rien fait car le fichier de configuration était incomplet (pas un nombre pair de coordonnées sur la ligne)]" << termcolor::reset;
			//throw INCOMPLETE_SIM;
		}
	}
	return res;
}

bool Simulation::finie() const {return grille->get_nbr_gen() == this->duree_sim();}

bool Simulation::evolve() {
	if (grille->get_nbr_gen() < this->duree_sim()) {
		grille->GameOfLife::wipe();
		grille->GameOfLife::add_motif(get_motif(++grille->get_nbr_gen()));
		return true;
	} else {
		return false;
	}
}

liste const& Simulation::get_viv() const {return grille->get_viv();}

void Simulation::print(std::ostream& out, bool avec_grille) const {grille->print(out, avec_grille);}

Simulation& Simulation::translate(int const& l, int const& c) {
	grille->translate(l,c);
	return *this;
}
Simulation& Simulation::resize(unsigned int const& lmin, unsigned int const& lmax, unsigned int const& cmin, unsigned int const& cmax) {
	grille->resize(lmin, lmax, cmin, cmax);
	return *this;
}

//std::vector<Motif> composants_connexes(GameOfLife const& jeu) {
//	liste vivantes(jeu.get_viv());
//	std::vector<Motif> res;
//	for (auto const& cell : vivantes) {
//		if (!res.empty()) {
//			std::vector<Motif>::iterator it(res.begin());
//			bool test(true);
//			while (it != res.end() && test) {
//				test = !it->a_pour_voisin(cell);
//				++it;
//			}
//			if (it != res.end()) it->push_back(cell);
//			else res.push_back(Motif({cell}));
//		} else {
//			res.push_back(Motif({cell}));
//		}
//	}
//	return res;
//}

//long int find(std::vector<long int> const& L, long int const& e) {
//	long int r(e);
//	while (L[r] != r) r = L[r];
//	return r;
//}
//long int unionn(std::vector<long int>& L, long int const& e1, long int const& e2) {
//	if (e1 < e2) {
//		L[e2] = e1;
//		return e1;
//	} else {
//		L[e1] = e2;
//		return e2;
//	}
//}
//bool is_adjacent(coord const& p1, coord const& p2) {return dist(X(p1),X(p2)) <= 1 && dist(Y(p1),Y(p2)) <= 1;}
//bool is_far_enough(coord const& p1, coord const& p2) {return Y(p1)-Y(p2) > 1;}
//void init_features(size_t const& label, coord const& pixel);
//void accumulate_features(size_t const& label, coord const& pixel);
//std::vector<long int> GameOfLife::sparseCLL() {
//	// first scan: pixel association
//	long int start_j(0), n(vivantes.size()), tpr(0);
//	std::vector<long int> L(n);
//	for (long int i(0); i < n ; ++i) {
//		L[i] = i;
//		tpr = i;
//		for (long int j(start_j); j < i-1 ; ++j) {
//			if (is_adjacent(vivantes[i], vivantes[j])) {
//				tpr = unionn(L, tpr, find(L, j));
//			} else if (is_far_enough(vivantes[i], vivantes[j])) {
//				++start_j;
//			}
//		}
//	}

//	// Second scan:  transitive closure and analysis
//	long int labels(0), l(0);
//	for (long int i(0); i < n-1 ; ++i) {
//		if (L[i] == i) {
//			++labels;
//			l = labels;
//			//init_features(l, vivantes[i]);
//		} else {
//			l = L[L[i]];
//			//accumulate_features(l, vivantes[i]);
//		}
//		L[i] = l;
//	}
//	L.push_back(labels);
//	return L;
//}