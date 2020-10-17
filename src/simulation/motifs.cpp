#include "motifs.h"
#include "rapidcsv.h"
#include <utility>

namespace motifs {
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
        rapidcsv::Document motif("../../data/"+fichier);
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
    Motif& Motif::operator+=(std::pair<size_t,size_t> const& p) {
       for (auto& el : cellules) {
             el.first += p.first;
             el.second += p.second;
       }
       return *this;
    }
    Motif& Motif::operator-=(std::pair<size_t,size_t> const& p) {
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
    Motif operator+(Motif lhs, std::pair<size_t,size_t> const& rhs) {return lhs+=rhs;}
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

    void translate(calque& calque)
    {
        calque.translate = *(calque.alive.cbegin());
    }
} // namespace motifs
liste f(coord const& a, coord const& b) {
    //double di(a.first-b.first), dj(a.second-b.second);
    //double tx(dj/di);
    liste res;
    int x,y(a.first),dx(b.first-a.first),dy(b.second-a.second);
    double e(0.0), efirst(dy/dx), esec(-1.0);
    for (int x(a.first); x <= a.second; ++x) {
        if (x >= 0 and y>= 0) res.push_back({size_t(x),size_t(y)});
        e += efirst;
        if (e >= 0.5) {
            ++y;
            e += esec;
        }
    }
}

liste