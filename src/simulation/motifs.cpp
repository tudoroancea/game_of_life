#include "motifs.h"
#include "rapidcsv.h"
#include <utility>

namespace motifs {
    Motif::Motif(std::string const& fichier) {
        rapidcsv::Document motif("../../data/"+fichier);
        for (size_t i(0); i < motif.GetRowCount() ; ++i) {
            liste.push_back({motif.GetCell<size_t>(0,i), motif.GetCell<size_t>(1,i)});
        }
    }
    size_t Motif::min_ligne() const {
        size_t min(liste.front().first);
        for (auto el : liste) {
                if (el.first < min) min = el.first;
                if (min == 0) break;
            }
            return min;
    }
    size_t Motif::min_colonne() const {
        size_t min(liste.front().second);
        for (auto el : liste) {
            if (el.second < min) min = el.second;
            if (min == 0) break;
        }
        return min;
    }
    size_t Motif::max_ligne() const {
       size_t max(liste.front().first);
       for (auto el : liste) {
             if (el.first > max) max = el.first;
       }
       return max;
    }
    size_t Motif::max_colonne() const {
       size_t max(liste.front().second);
       for (auto el : liste) {
             if (el.second > max) max = el.second;
       }
       return max;
    }
    // Méthodes de modification
    void Motif::rotate()
    // 90° en sens horaire (anti trigo)
    {
       size_t max(max_ligne()), tpr(0);
       for (auto& el : liste) {
             tpr = max-el.first;
             el.first = el.second;
             el.second = tpr;
       }
    }
    void Motif::rotate2() {
       if (min_ligne()-max_colonne()>=0) {
             size_t ibis(0),jbis(0),l(min_ligne()),c(min_colonne());
             for (auto& el : liste) {
                jbis = c+el.first;
                ibis = l-el.second;
                el.first = ibis;
                el.second = jbis;
             }
       }
    }
    Motif& Motif::operator+=(Motif const& rhs) {
       for (auto const& el : rhs.liste) liste.push_back(el);
       return *this;
    }
    Motif& Motif::operator+=(std::pair<size_t,size_t> const& p) {
       for (auto& el : liste) {
             el.first += p.first;
             el.second += p.second;
       }
       return *this;
    }
    Motif& Motif::operator-=(std::pair<size_t,size_t> const& p) {
       size_t l(min_ligne()), c(min_colonne());
       if (p.first>l) {
             for (auto& el : liste) el.first -= l;
       } else {
             for (auto& el : liste) el.first -= p.first;
       }
       if (p.second>c) {
             for (auto& el : liste) el.second -= c;
       } else {
             for (auto& el : liste) el.second -= p.second;
       }
       return *this;
    }
    Motif operator+(Motif lhs, Motif const& rhs) {return lhs+=rhs;}
    Motif operator+(Motif lhs, std::pair<size_t,size_t> const& rhs) {return lhs+=rhs;}

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
