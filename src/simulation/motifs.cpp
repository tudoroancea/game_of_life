#include "motifs.h"
#include "rapidcsv.h"
#include "termcolor.hpp"

#include <utility>
#include <algorithm> // pour std::min, std::max
#include <cmath> // pour std::abs
#include <filesystem> // Pour trouver la liste des fichiers


std::ostream& operator<<(std::ostream& out, FILE_CATEGORY const& cat) {
    if (cat == local) out << "local";
    else out << "presaved";
    return out;
}
std::ostream& operator<<(std::ostream& out, coord const& c) {
	out << X(c) << "," << Y(c);
	return out;
}

// Constructeurs & Destructeurs ========================================
Motif::Motif(std::initializer_list<coord> L) : cellules(L) {}
Motif::Motif(liste const& L) : cellules(L) {}
Motif::Motif(std::filesystem::path const& chemin) {
	if (std::filesystem::exists(chemin)) {
		rapidcsv::Document motif(chemin, rapidcsv::LabelParams(-1,-1));
		for (size_t i(0); i < motif.GetColumnCount() ; i+=2) {
			cellules.push_back({motif.GetCell<size_t>(i,0), motif.GetCell<size_t>(i+1,0)});
		}
	} else {
		#ifdef NON_EXISTING_PATH_WARNINGS
			std::cerr << termcolor::yellow << "[Motif(" << chemin.string() << ") a construit un motif vide car ce chemin n'existe pas]" << termcolor::reset;
		#endif
	}
}
Motif::Motif(std::string const& fichier, FILE_CATEGORY const& categorie) {
	std::filesystem::path chemin;
	if (categorie == presaved) chemin = std::filesystem::path(std::string(PRESAVED_PATH)+"/motifs/"+fichier+".csv");
	else chemin = std::filesystem::path(std::string(LOCAL_PATH)+"/motifs/"+fichier+".csv");
	if (std::filesystem::exists(chemin)) {
		rapidcsv::Document motif(chemin, rapidcsv::LabelParams(-1,-1));
		for (size_t i(0); i < motif.GetColumnCount() ; i+=2) {
			cellules.push_back({motif.GetCell<size_t>(i,0), motif.GetCell<size_t>(i+1,0)});
		}
	} else {
		#ifdef NON_EXISTING_PATH_WARNINGS
			std::cerr << termcolor::yellow << "[Motif(" << chemin.string() << ") a construit un motif vide car ce chemin n'existe pas]" << termcolor::reset;
		#endif
	}
}

// Méthodes de modification
Motif& Motif::push_back(coord const& p) {cellules.push_back(p); return *this;}
Motif& Motif::rotate(int const& angle) {
	return rotate(angle, {(max_ligne()-min_ligne())/2,(max_colonne()-min_colonne())/2});
}
Motif& Motif::rotate(int const& angle, coord const& centre) {
	bool test(false);
	switch (angle%4) {
		case -3:
		case 1: { // Rotation de 90° en sens trigo
			if (centre.second <= max_colonne()) {
				if (max_colonne()-centre.second <= centre.first) {
					if (min_ligne() <= centre.first) {
						if (centre.first-min_ligne() <= centre.second) {
							test = true;
						} else {
							test = false;
						}
					} else {
						test = true;
					}
				} else {
					test = false;
				}
			} else if (min_ligne() <= centre.first) {
				if (centre.first-min_ligne() <= centre.second) {
					test = true;
				} else {
					test = false;
				}
			} else {
				test = true;
			}
			if (test) {
				size_t tpr(0);
				for (auto& el : cellules) {
					tpr = el.first;
					el.first = centre.second + centre.first - el.second;
					el.second = tpr - centre.first + centre.second;
				}
			}
			break;
		}
		case -2:
		case 2: { // Rotation de 180° en sens trigo
			// xprime = -x + 2a
			// prime = -y +2b
			if (centre.second <= max_colonne()) {
                if (max_colonne()-centre.second <= centre.second) {
                    if (centre.first <= max_ligne()) {
                        if (max_ligne()-centre.first <= centre.first) {
                            test = true;
                        } else {
                            test = false;
                        }
                    } else {
                        test = true;
					}
                } else {
					test = false;
				}
            } else if (centre.first <= max_ligne()) {
				if (max_ligne()-centre.first <= centre.first) {
                    test = true;
                } else {
                    test = false;
                }
            } else {
                test = true;
            }
            if (test) {
                for (auto& el : cellules) {
                    el.first = 2*centre.first - el.first;
                    el.second = 2*centre.second - el.second;
                }
            }
			break;
		}
		case -1:
		case 3: { // Rotation de 270° en sens trigo
			// xprime = y - b + a
			// yprime = -x + a + b
			if (centre.first <= max_ligne()) {
                if (max_ligne()-centre.first <= centre.second) {
                    if (min_colonne() <= centre.second) {
                        if (centre.second-min_colonne() <= centre.first) {
                            test = true;
                        } else {
                            test = false;
                        }
                    } else {
                        test = true;
					}
                } else {
					test = false;
				}
            } else if (min_colonne() <= centre.second) {
				if (centre.second-min_colonne() <= centre.first) {
                    test = true;
                } else {
                    test = false;
                }
            } else {
                test = true;
            }
            if (test) {
                size_t tpr(0);
                for (auto& el : cellules) {
                    tpr = el.first;
                    el.first = el.second - centre.second + centre.first;
                    el.second = centre.first + centre.second - tpr;
				}
            }
			break;
		}
		default:
			break;
	}
	return *this;
}

