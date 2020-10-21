#pragma once
#ifndef MOTIFS_H
#define MOTIFS_H
#include <array>
#include <vector>

typedef std::pair<size_t,size_t> coord;
typedef std::vector<coord> liste;

class Motif {
private :
    liste cellules;

    // Methodes utilitaires
    size_t min_ligne() const;
    size_t min_colonne() const;
public :
    // Constructeurs & Destructeurs
    Motif() = default;
    Motif(Motif const&) = default;
    Motif(std::initializer_list<coord> L);
    Motif(liste const& L);
    /*
     *  @brief  Construit à partir un Motif enregistré sous la forme d'un fichier .csv
     *  @param  fichier nom du fichier sans extension
     */
    Motif(std::string const& fichier);

    // Méthodes de modification
    void push_back(coord const& p) {cellules.push_back(p);}
    /*
     *  @brief  90° en sens horaire
     */
    void rotate();
    void rotate2();
    Motif& operator+=(Motif const& rhs);
    Motif& operator+=(coord const& p);
    Motif& operator-=(coord const& p);
    Motif& recalibrate();

    // Getters
    liste::iterator begin();
    liste::iterator end();
    liste::const_iterator cbegin() const;
    liste::const_iterator cend() const;
    size_t max_ligne() const;
    size_t max_colonne() const;
};
Motif operator+(Motif lhs, Motif const& rhs);
Motif operator+(Motif lhs, coord const& rhs);



struct Calque {
    Motif alive;
    std::pair<size_t, size_t> translate = {0,0};
    bool on_off = false;
};
void translate(Calque& calque);


/*
 *  @brief  Renvoie les coordonnees d'un segment reliant 2 points en utilisant l'algorithme de Bresenham
 *  @param x1,y1    coordonnees (dans [0,L]x[0,C]) du premier point
 *  @param x2,y2    coordonnees (dans [0,L]x[0,C]) du second point
 *  @returns    liste de coordonnees
 */
liste segment(size_t x1, size_t y1, size_t const& x2, size_t const& y2);
/*
 *  @brief  Renvoie les coordonnees d'un segment reliant 2 points en utilisant l'algorithme de Bresenham
 *  @param a    coordonnees (dans [0,L]x[0,C]) du premier point
 *  @param b    coordonnees (dans [0,L]x[0,C]) du second point
 *  @returns    liste de coordonnees
 */
liste segment(coord a, coord const& b);
#endif // MOTIFS_H
