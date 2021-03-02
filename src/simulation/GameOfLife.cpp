#include "GameOfLife.hpp"
#include "Motif.hpp"
#include "rapidcsv.h"
#include "termcolor.hpp"

#include <queue>
#include <string>
#include <vector>
#include <unordered_map>
#include <array>
#include <functional>
#include <algorithm>
#include <filesystem> // pour trouver les simulations existantes
#include <chrono>

template<class T1, class T2>
std::size_t pair_hash::operator()(std::pair<T1,T2> const& p) const {
	if (p.first == p.second) {
		size_t first = p.first, second = (p.first << 2)|(p.first >> (8*sizeof(size_t)-2));
		return std::hash<T1>()(first) ^ std::hash<T2>()(second);
	} else {
		return std::hash<T1>()(p.first) ^ std::hash<T2>()(p.second);
	}
}

// Constructeurs ========================================================================================
GameOfLife::GameOfLife() : generation_(0) {
	for (size_t i(0); i < MAX_LIGNES+100; ++i) {
		for (size_t j(0); j < MAX_COLONNES+100; ++j) grille_[i][j] = false;
	}
}
GameOfLife::GameOfLife(Motif const& a_marquer) : generation_(0) {
	for (size_t i(0); i < MAX_LIGNES+100; ++i) {
		for (size_t j(0); j < MAX_COLONNES+100; ++j) grille_[i][j] = false;
	}
	for (auto it(a_marquer.cbegin()); it != a_marquer.cend(); ++it) {
		if (it->first < MAX_LIGNES+100 && it->second < MAX_COLONNES+100) {
			grille_[(it->first)+50][(it->second)+50] = true;
			vivantes_.push_back({it->first, it->second});
		}
	}
}

// Getters & Setters ========================================================================================
bool GameOfLife::at(size_t const& i, size_t const& j) const {
	if (i < MAX_LIGNES+100 && j < MAX_COLONNES+100) return grille_[i][j];
	else {
#ifdef OVERFLOW_WARNINGS
		std::cerr << termcolor::yellow << "[GameOfLife::at(" << i << "," << j << ") a renvoyé false car les coordonées étaient trop grandes]" << termcolor::reset;
#endif
		return false;
	}
}
Liste const& GameOfLife::vivantes() const {return vivantes_;}

[[maybe_unused]] std::array<std::array<bool,MAX_LIGNES+100>,MAX_COLONNES+100> const& GameOfLife::grille() const {return grille_;}

// Setters ================================================
GameOfLife& GameOfLife::addCell(size_t const& i, size_t const& j) {
#ifdef OVERFLOW_WARNINGS
	if (i < MAX_LIGNES && j < MAX_COLONNES) {
		if (!at(i + 50, j + 50)) {
			vivantes_.push_back({i+50, j+50});
			grille_[i+50][j+50] = true;
		}
	} else {
		std::cerr << termcolor::yellow << "[GameOfLife::addCell(" << i << "," << j << ") n'a rien fait car les coordonnées étaient trop grandes]" << termcolor::reset;
	}
#else
	if (!at(i+50,j+50) && i < MAX_LIGNES && j < MAX_COLONNES) {
		vivantes_.push_back({i+50, j+50});
		grille_[i+50][j+50] = true;
	}
#endif
	return *this;
}

GameOfLife& GameOfLife::deleteCell(size_t const& i, size_t const& j) {
	if (at(i + 50, j + 50)) {
		grille_[i+50][j+50] = false;
		auto a_effacer(std::find<Liste::iterator, Coord>(vivantes_.begin(), vivantes_.end(), {i+50, j+50}));
		if (a_effacer != vivantes_.end()) vivantes_.erase(a_effacer);
	} else {
#ifdef OVERFLOW_WARNINGS
		std::cerr << termcolor::yellow << "[GameOfLife::deleteCell(" << i << "," << j << ") n'a rien fait car les coordonnées étaient trop grandes]" << termcolor::reset;
#endif
	}
	return *this;
}