Motif& Motif::append(Motif const& autre) {
	for (auto const& el : autre.cellules) cellules.push_back(el);
	return *this;
}
Motif& Motif::append(liste const& autre) {
	for (auto const& el : autre) cellules.push_back(el);
	return *this;
}
Motif& Motif::append(std::initializer_list<coord> const& autre) {
	for (std::initializer_list<coord>::iterator it(autre.begin()); it != autre.end(); ++it) cellules.push_back(*it);
	return *this;
}
Motif& Motif::translate(int const& x, int const& y) {
	size_t xbis(0),ybis(0);
	if (x < 0) xbis = std::min(size_t(-x),min_ligne());
	if (y < 0) ybis = std::min(size_t(-y),min_colonne());
	for (auto& el : cellules) {
		if (xbis != 0) X(el) -= xbis;
		else X(el) += x;
		if (ybis != 0) Y(el) -= ybis;
		else Y(el) += y;
	}
	return *this;
}
Motif& Motif::translate(std::pair<int,int> const& p) {return translate(p.first, p.second);}
Motif& Motif::recalibrate() {translate(-min_ligne(), -min_colonne()); return *this;}
// Getters
liste::iterator Motif::begin() {return cellules.begin();}
liste::iterator Motif::end() {return cellules.end();}
liste::const_iterator Motif::cbegin() const {return cellules.cbegin();}
liste::const_iterator Motif::cend() const {return cellules.cend();}

// Infos
bool Motif::contient(coord const& c) const {
	return std::find(cellules.begin(), cellules.end(), c) != cellules.end();
}
size_t dist(size_t const& a, size_t const& b) {return (a <= b ? b-a : a-b);}
bool sont_voisins(coord const& a, coord const& b) {
	return dist(X(a),X(b)) <= 1 && dist(Y(a),Y(b)) <= 1;
}
bool Motif::a_pour_voisin(coord const& c) const {
	liste::const_iterator it(cellules.cbegin());
	while (it != cellules.cend() && !sont_voisins(*it, c)) {
		std::cout << it->first << ", " << it->second << std::endl;
		++it;
	}
	return it != cellules.cend();
	//for (auto const& cell : cellules) {
	//	bool test(sont_voisins(cell, c));
	//	if (test) {
	//		return true;
	//	}
	//}
	//return false;
}
size_t Motif::min_ligne() const {
	size_t min(cellules.front().first);
	for (auto el : cellules) {
			if (el.first < min) min = el.first;
			if (min == 0) break;
		}
		return min;
}
size_t Motif::min_colonne() const {
	size_t min(cellules.front().second);
	for (auto el : cellules) {
		if (el.second < min) min = el.second;
		if (min == 0) break;
	}
	return min;
}
size_t Motif::max_ligne() const {
	size_t max(cellules.front().first);
	for (auto el : cellules) {
			if (el.first > max) max = el.first;
	}
	return max;
}
size_t Motif::max_colonne() const {
	size_t max(cellules.front().second);
	for (auto el : cellules) {
			if (el.second > max) max = el.second;
	}
	return max;
}

// Gestion des motifs enregsitres
std::vector<std::string> existing_local_motifs() {
	std::vector<std::string> res;
	//std::cerr << termcolor::red << "loc_mot:" << std::filesystem::current_path().string() << termcolor::reset << std::endl;
	std::filesystem::path local_motifs(std::string(LOCAL_PATH)+"/motifs");
	if (std::filesystem::exists(local_motifs)) {
		for (auto const& file : std::filesystem::directory_iterator(local_motifs)) {
			if (file.path().extension() == ".csv") {
				res.push_back(file.path().stem().string());
			}
		}
	}
	return res;
}
std::vector<std::string> existing_presaved_motifs() {
	std::vector<std::string> res;
	//std::cerr << termcolor::red << "pre_mot:" << std::filesystem::current_path().string() << termcolor::reset << std::endl;
	std::filesystem::path presaved_motifs(std::string(PRESAVED_PATH)+"/motifs");
	if (std::filesystem::exists(presaved_motifs)) {
		for (auto const& file : std::filesystem::directory_iterator(presaved_motifs)) {
			if (file.path().extension() == ".csv") {
				res.push_back(file.path().stem().string());
			}
		}
	}
	return res;
}

