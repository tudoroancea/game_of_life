#include "motifs.h"
#include "rapidcsv.h"
#include <utility>


// Methodes utilitaires ================================================
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
// Constructeurs & Destructeurs ========================================
Motif::Motif(std::initializer_list<coord> L) : cellules(L) {}
Motif::Motif(liste const& L) : cellules(L) {}
Motif::Motif(std::string const& fichier) {
    rapidcsv::Document motif("../../data/"+fichier+".csv");
    for (size_t i(0); i < motif.GetRowCount() ; ++i) {
        cellules.push_back({motif.GetCell<size_t>(0,i), motif.GetCell<size_t>(1,i)});
    }
}
// Méthodes de modification
void Motif::rotate() {
    size_t max(max_ligne()), tpr(0);
    for (auto& el : cellules) {
            tpr = max-el.first;
            el.first = el.second;
            el.second = tpr;
    }
}
void Motif::rotate2() {
    if (min_ligne()-max_colonne()>=0) {
            size_t ibis(0),jbis(0),l(min_ligne()),c(min_colonne());
            for (auto& el : cellules) {
            jbis = c+el.first;
            ibis = l-el.second;
            el.first = ibis;
            el.second = jbis;
            }
    }
}
Motif& Motif::operator+=(Motif const& rhs) {
    for (auto const& el : rhs.cellules) cellules.push_back(el);
    return *this;
}
Motif& Motif::operator+=(coord const& p) {
    for (auto& el : cellules) {
            el.first += p.first;
            el.second += p.second;
    }
    return *this;
}
Motif& Motif::operator-=(coord const& p) {
    size_t l(min_ligne()), c(min_colonne());
    if (p.first>l) {
            for (auto& el : cellules) el.first -= l;
    } else {
            for (auto& el : cellules) el.first -= p.first;
    }
    if (p.second>c) {
            for (auto& el : cellules) el.second -= c;
    } else {
            for (auto& el : cellules) el.second -= p.second;
    }
    return *this;
}
Motif operator+(Motif lhs, Motif const& rhs) {return lhs+=rhs;}
Motif operator+(Motif lhs, coord const& rhs) {return lhs+=rhs;}
Motif& Motif::recalibrate() {(*this)-={min_ligne(), min_colonne()}; return *this;}
// Getters
liste::iterator Motif::begin() {return cellules.begin();}
liste::iterator Motif::end() {return cellules.end();}
liste::const_iterator Motif::cbegin() const {return cellules.cbegin();}
liste::const_iterator Motif::cend() const {return cellules.cend();}

// Construction de motifs géométriques
Motif rectangle(size_t lignes, size_t colonnes) {
    Motif res;
    for (size_t i(0); i < lignes ; ++i) {
        for (size_t j(0); j < colonnes ; ++j) res.push_back({i,j});
    }
    return res;
}
Motif ligneh(size_t longueur) {
    Motif res;
    for (size_t i(0); i < longueur ; ++i) res.push_back({i,0});
    return res;
}
Motif lignev(size_t longueur) {
    Motif res;
    for (size_t j(0); j < longueur ; ++j) res.push_back({0,j});
    return res;
}

void translate(Calque& calque)
{
    calque.translate = *(calque.alive.cbegin());
}


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
                            if ((--y1) == y2) {
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
                    std::cout << x1 << ' ' << y1 << std::endl;
                } while ((++y1) < y2);
            } else { // dy<0 et dx=0
                // vecteur vertical décroissant
                do {
                    res.push_back({x1,y1});
                    std::cout << x1 << ' ' << y1 << std::endl;
                } while ((--y1) > y2);
            }
        }
    }
    res.push_back({x2,y2});
    return res;
}

liste segment(coord a, coord const& b) {return segment(a.first, a.second, b.first, b.second);}