GameOfLife& GameOfLife::inverseCell(size_t const& i, size_t const& j) {
#ifdef OVERFLOW_WARNINGS
	if (i < MAX_LIGNES && j < MAX_COLONNES) {
		if (at(i + 50, j + 50)) {
			grille_[i+50][j+50] = false;
			auto a_effacer(std::find<Liste::iterator, Coord>(vivantes_.begin(), vivantes_.end(), {i+50, j+50}));
			if (a_effacer != vivantes_.end()) vivantes_.erase(a_effacer);
		} else {
			grille_[i+50][j+50] = true;
			vivantes_.push_back({i+50, j+50});
		}
	} else {
		std::cerr << termcolor::yellow << "[GameOfLife::inverseCell(" << i << "," << j << ") n'a rien fait car les coordonnées étaient trop grandes]" << termcolor::reset;
	}
#else
	if (at(i+50, j+50)) {
		grille_[i+50][j+50] = false;
		Liste::iterator a_effacer(std::find<Liste::iterator, Coord>(vivantes_.begin(),	vivantes_.end(), {i+50, j+50}));
		if (a_effacer != vivantes_.end()) vivantes_.erase(a_effacer);
	} else {
		if((i < MAX_LIGNES) && (j < MAX_COLONNES)) {
			grille_[i+50][j+50] = true;
			vivantes_.push_back({i+50,j+50});
		}
	}
#endif
	return *this;
}

GameOfLife& GameOfLife::addMotif(Motif const& motif) {
	for (auto it(motif.cbegin()); it != motif.cend(); ++it) addCell(it->first, it->second);
	return *this;
}
GameOfLife& GameOfLife::deleteMotif(Motif const& motif) {
	for (auto it(motif.cbegin()); it != motif.cend(); ++it) deleteCell(it->first, it->second);
	return *this;
}
GameOfLife& GameOfLife::wipe() {
	for (auto it(vivantes_.begin()); it != vivantes_.end();) {
		grille_[it->first][it->second] = false;
		vivantes_.erase(it);
	}
	return *this;
}

unsigned int& GameOfLife::generation() {return generation_;}
// Affichage ========================================================================================
void GameOfLife::print(std::ostream& out) const {
	out << "Génération n° " << generation_ << "\n";
	for (size_t i(50); i < MAX_LIGNES+50; ++i) {
		for (size_t j(50); j < MAX_COLONNES+50; ++j) {
			if (grille_[i][j]) out << 0;
			else out << ' ';
		}
		out << std::endl;
	}
	out << "\n=================================\n";
}

