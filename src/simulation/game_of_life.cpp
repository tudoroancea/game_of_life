#include "game_of_life.h"
#include "motifs.h"
#include "rapidcsv.h"
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

GameOfLife& GameOfLife::add_cell(coord const& c) {
	coord c_translate({c.first+50,c.second+50});
	if((!access(c_translate.first, c_translate.second)) && ((c_translate.first < L+50) && (c_translate.second < C+50))) {
		vivantes.push_back(c_translate);
		vivantes_visibles.push_back(c);
        champ[c_translate.first][c_translate.second] = true;
	}
	return *this;
}
GameOfLife& GameOfLife::add_cell(size_t const& i, size_t const& j) {add_cell({i,j}); return *this;}
GameOfLife& GameOfLife::suppr_cell(coord const& c) {
	coord c_translate({c.first+50,c.second+50});
	if (access(c_translate.first, c_translate.second)) {
		champ[c_translate.first][c_translate.second] = false;
		vivantes.erase(std::find<liste::iterator, coord>(vivantes.begin(),	vivantes.end(), c_translate));
		vivantes_visibles.erase(std::find<liste::iterator, coord>(vivantes_visibles.begin(), vivantes_visibles.end(), c));
	}
	return *this;
}
GameOfLife& GameOfLife::suppr_cell(size_t const& i, size_t const& j) {suppr_cell({i,j}); return *this;}
GameOfLife& GameOfLife::inv_cell(coord const& c) {
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
	return *this;
}
GameOfLife& GameOfLife::inv_cell(size_t const& i, size_t const& j) {inv_cell({i,j}); return *this;}
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
		champ[it->first][it->second] = false;
		vivantes.erase(it);
	}
	vivantes_visibles.erase(vivantes_visibles.begin(), vivantes_visibles.end());
	return *this;
}
GameOfLife& GameOfLife::resize(unsigned int const& l, unsigned int const& c) {
	L = l; C = c;
	for (liste::iterator it(vivantes.begin()); it != vivantes.end(); ++it) {
		if(it->first >= L+50 || it->second >= C+50) {
			champ[it->first][it->second] = false;
			vivantes.erase(it);
		}
	}
	for (liste::iterator it(vivantes_visibles.begin()); it != vivantes_visibles.end(); ++it) {
		if (!access(it->first, it->second)) {
			vivantes_visibles.erase(it);
			--it;
		}
	}
	return *this;
}
unsigned int& GameOfLife::get_nbr_gen() {return nbr_gen;}
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

// Enregistrement de motifs et simulations ========================================================================================
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
bool GameOfLife::save_sim(std::string const& nom_simulation, unsigned int const& duree_sim, std::string const& categorie) {
	// On verifie dans quel dossier on doit enregistrer la simulation
	std::filesystem::path chemin;
	if (categorie != "local") chemin = std::filesystem::path("../../data/presaved/sims/"+nom_simulation);
    else chemin = std::filesystem::path("../../data/local/sims/"+nom_simulation);

	// On verifie si un dossier du même nom existe deja, et sinon en enregistre la simulation
	if (!std::filesystem::exists(chemin)) {
		std::filesystem::create_directory(chemin);
		std::ofstream info, content;

		// On crée les informations de la simulation (dimensions de la grille, nombre de generations)
		info.open(chemin.string()+"/"+nom_simulation+"-info.csv");
		info << "ligne,colonne\n" << L << ',' << C << '\n' << duree_sim << ",\n";
		content.open(chemin.string()+"/"+nom_simulation+"-content.cvs");

		// On cree un premier fichier pour la config au depart (avant lacement de la simulation)
		size_t compteur(0);
		info << compteur << ",\n";
		content << "ligne,colonne\n";
		for (auto const& el : vivantes_visibles) content << el.first << ',' << el.second << '\n';
		compteur += vivantes_visibles.size();

		// On enregistre un nouveau fichier pour chaque nouvelle generation
		for (size_t i(0); i < duree_sim ; ++i) {
			info << compteur << ",\n";
			evolve();
			for (auto const& el : vivantes_visibles) content << el.first << ',' << el.second << '\n';
			compteur += vivantes_visibles.size();
		}
		info << compteur << '\n';
		info.close(); content.close();
		return true;
	} else return false;
}



Simulation::Simulation() : nom(""), info_path(std::filesystem::path(std::string(GLOBAL_PATH)+"/data")), content_path(std::filesystem::path(std::string(GLOBAL_PATH)+"/data")) {}

Simulation::Simulation(std::string const& nom_sim, std::string const& categorie)
	: nom(nom_sim),
	  info_path(std::filesystem::path(categorie == "local" ? std::string(GLOBAL_PATH)+"/data/"+categorie+"/sims" : std::string(GLOBAL_PATH)+"/data/presaved/sims")),
	  content_path(std::filesystem::path(categorie == "local" ? std::string(GLOBAL_PATH)+"/data/"+categorie+"/sims" : std::string(GLOBAL_PATH)+"/data/presaved/sims"))
{
	if (std::filesystem::exists(info_path/nom_sim)) {
		(info_path /= nom_sim) /= nom_sim+"-info.csv";
		(content_path /= nom_sim) /= nom_sim+"-content.csv";
		info_file = rapidcsv::Document(info_path.string());
		content_file = rapidcsv::Document(content_path.string());
		grille.resize(info_file.GetCell<unsigned int>(0,0), info_file.GetCell<unsigned int>(1,0));
	}
}
bool Simulation::exist_sim() const {return (info_path.extension().string()==".csv");}

Motif Simulation::get_motif(unsigned int const& num_gen) const {
	Motif res;
	if (num_gen <= info_file.GetCell<unsigned int>(0,1)) {
		unsigned int debut(info_file.GetCell<unsigned int>(0,num_gen+2)), fin(info_file.GetCell<unsigned int>(0,num_gen+3));
		for (unsigned int i(debut); i < fin ; ++i) res.push_back({content_file.GetCell<size_t>(0,i), content_file.GetCell<size_t>(1,i)});
	}
	return res;
}

void Simulation::evolve() {
	if (grille.get_nbr_gen() < info_file.GetCell<unsigned int>(0,1)) {
		grille.wipe();
		++grille.get_nbr_gen();
		grille.add_motif(get_motif(grille.get_nbr_gen()));
	}
}

liste Simulation::get_viv() const {return grille.get_viv();}