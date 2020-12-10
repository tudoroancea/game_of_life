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
liste const& GameOfLife::get_viv_2() const {return vivantes;}
std::array<std::array<bool,500>,500> const& GameOfLife::get_champ() const {return champ;}

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
	bool l_is_inf(l<L), c_is_inf(c<C);
	L = l; C = c;
	//Source de la disparition lors du zoom.
	if (l_is_inf || c_is_inf) {
		for (liste::iterator it(vivantes.begin()); it != vivantes.end(); ++it) {
			if(it->first >= L+100 || it->second >= C+100) {
				champ[it->first][it->second] = false;
				vivantes.erase(it);
				--it;
			}
		}
		for (liste::iterator it(vivantes_visibles.begin()); it != vivantes_visibles.end(); ++it) {
			if (!access(it->first, it->second)) {
				vivantes_visibles.erase(it);
				--it;
			}
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
	if (std::filesystem::exists(std::filesystem::path(std::string(DATA_PATH)))) {
		std::filesystem::current_path(std::filesystem::path(std::string(DATA_PATH)));
		std::ofstream out;
		if (dossier != "local") out = std::ofstream("presaved/motifs/"+nom_motif+".csv");
		else out = std::ofstream("local/motifs/"+nom_motif+".csv");
		out << "ligne,colonne\n";
		for (auto const& el : vivantes_visibles) if (imin <= el.first && el.first < imax && jmin <= el.second && el.second < jmax) out << el.first << ',' << el.second << '\n';
		out.close();
	} else std::cerr << " ERROR : On ne peut pas sauver le motif car le dossier " << DATA_PATH << " n'existe pas." << std::endl;
}
bool GameOfLife::save_sim(std::string const& nom_simulation, unsigned int const& duree_sim, std::string const& categorie) {
	// On verifie dans quel dossier on doit enregistrer la simulation
	std::filesystem::current_path(std::string(DATA_PATH));
	std::filesystem::path chemin;
	if (categorie != "local") chemin = std::filesystem::path("presaved/sims/");
    else chemin = std::filesystem::path("local/sims/");
	// On verifie si un dossier du même nom existe deja, et sinon en enregistre la simulation
	if (!std::filesystem::exists(chemin)) {
		return false;
	}
	chemin /= ("sim-"+nom_simulation);
	if (!std::filesystem::exists(chemin)) {
		std::filesystem::create_directory(chemin);
		std::ofstream info, content;

		// On crée les informations de la simulation (dimensions de la grille, nombre de generations)
		info.open(chemin.string()+"/"+nom_simulation+"-info.csv");
		info << "ligne,colonne\n" << L << ',' << C << '\n' << duree_sim << ",\n";
		content.open(chemin.string()+"/"+nom_simulation+"-content.csv");

		// On cree un premier fichier pour la config au depart (avant lacement de la simulation)
		size_t compteur(0);
		info << compteur << ",\n";
		content << "ligne,colonne\n";
		for (auto const& el : vivantes_visibles) content << el.first << ',' << el.second << '\n';
		this->print();
		compteur += vivantes_visibles.size();

		// On enregistre un nouveau fichier pour chaque nouvelle generation
		for (size_t i(0); i < duree_sim ; ++i) {
			info << compteur << ",\n";
			evolve();
			CLEAR()
			this->print();
			for (auto const& el : vivantes_visibles) content << el.first << ',' << el.second << '\n';
			compteur += vivantes_visibles.size();

		}
		info << compteur << ",\n";
		info.close(); content.close();
		return true;
	} else {
		std::cout << "le dossier de simulation existe deja" << std::endl;
		return false;
	}
}



Simulation::Simulation()
	: nom(""), grille(nullptr), nbr_gen(0), info_path(std::filesystem::path(std::string(DATA_PATH))), content_path(std::filesystem::path(std::string(DATA_PATH))) {}

Simulation::~Simulation() {
	delete grille;
}

Simulation& Simulation::load(std::string const& nom_sim, std::string const& categorie) {
	nom = nom_sim;
	if (categorie == "local") {
		info_path /= "local/sims/"+nom_sim;
		content_path /= "local/sims/"+nom_sim;
	} else {
		info_path /= "presaved/sims/"+nom_sim;
		content_path /= "presaved/sims/"+nom_sim;
	}

	if (std::filesystem::exists(info_path)) {
		std::cout << " Le dossier de la simulation <" << nom_sim << "> existe bien." << std::endl;
		// Le dossier de simulation existe bien. On ajuste les path correspondants
		//std::cout << "info_path = " << info_path.string() << " est de type " << typeid(info_path).name() << std::endl;
		//std::cout << "content_path = " << content_path.string() << " est de type " << typeid(content_path).name() << std::endl;
		std::string s1(nom_sim+"-info.csv"), s2(nom_sim+"-content.csv");
		info_path /=  s1;
		content_path /= s2;
		//std::cout << inf.string() << "     " << con.string() << std::endl;
		if (std::filesystem::exists(info_path)) {
			// Le fichier info existe bien. On l'ouvre
			info_file = rapidcsv::Document(info_path.string());
			if (info_file.GetRowCount() >= 3) {
				// Infos sur les dimensions de la grille et la duree de la simulation présentes
				grille = new GameOfLife(info_file.GetCell<unsigned int>(0,0), info_file.GetCell<unsigned int>(1,0));
				nbr_gen = info_file.GetCell<unsigned int>(0,1);
				if (std::filesystem::exists(content_path)) {
					// Le fichier content exite bien. On l'ouvre
					content_file = rapidcsv::Document(content_path.string());
					if (info_file.GetRowCount() >= 4) {
						// Motif non vide (listes de séparateurs contient autre chose que juste 0)
						size_t fin(info_file.GetCell<size_t>(0,3));
						Motif M;
						try {
							for (size_t i(0); i < fin; ++i)	M.push_back({content_file.GetCell<size_t>(0,i), content_file.GetCell<size_t>(1,i)});
						}
						catch(std::out_of_range const& err) {
							throw Error::INCOMPLETE_CONTENT;
						}
						grille->add_motif(M);
					} else {
						std::cout << " WARNING : contruction grille vide au début de la simulation." << std::endl;
					}
				} else {
					throw Error::NON_EXISTING_CONTENT;
				}
			} else {
				throw Error::INCOMPLETE_INFO;
			}
		} else {
			throw Error::NON_EXISTING_INFO;
		}
	} else {
		throw Error::NON_EXISTING_SIM;
	}
	return *this;
}


std::string const& Simulation::get_nom() const {return nom;}

Motif Simulation::get_motif(unsigned int const& num_gen) const {
	Motif res;
	if (nbr_gen != 0 && num_gen <= nbr_gen) {
		unsigned int debut(0), fin(0);
		try {
			debut = info_file.GetCell<unsigned int>(0,num_gen+2);
			fin = info_file.GetCell<unsigned int>(0,num_gen+3);
		}
		catch(std::out_of_range const& err) {
			throw Error::INCOMPLETE_INFO;
			return res;
		}
		try {
			for (unsigned int i(debut); i < fin ; ++i) res.push_back({content_file.GetCell<size_t>(0,i), content_file.GetCell<size_t>(1,i)});
		}
		catch (std::out_of_range const& err) {
			throw Error::INCOMPLETE_CONTENT;
			return res;
		}
	}
	return res;
}

bool Simulation::finie() const {return grille->get_nbr_gen() == nbr_gen;}

void Simulation::evolve() {
	if (grille->get_nbr_gen() < info_file.GetCell<unsigned int>(0,1)) {
		grille->wipe();
		++grille->get_nbr_gen();
		grille->add_motif(get_motif(grille->get_nbr_gen()));
	}
}

liste Simulation::get_viv() const {return grille->get_viv();}

void Simulation::print(std::ostream& out) const {grille->print(out);}

std::vector<Motif> composants_connexes(GameOfLife const& jeu) {
	liste vivantes(jeu.get_viv_2());
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
}

size_t find(std::vector<size_t> const& L, size_t const& e) {
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
}
bool is_adjacent(coord const& p1, coord const& p2) {return sont_voisins(p1, p2);}
bool is_far_enough(coord const& p1, coord const& p2) {return Y(p1)-Y(p2) > 1;}
//void init_features(size_t const& label, coord const& pixel);
//void accumulate_features(size_t const& label, coord const& pixel);
std::vector<size_t> GameOfLife::sparseCLL() {
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
}