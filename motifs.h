#pragma once
#ifndef MOTIFS_H
#define MOTIFS_H
#include <array>
#include <vector>

typedef std::vector<std::pair<size_t,size_t>> vec;
typedef std::vector<std::pair<int,int>> vec_neg;

namespace motifs {
    struct calque {
        vec_neg alive;
        bool on_off = false;
    };
    void translate(calque& calque);
    class Motif {
    private :
        vec liste;
        size_t min_ligne() const;
        size_t min_colonne() const;
        size_t max_ligne() const;
        size_t max_colonne() const;
    public :
        // Constructeurs & Destructeurs
        Motif() = default;
        Motif(Motif const&) = default;
        Motif(std::initializer_list<std::pair<size_t,size_t>> L) : liste(L) {}

        // Méthodes de modification
        void push_back(std::pair<size_t,size_t> const& p) {liste.push_back(p);}
        void rotate();      // 90° en sens horaire (anti trigo)

        void rotate2();
        Motif& operator+=(Motif const& rhs);
        Motif& operator+=(std::pair<size_t,size_t> const& p);
        Motif& operator-=(std::pair<size_t,size_t> const& p);
        Motif& recalibrate() {(*this)-={min_ligne(), min_colonne()}; return *this;}

        // Getters
        vec::iterator begin() {return liste.begin();}
        vec::iterator end() {return liste.end();}
        vec::const_iterator cbegin() const {return liste.cbegin();}
        vec::const_iterator cend() const {return liste.cend();}
    };
    Motif operator+(Motif lhs, Motif const& rhs);
    Motif operator+(Motif lhs, std::pair<size_t,size_t> const& rhs);

    // Modèles déjà créés
    const Motif grenouille({{1,1},{1,2},{1,3},{2,0},{2,1},{2,2}});
    const Motif planeur({{0,1},{1,2},{2,0},{2,1},{2,2}});
    const Motif gland({{0,1},{1,3},{2,0},{2,1},{2,4},{2,5},{2,6},{2,7}});
    const Motif canon({{5,0},{5,1},{6,0},{6,1},
    {5,10},{6,10},{7,10},{4,11},{3,12},{3,13},{4,15},{5,16},{6,16},{7,16},{6,17},{6,14},{8,15},{9,13},{9,12},{8,11}
    });
    const Motif lievres({{1,0},{1,2},{2,1},{3,1},{0,4},{0,6},{1,5},{2,5},{3,7}});

    // Construction de motifs géométriques
    Motif rectangle(size_t lignes, size_t colonnes);
    Motif ligneh(size_t longueur);
    Motif lignev(size_t longueur);
} // namespace motifs


#endif // MOTIFS_H
