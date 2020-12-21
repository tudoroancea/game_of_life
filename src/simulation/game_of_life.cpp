#include "game_of_life.h"
#include "motifs.h"
#include "rapidcsv.h"

#include <string>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm> //
#include <typeinfo> // for typeid
#include <filesystem> // pour trouver les simulations existantes

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
	if (i < MAX_LIGNES+100 || j < MAX_COLONNES+100) return grille[i][j];
	else {
		#ifdef OVERFLOW_WARNINGS
			std::cerr << "[GameOfLife::access(" << i << "," << j << ") a renvoyé false car les coordonées étaient trop grandes]";
		#endif
		return false;
	}
}
liste const& GameOfLife::get_viv() const {return vivantes;}
std::array<std::array<bool,500>,500> const& GameOfLife::get_grille() const {return grille;}

// Setters ================================================
GameOfLife& GameOfLife::add_cell(size_t const& i, size_t const& j) {
	#ifdef OVERFL0W_WARNINGS
	if (i < MAX_LIGNES && j < MAX_COLONNES) {
		if (!access(i+50, j+50)) {
			vivantes.push_back({i+50, j+50});
			grille[i][j] = true;
		}
	} else {
		std::cerr << "[GameOfLife::add_cell(" << i << "," << j << ") n'a rien fait car les coordonnées étaient trop grandes]";
	}
	#else
	if (!access(i+50,j+50) && i < MAX_LIGNES && j < MAX_COLONNES) {
		vivantes.push_back({i+50, j+50});
		grille[i][j] = true;
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
		std::cerr << "[GameOfLife::suppr_cell(" << i << "," << j << ") n'a rien fait car les coordonnées étaient trop grandes]";
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
		std::cerr << "[GameOfLife::inv_cell(" << i << "," << j << ") n'a rien fait car les coordonnées étaient trop grandes]";
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
		std::cerr << "[verif(" << i << "," << j << ",vivantes) n'a rien fait car les coords étaient trop grandes]";
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

// Enregistrement de motifs et simulations ========================================================================================
void GameOfLife::save_motif(std::string const& nom_motif, FILE_CATEGORY const& categorie) const {
	std::filesystem::path chemin(std::string(DATA_PATH));
	if (std::filesystem::exists(chemin)) {
		if (categorie == local) {
			if (std::filesystem::exists(chemin/="local/motifs")) {
				std::filesystem::current_path(chemin);
				std::ofstream out(nom_motif+".csv");
				for (auto const& el : vivantes) out << el.first << ',' << el.second << '\n';
				out.close();
			} else std::cerr << " ERROR : On ne peut pas sauver le motif car " << DATA_PATH << "/local/motifs n'existe pas." << std::endl;
		} else {
			if (std::filesystem::exists(chemin/="presaved/motifs")) {
				std::filesystem::current_path(chemin);
				std::ofstream out(nom_motif+".csv");
				for (auto const& el : vivantes) out << el.first << ',' << el.second << '\n';
				out.close();
			} else std::cerr << " ERROR : On ne peut pas sauver le motif car " << DATA_PATH << "/presaved/motifs n'existe pas." << std::endl;
		}
	} else std::cerr << " ERROR : On ne peut pas sauver le motif car le dossier " << DATA_PATH << " n'existe pas." << std::endl;
}
bool GameOfLife::save_sim(std::string const& nom_simulation, unsigned int const& duree_sim, FILE_CATEGORY const& categorie) {
	// On verifie dans quel dossier on doit enregistrer la simulation
	std::filesystem::current_path(std::string(DATA_PATH));
	std::filesystem::path chemin;
	if (categorie == local) chemin = std::filesystem::path("local/sims/");
    else chemin = std::filesystem::path("presaved/sims/");
	// On verifie si un dossier du même nom existe deja, et sinon en enregistre la simulation
	if (!std::filesystem::exists(chemin)) {
		return false;
		#ifdef NON_EXISTING_PATH_WARNINGS
			std::cerr << "[GameOfLife::save_sim(" << nom_simulation << "," << duree_sim << "," << categorie << ") a renvoyé false car " DATA_PATH;
			if (categorie == local) std::cerr << "/local/sims/ n'existe pas]";
			else std::cerr << "/presaved/sims/ n'existe pas]";
		#endif
	}
	chemin /= ("sim-"+nom_simulation);
	if (!std::filesystem::exists(chemin)) {
		std::filesystem::create_directory(chemin);
		std::filesystem::current_path(chemin);
		std::ofstream info, content;

		// On crée les informations de la simulation (dimensions de la grille, nombre de generations)
		info.open(chemin.string()+"/"+nom_simulation+"-info.csv");
		info << "ligne,colonne\n"
			 << MAX_LIGNES << "," << MAX_COLONNES << '\n'
			 << duree_sim << ",\n";
		content.open(chemin.string()+"/"+nom_simulation+"-content.csv");

		//
		size_t compteur(1);
		info << compteur << ",\n";
		content << "ligne,colonne\n";
		for (auto const& el : vivantes) content << el.first-50 << ',' << el.second-50 << '\n';
		compteur += vivantes.size();

		//
		for (size_t i(0); i < duree_sim ; ++i) {
			info << compteur << ",\n";
			this->evolve();
			for (auto const& el : vivantes) content << el.first-50 << ',' << el.second-50 << '\n';
			compteur += vivantes.size();

		}
		info << compteur << ",\n";
		info.close(); content.close();
		return true;
	} else {
		std::cerr << "Le dossier de simulation existe déjà. " << std::endl;
		return false;
	}
}

// Constructeurs ==================================================================================================================
GameOfLifeView::GameOfLifeView(unsigned int const& lmin, unsigned int const& lmax, unsigned int const& cmin, unsigned int const& cmax)
: GameOfLife(),
	Lmin(lmin%400 < lmax%400 ? lmin%400 : lmax%400),
	Lmax(lmin%400 < lmax%400 ? lmax%400 : lmin%400),
	Cmin(cmin%400 < cmax%400 ? cmin%400 : cmax%400),
	Cmax(cmin%400 < cmax%400 ? cmax%400 : cmin%400)
{
	if (Lmin < 50) Lmin = 50;
	if (Lmax >= MAX_LIGNES-50) Lmax = MAX_LIGNES-50;
	if (Cmin < 50) Cmin = 50;
	if (Cmax >= MAX_COLONNES-50) Cmax = MAX_COLONNES-50;
}

// Setters du jeu ==========================================================================================================================================================================
GameOfLifeView& GameOfLifeView::add_cell(size_t const& i, size_t const& j) {
	if (i < Lmax-Lmin && j < Cmax-Cmin) {
		bool deja_vivante(access(i+Lmin+50,j+Cmin+50));
		GameOfLife::add_cell(i+Lmin,j+Cmin);
		if (!deja_vivante && access(i+Lmin+50,j+Cmin+50)) vivantes_visibles.push_back({i, j});
	} else {
		#ifdef OVERFLOW_WARNINGS
		std::cerr << "[GameOfLifeView::add_cell(" << i << "," << j << ") n'a rien fait car coords trop grandes]";
		#endif
	}
	return *this;
}
GameOfLifeView& GameOfLifeView::add_cell(coord const& c) {return add_cell(c.first, c.second);}
GameOfLifeView& GameOfLifeView::suppr_cell(size_t const& i, size_t const& j) {
	if (i < Lmax-Lmin && j < Cmax-Cmin) {
		bool vivante_avant(access(i+Lmin+50,j+Cmin+50));
		GameOfLife::suppr_cell(i+Lmin,j+Cmin);
		if (vivante_avant) {
			liste::iterator a_effacer(std::find<liste::iterator, coord>(vivantes_visibles.begin(), vivantes_visibles.end(), {i, j}));
			if (a_effacer != vivantes_visibles.end()) vivantes_visibles.erase(a_effacer);
		}
	} else {
		#ifdef OVERFLOW_WARNINGS
		std::cerr << "[GameOfLifeView::suppr_cell(" << i << "," << j << ") n'a rien fait car coords trop grandes]";
		#endif
	}
	return *this;
}
GameOfLifeView& GameOfLifeView::suppr_cell(coord const& c) {return suppr_cell(c.first, c.second);}
GameOfLifeView& GameOfLifeView::inv_cell(size_t const& i, size_t const& j) {
	if (i < Lmax-Lmin && j < Cmax-Cmin) {
		bool vivante_avant(access(i+Lmin+50,j+Cmin+50));
		GameOfLife::inv_cell(i+Lmin, j+Cmin);
		if (vivante_avant && !access(i+Lmin+50,j+Cmin+50)) {
			liste::iterator a_effacer(std::find<liste::iterator, coord>(vivantes_visibles.begin(), vivantes_visibles.end(), {i, j}));
			if (a_effacer != vivantes_visibles.end()) vivantes_visibles.erase(a_effacer);
		} else if (!vivante_avant && access(i+Lmin+50, j+Cmin+50)) {
			vivantes_visibles.push_back({i,j});
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
//GameOfLifeView& GameOfLifeView::resize(unsigned int const& l, unsigned int const& c) {
//	if (l <= MAX_LIGNES && c <= MAX_COLONNES) {

//	} else {
//		#ifdef OVERFLOW_WARNINGS
//		std::cerr << "[GameOfLifeView::resize(" << l << "," << c << ") n'a rien fait car bords dépassent la grille]";
//		#endif
//	}
//}
GameOfLifeView& GameOfLifeView::translate(int const& l, int const& c) {
	if (-Lmin <= l && l <= MAX_LIGNES-Lmax && -Cmin <= c && c <= MAX_COLONNES-Cmax) {
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
		std::cerr << "[GameOfLifeView::translate(" << l << "," << c << ") n'a rien fait car bords dépassent la grille]";
		#endif
	}
	return *this;
}

void GameOfLifeView::print(std::ostream& out, bool avec_grille) const {
	if (avec_grille) {
		out << "Génération n° " << nbr_gen << "\n";
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
		out << "Génération n° " << nbr_gen << "\n";
		for (size_t i(0); i < Lmax-Lmin ; ++i) {
			for (size_t j(0); j < Cmax-Cmin ; ++j) {
				if (grille[i+Lmin][j+Cmin]) out << "0";
				else out << ' ';
			}
			out << "\n";
		}
		out << "\n=================================\n";
	}
}



Simulation::Simulation()
	: nom(""), grille(nullptr), nbr_gen(0), info_path(std::filesystem::path(std::string(DATA_PATH))), content_path(std::filesystem::path(std::string(DATA_PATH))) {}

Simulation::~Simulation() {
	delete grille;
}

Simulation& Simulation::load(std::string const& nom_sim, FILE_CATEGORY const& categorie) {
	nom = nom_sim;
	if (categorie == local) {
		info_path /= "local/sims/sim-"+nom_sim;
		content_path /= "local/sims/sim-"+nom_sim;
	} else {
		info_path /= "presaved/sims/sim-"+nom_sim;
		content_path /= "presaved/sims/sim-"+nom_sim;
	}

	if (std::filesystem::exists(info_path)) {
		// Le dossier de simulation existe bien. On ajuste les path correspondants
		//std::string s1(nom_sim+"-info.csv"), s2(nom_sim+"-content.csv");
		info_path /=  (nom_sim+"info.csv");
		content_path /= (nom_sim+"-content.csv");
		if (std::filesystem::exists(info_path)) {
			// Le fichier info existe bien. On l'ouvre
			info_file = rapidcsv::Document(info_path.string());
			// On essaie de créer la grille à partir des dimensions indiquées dans info.csv. Si les dimensions ne correspondent pas à MAX_LIGNES et MAX_COLONNES, on ne crée pas
			unsigned int L(0);
			unsigned int C(0);
			try {
				L = info_file.GetCell<unsigned int>(0,1);
				C = info_file.GetCell<unsigned int>(1,1);
			}
			catch (...) { // On peut attraper des erreurs out_of_range mais aussi des erreurs de conversion
				throw Error::INCOMPLETE_INFO;
			}
			if (L == MAX_LIGNES && C == MAX_COLONNES) {
				grille = new GameOfLifeView(0, MAX_LIGNES, 0, MAX_COLONNES);
				size_t debut(0), fin(0);
				try {
					nbr_gen = info_file.GetCell<unsigned int>(0,2);
					debut = info_file.GetCell<size_t>(0,3);
					fin = info_file.GetCell<size_t>(0,4);
				}
				catch (...) { // On peut attraper des erreurs out_of_range mais aussi des erreurs de conversion
					throw Error::INCOMPLETE_INFO;
				}
				// On essaie d'ouvrir la génération 0
				if (debut != 0 && fin != 0) {
					try {
						Motif M;
						for (size_t i(debut); i < fin ; ++i) {
							M.push_back({content_file.GetCell<size_t>(0,i), content_file.GetCell<size_t>(1,i)});
						}
						grille->add_motif(M);
					}
					catch (...) {
						throw Error::INCOMPLETE_CONTENT;
					}
				} else {
					throw Error::NON_EXISTING_CONTENT;
				}
			} else {
				nom = "";
				info_path = std::filesystem::path(std::string(DATA_PATH));
				content_path = std::filesystem::path(std::string(DATA_PATH));
				#ifdef WRONG_DIMENSIONS_WARNINGS
				std::cerr << "[Simulation::load(" << nom_sim << "," << categorie << ") n'a pas créé de grille car les dimensions de la simulation enregistrée de correspondent pas à celles de la version du progamme]";
				#endif
			}
		} else {
			throw Error::NON_EXISTING_INFO;
		}
	} else {
		throw Error::NON_EXISTING_SIM;
	}
	return *this;
}

Simulation& Simulation::release() {
	nom = "";
	delete grille;
	grille = nullptr;
	nbr_gen = 0;
	info_path = std::filesystem::path(std::string(DATA_PATH));
	content_path = std::filesystem::path(std::string(DATA_PATH));
	return *this;
}

std::string const& Simulation::get_nom() const {return nom;}

Motif Simulation::get_motif(unsigned int const& num_gen) const {
	Motif res;
	if (nbr_gen != 0 && num_gen <= nbr_gen) {
		unsigned int debut(0), fin(0);
		try {
			debut = info_file.GetCell<size_t>(0,3+num_gen);
			fin = info_file.GetCell<size_t>(0,4+num_gen);
		}
		catch(...) { // On attrape des out_of_range mais aussi des erreurs de conversion
			throw Error::INCOMPLETE_INFO;
			return res;
		}
		try {
			for (size_t i(debut); i < fin ; ++i) {
				res.push_back({content_file.GetCell<size_t>(0,i),content_file.GetCell<size_t>(1,i)});
			}
		}
		catch (...) { // On attrape des out_of_range mais aussi des erreurs de conversion
			throw Error::INCOMPLETE_CONTENT;
			return res;
		}
	}
	return res;
}

bool Simulation::finie() const {return grille->get_nbr_gen() == nbr_gen;}

bool Simulation::evolve() {
	if (grille->get_nbr_gen() < nbr_gen) {
		grille->GameOfLife::wipe();
		++grille->get_nbr_gen();
		try {
			grille->GameOfLife::add_motif(get_motif(grille->get_nbr_gen()));
		}
		catch (Error const& err) {
			switch (err) {
				case INCOMPLETE_INFO:
                    std::cerr << "[ERROR : Fichier de configuration <" << nom << "-info.csv> incomplet.]" << std::endl;
					break;
                case INCOMPLETE_CONTENT:
                    std::cerr << "[ERROR : Fichier de contenu <" << nom << "-content.csv> incomplet.]" << std::endl;
					break;
                default:
                    std::cerr << "[UNEXPECTED ERROR]" << std::endl;
					break;
			}
		}
		return true;
	} else {
		return false;
	}
}

liste Simulation::get_viv() const {return grille->get_viv();}

void Simulation::print(std::ostream& out, bool avec_grille) const {grille->print(out, avec_grille);}

Simulation& Simulation::translate(int const& l, int const& c) {
	grille->translate(l,c);
	return *this;
}
Simulation& Simulation::resize(unsigned int const& lmin, unsigned int const& lmax, unsigned int const& cmin, unsigned int const& cmax) {
	grille->resize(lmin, lmax, cmin, cmax);
	return *this;
}

/*std::vector<Motif> composants_connexes(GameOfLife const& jeu) {
	liste vivantes(jeu.get_viv());
	std::vector<Motif> res;
	for (auto const& cell : vivantes) {
		if (!res.empty()) {
			std::vector<Motif>::iterator it(res.begin());
			bool test(true);
			while (it != res.end() && test) {
				test = !it->a_pour_voisin(cell);
				++it;
			}
			if (it != res.end()) it->push_back(cell);
			else res.push_back(Motif({cell}));
		} else {
			res.push_back(Motif({cell}));
		}
	}
	return res;
}*/

/*size_t find(std::vector<size_t> const& L, size_t const& e) {
	size_t r(e);
	while (L[r] != r) r = L[r];
	return r;
}
size_t unionn(std::vector<size_t>& L, size_t const& e1, size_t const& e2) {
	if (e1 < e2) {
		L[e2] = e1;
		return e1;
	} else {
		L[e1] = e2;
		return e2;
	}
}*/
//bool is_adjacent(coord const& p1, coord const& p2) {return sont_voisins(p1, p2);}
//bool is_far_enough(coord const& p1, coord const& p2) {return Y(p1)-Y(p2) > 1;}
//void init_features(size_t const& label, coord const& pixel);
//void accumulate_features(size_t const& label, coord const& pixel);
/*std::vector<size_t> GameOfLife::sparseCLL() {
	// first scan: pixel association
	size_t start_j(0), n(vivantes.size()), tpr(0);
	std::vector<size_t> Liste(n);
	for (size_t i(0); i < n ; ++i) {
		Liste[i] = i;
		tpr = i;
		for (size_t j(start_j); j < i-1 ; ++j) {
			if (is_adjacent(vivantes[i], vivantes[j])) {
				tpr = unionn(Liste, tpr, find(Liste, j));
			} else if (is_far_enough(vivantes[i], vivantes[j])) {
				++start_j;
			}
		}
	}

	// Second scan:  transittive closure and analysis
	size_t labels(0), l(0);
	for (size_t i(0); i < n-1 ; ++i) {
		if (Liste[i] == i) {
			++labels;
			l = labels;
			//init_features(l, vivantes[i]);
		} else {
			l = Liste[Liste[i]];
			//accumulate_features(l, vivantes[i]);
		}
		Liste[i] = l;
	}
	Liste.push_back(labels);
	return Liste;
}*/