// Calques Qt
void translate(Calque& calque) {
	calque.translate = *(calque.alive.cbegin());
}


// Construction de motifs géométriques
liste segment(size_t x1, size_t y1, size_t const& x2, size_t const& y2) {
	liste res;
	int dx(x2-x1), dy(y2-y1);
	if (dx != 0) {
		if (dx > 0) {
			if (dy != 0) {
				if (dy > 0) {
					// vecteur oblique 1er cadran
					if (dx >= dy) {
						//vecteur diagonal ou oblique proche de l'horizontale, dans le 1er octant
						int e(dx);
						dx = 2*e;
						dy *= 2;
						while (true) {
							res.push_back({x1,y1});
							if (++x1 == x2) break;
							if ( (e-=dy) < 0) {
								++y1;
								e+=dx;
							}
						}
					} else {
						// vecteur oblique proche de la verticale dans le 2e octant
						int e(dy);
						dy = 2*e;
						dx *= 2;
						while (true) {
							res.push_back({x1,y1});
							if ((++y1) == y2) break;
							if((e-=dx) < 0) {
								++x1;
								e += dy;
							}
						}
					}
				} else { // dy<0 et dx > 0
					if (dx >= -dy) {
						//vecteur diagonal ou oblique proche de l'horizontale, dans le 8e octant
						int e(dx);
						dx = 2*e;
						dy *= 2;
						while (true) {
							res.push_back({x1,y1});
							if ((++x1) == x2) break;
							if ((e+=dy) < 0) {
								--y1;
								e += dx;
							}
						}
					} else {
						// vecteur oblique proche de la verticale, dans le 7e octant
						int e(dy);
						dy = 2*e;
						dx *= 2;
						while (true) {
							res.push_back({x1,y1});
							if ((--y1) == y2) break;
							if ((e += dx) > 0) {
								++x1;
								e += dy;
							}
						}
					}
				}
			} else { // dy=0 et dx>0
				//vecteur horizontal vers la droite
				do {
					res.push_back({x1,y1});
				} while ((++x1) != x2);
			}
		} else { // dx < 0
			dy = y2-y1;
			if (dy != 0) {
				if (dy > 0) {
					// vecteur oblique dans le 2e cadran
					if (-dx >= dy) {
						//vecteur diagonale ou oblique proche de l'horizontale, dans le 4e octant
						int e(dx);
						dx = 2*e;
						dy *= 2;
						while (true) {
							res.push_back({x1,y1});
							if ((--x1) == x2) break;
							if ((e+=dy) >= 0) {
								++y1;
								e += dx;
							}
						}
					} else {
						// vecteur oblique proche de la verticale, dans le 3e octant
						int e(dy);
						dy = 2*e;
						dx *= 2;
						while (true) {
							res.push_back({x1,y1});
							if ((++y1) == y2) break;
							if ((e+=dx) <= 0) {
								--x1;
								e += dy;
							}
						}
					}
				} else { // dy<0 et dx>0
					// vecteur oblique dans le 3e cadran
					if (dx <= dy) {
						// vecteur diagonal ou obliquye proche de l'horizontale, dans le 5e octant
						int e(dx);
						dx = 2*e;
						dy *= 2;
						while (true) {
							res.push_back({x1,y1});
							if ((--x1) == x2) break;
							if ((e-=dy) >= 0) {
								--y1;
								e += dx;
							}
						}
					} else { // vecteur oblique de la verticale, dans le 6e octant
						int e(dy);
						dy = 2*e;
						dx *= 2;
						while (true) {
							res.push_back({x1,y1});
							if ((--y1) == y2) break;
							if ((e-=dx) >= 0) {
								--x1;
								e += dy;
							}
						}
					}
				}
			} else { // dy =0 et dx < 0
				// vecteur horizontal vers la gauche
				do {
					res.push_back({x1,y1});
				} while ((--x1) != x2);
			}
		}
	} else { // dx=0
		if (dy != 0) {
			if (dy > 0) {
				// vecteur vertical croissant
				do {
					res.push_back({x1,y1});
				} while ((++y1) < y2);
			} else { // dy<0 et dx=0
				// vecteur vertical décroissant
				do {
					res.push_back({x1,y1});
				} while ((--y1) > y2);
			}
		}
	}
	res.push_back({x2,y2});
	return res;
}

liste segment(coord a, coord const& b) {return segment(a.first, a.second, b.first, b.second);}