// Evolution de la simulation  ========================================================================================
bool GameOfLife::next_state(size_t const& i, size_t const& j) const {
	short int S(0);
	if (at(i - 1, j - 1)) ++S;
	if (at(i - 1, j)) ++S;
	if (at(i - 1, j + 1)) ++S;
	if (at(i, j - 1)) ++S;
	if (at(i, j + 1)) ++S;
	if (at(i + 1, j - 1)) ++S;
	if (at(i + 1, j)) ++S;
	if (at(i + 1, j + 1)) ++S;
	return (S==3) || (at(i, j) && S == 2);
}
void GameOfLife::verif(size_t const& i, size_t const& j, Liste& v) {
#ifdef OVERFLOW_WARNINGS
	if (i < MAX_LIGNES+100 && j < MAX_COLONNES+100) {
		if(!at(i, j) && std::find<Liste::iterator, Coord>(v.begin(), v.end(), {i, j}) == v.end()) {
			if (next_state(i,j)) {
				v.push_back({i,j});
			}
		}
	} else {
		std::cerr << termcolor::yellow << "[verif(" << i << "," << j << ",vivantes_) n'a rien fait car les coords étaient trop grandes]" << termcolor::reset;
	}
#else
	if(!at(i,j) && std::find<Liste::iterator, Coord>(v.begin(),v.end(),{i,j}) == v.end()) {
		if (next_state(i,j) && i < MAX_LIGNES+100 && j < MAX_COLONNES+100) {
			v.push_back({i,j});
		}
	}
#endif
}
void GameOfLife::evolve() {
	// On crée une nouvelle Liste qui contiendra les nouvelles vivantes_
	Liste nouvelles;
	// On check chaque cellule déjà vivante pour voir si elle le reste
	for (Liste::iterator it(vivantes_.begin()); it != vivantes_.end(); ++it) {
		if (next_state(it->first,it->second)) {
			nouvelles.push_back(*it);
		}
		
		// On vérifie dans ses voisines lesquelles étaient mortes et deviendraient potentiellement vivantes_
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
	for (auto const& el : vivantes_) grille_[el.first][el.second] = false;
	
	// On rajoute les nouvelles
	for (auto const& el : nouvelles) grille_[el.first][el.second] = true;
	// On update la Liste des vivantes_ et le nombre de generatitons
	vivantes_ = nouvelles;
	++generation_;
}



// Enregistrement de motifs et simulations ========================================================================================
bool GameOfLife::saveMotif(std::string const& nomMotif, FILE_CATEGORY const& categorie) const {
	std::filesystem::path chemin;
	if (categorie == presaved) chemin = std::filesystem::path(std::string(PRESAVED_PATH)+"/motifs");
	else chemin = std::filesystem::path(std::string(LOCAL_PATH)+"/motifs");
	if (std::filesystem::exists(chemin)) {
		chemin /= (nomMotif+".csv");
		if (!std::filesystem::exists(chemin)) {
			std::ofstream out(chemin);
			out.close();
			rapidcsv::Document file(chemin, rapidcsv::LabelParams(-1,-1));
			for (size_t cell(0); cell < vivantes_.size(); ++cell) {
				file.SetCell<size_t>(2*cell, 0, X(vivantes_[cell])-50);
				file.SetCell<size_t>(2*cell+1, 0, Y(vivantes_[cell])-50);
			}
			file.Save();
			return true;
		} else {
			std::cerr << termcolor::cyan << "GameOfLife::saveMotif(" << nomMotif << "," << categorie << ") a renvoyé " << termcolor::bold << "FALSE" << termcolor::reset << termcolor::cyan << " car un motif du même nom existait déjà]" << termcolor::reset;
			return false;
		}
	} else {
#ifdef NON_EXISTING_PATH_WARNINGS
		std::cerr << termcolor::red << "[GameOfLife::saveMotif(" << nomMotif << "," << categorie << ") a renvoyé " << termcolor::bold << "FALSE" << termcolor::reset << termcolor::red << " car /" DATA_PATH;
		if (categorie == presaved) std::cerr << "presaved";
		else std::cerr << "local";
		std::cerr << "/motifs n'existe pas]" << termcolor::reset;
#endif
		return false;
	}
}
bool GameOfLife::saveSim(std::string const& nom, unsigned int const& duree, FILE_CATEGORY const& categorie) {
	// On verifie dans quel dossier on doit enregistrer la simulation
	std::filesystem::path chemin;
	chemin = std::filesystem::path(std::string(categorie == presaved ? PRESAVED_PATH : LOCAL_PATH) + "/sims");
#ifdef NON_EXISTING_PATH_WARNINGS
	if (!std::filesystem::exists(chemin)) {
		std::cerr << termcolor::red << "[GameOfLife::saveSim(" << nom << "," << duree << "," << categorie << ") a renvoyé " << termcolor::bold << "FALSE" << termcolor::reset << termcolor::red << " car " DATA_PATH;
		if (categorie == presaved) std::cerr << "/presaved/sims n'existe pas]";
		else std::cerr << "/local/sims n'existe pas]";
		std::cerr << termcolor::reset;
		return false;
	}
#endif
	// On verifie si une simulation du même nom existe déjà, et si ce n'est pas le cas on crée la simulation
	chemin /= ("sim-"+nom+".csv");
	if (!std::filesystem::exists(chemin)) {
		std::ofstream file(chemin);
		file.close();
		rapidcsv::Document res(chemin.string(), rapidcsv::LabelParams(-1,0));
		
		res.SetCell<unsigned int>(0,0,MAX_LIGNES);
		res.SetCell<unsigned int>(1,0,MAX_COLONNES);
		res.SetRowName(0,"dimensions (lignes x colonnes)");
		res.Save();
		
		res.SetCell<int>(0, 1, duree);
		res.SetRowName(1,"nombre de générations");
		res.Save();
		
		size_t gen(0);
		do {
			// On ajoute les cellules vivantes_ à la génétation i
			for (size_t cell(0); cell < vivantes_.size(); ++cell) {
				res.SetCell<size_t>(2*cell, 2+gen, X(vivantes_[cell])-50);
				res.SetCell<size_t>(2*cell+1, 2+gen, Y(vivantes_[cell])-50);
				res.Save();
			}
			res.SetRowName(2+gen,"Génération n°"+std::to_string(gen));
			res.Save();
			this->evolve();
			++gen;
		} while (gen <= duree);
		return true;
	} else {
		std::cerr << termcolor::cyan << "GameOfLife::saveSim(" << nom << "," << duree << "," << categorie << ") a renvoyé " << termcolor::bold << "FALSE" << termcolor::reset << termcolor::cyan << " car une sim du même nom existait déjà]" << termcolor::reset;
		return false;
	}
}
std::vector<std::string> existingLocalSims() {
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
std::vector<std::string> existingPresavedSims() {
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

// Recherche et analyse des structures et composantes connexes de la grille_ ============================================================
unsigned int GameOfLife::nbCells() const {return vivantes_.size();}

void GameOfLife::dfs(std::unordered_map<Coord, size_t, pair_hash>& labels, size_t x, size_t y, size_t label) const {
	if (at(x, y) && labels[{x, y}] == 0) {
		// mark the current cell
		labels[{x,y}] = label;
		
		// recursively mark the neighbors
		for (int direction = 0; direction < 8; ++direction) {
			dfs(labels, x + dx[direction], y + dy[direction], label);
		}
	}
}
size_t GameOfLife::nbr_CC_rec() const {
	std::unordered_map<Coord, size_t, pair_hash> labels;
	for (auto const& cell : vivantes_) {
		labels[cell] = 0;
	}
	size_t current_label(0);
	for (auto const& cell : vivantes_) {
		if (at(X(cell), Y(cell)) && labels[cell] == 0) {
			dfs(labels, X(cell), Y(cell), ++current_label);
		}
	}
	return current_label;
}
size_t GameOfLife::nbr_CC_ite() const {
	std::unordered_map<Coord, size_t, pair_hash> labels;
	for (auto const& cell : vivantes_) {
		labels[cell] = 0;
	}
	size_t current_label(0);
	std::queue<Coord> q;
	for(auto const& cell : vivantes_) {
		if (at(X(cell), Y(cell)) && labels[cell] == 0) {
			++current_label;
			labels[cell] = current_label;
			q.push(cell);
		} else {
			continue;
		}
		while (!q.empty()) {
			Coord c(q.front());
			q.pop();
			for (size_t direction(0); direction < 8; ++direction) {
				if (at(X(c) + dx[direction], Y(c) + dy[direction]) && labels[{X(c)+dx[direction], Y(c)+dy[direction]}] == 0) {
					labels[{X(c)+dx[direction], Y(c)+dy[direction]}] = current_label;
					q.push({X(c)+dx[direction], Y(c)+dy[direction]});
				}
			}
		}
	}
	return current_label;
}
std::vector<Motif> GameOfLife::find_CC_rec() const {
	std::unordered_map<Coord, size_t, pair_hash> labels;
	for (auto const& cell : vivantes_) {
		labels[cell] = 0;
	}
	size_t current_label(0);
	for (auto const& cell : vivantes_) {
		if (at(X(cell), Y(cell)) && labels[cell] == 0) {
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
	std::unordered_map<Coord, size_t, pair_hash> labels;
	for (auto const& cell : vivantes_) {
		labels[cell] = 0;
	}
	size_t current_label(0);
	std::queue<Coord> q;
	for(auto const& cell : vivantes_) {
		if (at(X(cell), Y(cell)) && labels[cell] == 0) {
			++current_label;
			labels[cell] = current_label;
			q.push(cell);
		} else {
			continue;
		}
		while (!q.empty()) {
			Coord c(q.front());
			q.pop();
			for (size_t direction(0); direction < 8; ++direction) {
				if (at(X(c) + dx[direction], Y(c) + dy[direction]) && labels[{X(c)+dx[direction], Y(c)+dy[direction]}] == 0) {
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
		  Lmin_(lmin%(MAX_LIGNES+1) < lmax%(MAX_LIGNES+1) ? lmin%(MAX_LIGNES+1) : lmax%(MAX_LIGNES+1)),
		  Lmax_(lmin%(MAX_LIGNES+1) < lmax%(MAX_LIGNES+1) ? lmax%(MAX_LIGNES+1) : lmin%(MAX_LIGNES+1)),
		  Cmin_(cmin%(MAX_COLONNES+1) < cmax%(MAX_COLONNES+1) ? cmin%(MAX_COLONNES+1) : cmax%(MAX_COLONNES+1)),
		  Cmax_(cmin%(MAX_COLONNES+1) < cmax%(MAX_COLONNES+1) ? cmax%(MAX_COLONNES+1) : cmin%(MAX_COLONNES+1))
{}

bool GameOfLifeView::saveMotif(std::string const& nomMotif, FILE_CATEGORY const& categorie) const {
	std::filesystem::path chemin;
	if (categorie == presaved) chemin = std::filesystem::path(std::string(PRESAVED_PATH)+"/motifs");
	else chemin = std::filesystem::path(std::string(LOCAL_PATH)+"/motifs");
	if (std::filesystem::exists(chemin)) {
		chemin /= (nomMotif+".csv");
		if (!std::filesystem::exists(chemin)) {
			std::ofstream out(chemin);
			out.close();
			rapidcsv::Document file(chemin, rapidcsv::LabelParams(-1,-1));
			for (size_t cell(0); cell < vivantes_visibles.size(); ++cell) {
				file.SetCell<size_t>(2*cell, 0, X(vivantes_visibles[cell])-Lmin_-50);
				file.SetCell<size_t>(2*cell+1, 0, Y(vivantes_visibles[cell])-Cmin_-50);
			}
			file.Save();
			return true;
		} else {
			std::cerr << termcolor::cyan << "GameOfLifeView::saveMotif(" << nomMotif << "," << categorie << ") a renvoyé " << termcolor::bold << "FALSE" << termcolor::reset << termcolor::cyan << " car un motif du même nom existait déjà]" << termcolor::reset;
			return false;
		}
	} else {
#ifdef NON_EXISTING_PATH_WARNINGS
		std::cerr << termcolor::red << "[GameOfLifeView::saveMotif(" << nomMotif << "," << categorie << ") a renvoyé " << termcolor::bold << "FALSE" << termcolor::reset << termcolor::red << " car /" DATA_PATH;
		if (categorie == presaved) std::cerr << "presaved";
		else std::cerr << "local";
		std::cerr << "/motifs n'existe pas]" << termcolor::reset;
#endif
		return false;
	}
}
// Setters du jeu GameOfLifeView ==========================================================================================================================================================================
GameOfLifeView& GameOfLifeView::addCell(size_t const& i, size_t const& j) {
	if (i < Lmax_-Lmin_ && j < Cmax_-Cmin_ && !at(i + Lmin_ + 50, j + Cmin_ + 50)) {
		vivantes_.push_back({i+Lmin_+50, j+Cmin_+50});
		vivantes_visibles.push_back({i,j});
		grille_[i+Lmin_+50][j+Cmin_+50] = true;
	} else {
		if (at(i + Lmin_ + 50, j + Cmin_ + 50)) {
#ifdef ALREADY_MODIFIED_WARNINGS
			std::cerr << "[GameOfLifeView::add_cell(" << i << "," << j << ") n'a rien fait car cell déjà vivante]";
#endif
		} else {
#ifdef OVERFLOW_WARNINGS
			std::cerr << "[GameOfLifeView::addCell(" << i << "," << j << ") n'a rien fait car coords trop grandes]";
#endif
		}
	}
	return *this;
}

GameOfLifeView& GameOfLifeView::deleteCell(size_t const& i, size_t const& j) {
	if (i < Lmax_-Lmin_ && j < Cmax_-Cmin_ && at(i + Lmin_ + 50, j + Cmin_ + 50)) {
		Liste::iterator a_effacer(std::find<Liste::iterator, Coord>(vivantes_.begin(), vivantes_.end(), {i+Lmin_+50, j+Cmin_+50}));
		if (a_effacer != vivantes_.end()) vivantes_.erase(a_effacer);
		a_effacer = std::find<Liste::iterator, Coord>(vivantes_visibles.begin(), vivantes_visibles.end(), {i, j});
		if (a_effacer != vivantes_visibles.end()) vivantes_visibles.erase(a_effacer);
		grille_[i+Lmin_+50][j+Cmin_+50] = false;
	} else {
		if (!at(i + Lmin_ + 50, j + Cmin_ + 50)) {
#ifdef ALREADY_MODIFIED_WARNINGS
			std::cerr << "[GameOfLifeView::suppr_cell(" << i << "," << j << ") n'a rien fait car cell déjà morte]";
#endif
		} else {
#ifdef OVERFLOW_WARNINGS
			std::cerr << "[GameOfLifeView::deleteCell(" << i << "," << j << ") n'a rien fait car coords trop grandes]";
#endif
		}
	}
	return *this;
}

GameOfLifeView& GameOfLifeView::inverseCell(size_t const& i, size_t const& j) {
	if (i < Lmax_-Lmin_ && j < Cmax_-Cmin_) {
		if (this->at(i + Lmin_ + 50, j + Cmin_ + 50)) {
			this->deleteCell(i, j);
		} else {
			this->addCell(i, j);
		}
	} else {
#ifdef OVERFLOW_WARNINGS
		std::cerr << "[GameOfLifeView::inverseCell(" << i << "," << j << ") n'a rien fait car coords trop grandes";
#endif
	}
	return *this;
}

GameOfLifeView& GameOfLifeView::addMotif(Motif const& motif) {
	for (auto it(motif.cbegin()); it != motif.cend(); ++it) this->addCell(it->first, it->second);
	return *this;
}
GameOfLifeView& GameOfLifeView::deleteMotif(Motif const& motif) {
	for (auto it(motif.cbegin()); it != motif.cend(); ++it) this->deleteCell(it->first, it->second);
	return *this;
}
GameOfLifeView& GameOfLifeView::wipe() {
	for (auto it(vivantes_visibles.begin()); it != vivantes_visibles.end();) {
		grille_[it->first+Lmin_+50][it->second+Cmin_+50] = false;
		auto a_effacer(std::find<Liste::iterator,Coord>(vivantes_.begin(), vivantes_.end(), {it->first+Lmin_+50, it->second+Cmin_+50}));
		if (a_effacer != vivantes_.end()) vivantes_.erase(a_effacer);
		vivantes_visibles.erase(it);
	}
	return *this;
}

// Evolution du jeu ==========================================================================================================================================================================
void GameOfLifeView::verif(size_t const& i, size_t const& j, Liste& v, Liste& v_visibles) {
#ifdef OVERFLOW_WARNINGS
	if (i < MAX_LIGNES+100 && j < MAX_COLONNES+100) {
		if(!this->at(i, j) && std::find<Liste::iterator, Coord>(v.begin(), v.end(), {i, j}) == v.end()) {
			if (next_state(i,j)) {
				v.push_back({i,j});
				if (Lmin_+50 <= i && i < Lmax_+50 && Cmin_+50 <= j && j < Cmax_+50) v_visibles.push_back({i-Lmin_-50, j-Cmin_-50});
			}
		}
	}
#else
	if(!at(i,j) && std::find<Liste::iterator, Coord>(v.begin(),v.end(),{i,j}) == v.end()) {
        if (next_state(i,j) && i<MAX_LIGNES+100 && j<MAX_COLONNES+100) {
			v.push_back({i,j});
			if (Lmin_+50 <= i && i < Lmax_+50 && Cmin_+50 <= j && j < Cmax_+50) v_visibles.push_back({i-Lmin_-50, j-Cmin_-50});
		}
	}
#endif
}
void GameOfLifeView::evolve() {
	// On crée une nouvelle Liste qui contiendra les nouvelles vivantes_
	Liste nouvelles;
	Liste nouvelles_visibles;
	// On check chaque cellule déjà vivante pour voir si elle le reste
	for (Liste::iterator it(vivantes_.begin()); it != vivantes_.end(); ++it) {
		if (next_state(it->first,it->second)) {
			nouvelles.push_back(*it);
			if (Lmin_+50 <= it->first && it->first < Lmax_+50 && Cmin_+50 <= it->second && it->second < Cmax_+50) nouvelles_visibles.push_back({it->first-Lmin_-50, it->second-Cmin_-50});
		}
		
		// On vérifie dans ses voisines lesquelles étaient mortes et deviendraient potentiellement vivantes_
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
	for (auto const& el : vivantes_) grille_[el.first][el.second] = false;
	
	// On rajoute les nouvelles
	for (auto const& el : nouvelles) grille_[el.first][el.second] = true;
	// On update la Liste des vivantes_ et le nombre de generatitons
	vivantes_ = nouvelles;
	vivantes_visibles = nouvelles_visibles;
	++generation_;
}

// Getters GameOfLifeView ==========================================================================================================================================================================
Liste const& GameOfLifeView::vivantes() const {return vivantes_visibles;}
unsigned int GameOfLifeView::nbCells() const {return vivantes_visibles.size();}

unsigned int GameOfLifeView::Lmin() const {return Lmin_;}
unsigned int GameOfLifeView::Cmin() const {return Cmin_;}
unsigned int GameOfLifeView::Lmax() const {return Lmax_;}
unsigned int GameOfLifeView::Cmax() const {return Cmax_;}

GameOfLifeView& GameOfLifeView::resize(unsigned int const& lmin, unsigned int const& lmax, unsigned int const& cmin, unsigned int const& cmax) {
	if (lmin < MAX_LIGNES && lmax <= MAX_LIGNES && cmin < MAX_COLONNES && cmax <= MAX_COLONNES) {
		if (lmin != Lmin_ || lmax != Lmax_ || cmin != Cmin_ || cmax != Cmax_) {
			Lmin_ = lmin;
			Lmax_ = lmax;
			Cmin_ = cmin;
			Cmax_ = cmax;
			vivantes_visibles.erase(vivantes_visibles.begin(), vivantes_visibles.end());
			for (auto const& el : vivantes_) if (Lmin_+50 <= el.first && el.first < Lmax_+50 && Cmin_+50 <= el.second && el.second < Cmax_+50)vivantes_visibles.push_back({el.first-Lmin_-50, el.second-Cmin_-50});
		}
	} else {
#ifdef OVERFLOW_WARNINGS
		std::cerr << "[GameOfLifeView::resize(" << lmin << "," << lmax << "," << cmin << "," << cmax << ") n'a rien fait car bords dépassent la grille_]";
#endif
	}
	return *this;
}
GameOfLifeView& GameOfLifeView::translate(int const& l, int const& c) {
	int maxl(MAX_LIGNES-Lmax_), maxc(MAX_COLONNES-Cmax_), minl(-Lmin_), minc(-Cmin_);
	if (minl <= l && l <= maxl && minc <= c && c <= maxc) {
		if (l != 0 || c != 0) {
			Lmin_ += l;
			Lmax_ += l;
			Cmin_ += c;
			Cmax_ += c;
			vivantes_visibles.erase(vivantes_visibles.begin(), vivantes_visibles.end());
			for (auto const& el : vivantes_) if (Lmin_+50 <= el.first && el.first < Lmax_+50 && Cmin_+50 <= el.second && el.second < Cmax_+50)vivantes_visibles.push_back({el.first-Lmin_-50, el.second-Cmin_-50});
		}
	} else {
#ifdef OVERFLOW_WARNINGS
		std::cerr << termcolor::yellow << "[GameOfLifeView::translate(" << l << "," << c << ") n'a rien fait car bords dépassent la grille_]" << termcolor::reset;
#endif
	}
	return *this;
}
GameOfLifeView& GameOfLifeView::zoom(double const& grossissement) {
	if (grossissement >= 0) {
		if (Lmax_ - Lmin_ >= 20 && Cmax_-Cmin_ >= 20) this->resize(Lmin_+10, Lmax_-10, Cmin_+10, Cmax_-10);
	} else {
		if (Lmin_ >= 10 && Lmax_ <= MAX_LIGNES-10 && Cmin_ >= 10 && Cmax_ <= MAX_COLONNES-10) this->resize(Lmin_-10, Lmax_+10, Cmin_-10, Cmax_+10);
	}
	return *this;
}

GameOfLifeView& GameOfLifeView::zoom(size_t const& x, size_t const& y, double grossissement) {
	if (x < Lmax_-Lmin_ && y < Cmax_-Cmin_ && 0<=grossissement && grossissement != 1) {
		unsigned int nbr_l(Lmax_-Lmin_), nbr_c(Cmax_-Cmin_);
		nbr_l *= grossissement;
		nbr_c *= grossissement;
		unsigned int Lminprime(x+Lmin_-grossissement*x), Cminprime(y+Cmin_-grossissement*y);
		this->resize(Lminprime, nbr_l+Lminprime, Cminprime, nbr_c+Cminprime);
	}
	return *this;
}
void GameOfLifeView::print(std::ostream& out, bool avec_grille) const {
	if (avec_grille) {
		out << "\nGénération n° " << generation_ << "\n";
		for (size_t i(50); i < Lmin_; ++i) {
			// ON affiche la grille_ normale
			for (size_t j(50); j < MAX_COLONNES+50; ++j) {
				if (grille_[i][j]) out << 0;
				else out << ' ';
			}
			out << std::endl;
		}
		for (size_t i(Lmin_); i < Lmax_ ; ++i) {
			for (size_t j(0); j < MAX_COLONNES+50 ; ++j) {
				if (grille_[i][j]) out << 0;
				else out << ' ';
			}
			out << "\t\t";
			for (size_t j(0); j < Lmax_-Lmin_ ; ++j) {
				if(grille_[i+Lmin_][j+Lmin_]) out << 0;
				else out << ' ';
			}
			out << "\n";
		}
		for (size_t i(Lmax_); i < MAX_LIGNES+50; ++i) {
			// ON affiche la grille_ normale
			for (size_t j(50); j < MAX_COLONNES+50; ++j) {
				if (grille_[i][j]) out << 0;
				else out << ' ';
			}
			out << std::endl;
		}
		out << "\n=================================\n";
	} else {
		out << "\nSans grille_ | Génération n° " << generation_ << "\n";
		for (size_t i(0); i < Lmax_-Lmin_ ; ++i) {
			for (size_t j(0); j < Cmax_-Cmin_ ; ++j) {
				if (grille_[i+Lmin_+50][j+Cmin_+50]) out << "0";
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

unsigned int Simulation::duree() const {
	if (config_path.extension().string() == ".csv") {
		try {
			return config_file->GetCell<unsigned int>(0,"nombre de générations");
		}
		catch (...) {
			std::cerr << termcolor::red << "[duree() a renvoyé 0 car le fichier de configuration était incomplet]" << termcolor::reset;
#ifdef THROW_SIMULATION_ERRORS
			throw INCOMPLETE_SIM;
#endif
		}
	}
	return 0;
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
				grille->addMotif(this->motif(0));
			} else {
				std::cerr << termcolor::red << "[Simulation::load(" << nom_sim << "," << categorie << ") n'a rien fait car les dimensions de la simulation ne correspondaient pas aux dimensions actuelles de << " << MAX_LIGNES << "x" << MAX_COLONNES << "]" << termcolor::reset;
				config_path = std::filesystem::path(std::string(DATA_PATH));
#ifdef THROW_SIMULATION_ERRORS
				throw INCOMPATIBLE_DIMENSIONS;
#endif
			}
		} else {
			std::cerr << termcolor::red << "[Simulation::load(" << nom_sim << "," << categorie << ") n'a rien fait car le fichier de configuration était incomplet]" << termcolor::reset;
			config_path = std::filesystem::path(std::string(DATA_PATH));
#ifdef THROW_SIMULATION_ERRORS
			throw INCOMPLETE_SIM;
#endif
		}
	} else {
#ifdef NON_EXISTING_PATH_WARNINGS
		std::cerr << termcolor::red << "[Simulation::load(" << nom_sim << "," << categorie << ") n'a rien fait car la simulation indiquée n'existe pas]" << termcolor::reset;
#endif
		config_path = std::filesystem::path(std::string(DATA_PATH));
#ifdef THROW_SIMULATION_ERRORS
		throw NON_EXISTING_SIM;
#endif
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


Motif Simulation::motif(unsigned int const& num_gen) const {
	Motif res;
	if (config_path.extension().string() == ".csv" && num_gen <= this->duree()) {
		
		size_t j(0);
		std::vector<size_t> row;
		while (j < config_file->GetColumnCount()) {
			try {
				row.push_back(config_file->GetCell<size_t>(j,2+num_gen));
				++j;
			}
			catch (...) {
				std::cerr << termcolor::yellow << "[Simulation::motif(" << num_gen << ") n'a pas terminé de lire la ligne car il y a eu un problème de coversion à la position " << j << "]" << termcolor::reset;
				break;
			}
		}
		
		if (row.size()%2 == 0) {
			for (size_t i(0); i < row.size() ; i+=2) {
				res.push_back({row[i], row[i+1]});
			}
		} else {
			std::cerr << termcolor::red << "[Simulation::motif(" << num_gen << ") n'a rien fait car le fichier de configuration était incomplet (pas un nombre pair de coordonnées sur la ligne)]" << termcolor::reset;
			//throw INCOMPLETE_SIM;
		}
	}
	return res;
}

bool Simulation::finie() const {return grille->generation() == this->duree();}

bool Simulation::evolve() {
	if (grille->generation() < this->duree()) {
		grille->GameOfLife::wipe();
		grille->GameOfLife::addMotif(motif(++grille->generation()));
		return true;
	} else {
		return false;
	}
}

Liste const& Simulation::vivantes() const {return grille->vivantes();}

void Simulation::print(std::ostream& out, bool avec_grille) const {grille->print(out, avec_grille);}

Simulation& Simulation::translate(int const& l, int const& c) {
	grille->translate(l,c);
	return *this;
}
Simulation& Simulation::resize(unsigned int const& lmin, unsigned int const& lmax, unsigned int const& cmin, unsigned int const& cmax) {
	grille->resize(lmin, lmax, cmin, cmax);
	return *this;
}