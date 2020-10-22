#pragma once
#ifndef MOTIFS_H
#define MOTIFS_H
#include <array>
#include <vector>

typedef std::pair<size_t,size_t> coord;
typedef std::vector<coord> liste;

class Motif {
private :
    /*
     *  @brief  liste des cellules vivantes composant le motif (dans une grille potentiellement inifnie)
     */
    liste cellules;

public :
    // Constructeurs & Destructeurs
    /*
     *  @brief  cree un motif vide
     */
    Motif() = default;
    Motif(Motif const&) = default;
    /*
     *  @brief  cree un motif compose des coordonnees donnees en argument
     */
    Motif(std::initializer_list<coord> L);
    /*
     *  @brief  cree un motif compose des coordonnees donnees en argument
     */
    Motif(liste const& L);
    /*
     *  @brief  Construit à partir un Motif enregistré sous la forme d'un fichier .csv. Si ce fichier n'exitse pas, construit un motif vide
     *  @param  fichier nom du fichier sans extension
     */
    Motif(std::string const& fichier);

    // Méthodes de modification
    /*
     *  @brief  Rotation de 90° en sens horaire, autour du point (0,0)
     *  @returns    reference sur l'instance courante
     */
    void rotate();
    /*
     *  @brief  En construction
     *  @returns    reference sur l'instance courante
     */
    void rotate2();
    /*
     *  @brief  rajoute une cellule au motif
     *  @param  p   cellule a rajouter
     */
    void push_back(coord const& p);
    /*
     *  @brief  Concatene la listes d'un autre motif
     *  @param  autre  motif a ajouter
     *  @returns    retourne une reference sur l'instance courante
     */
    Motif& append(Motif const& autre);
    /*
     *  @brief  Translate le motif. Si le resultat doit avoir des coordonnees positives, reduit la translation
     *  @param  x,y coordonnees du vecteur par lequel on translate
     *  @returns    retourne une reference sur l'instance courante
     */
    Motif& translate(int const& x, int const& y);
    /*
     *  @brief  Translate le motif. Si le resultat doit avoir des coordonnees positives, reduit la translation
     *  @param  p   vecteur de translation
     *  @returns    retourne une reference sur l'instance courante
     */
    Motif& translate(std::pair<int,int> const& p);
    /*
     *  @brief  Translate le motif pour le rapprocher le plus possible de l'origine
     *  @returns    retourne une reference sur l'instance courante
     */
    Motif& recalibrate();

    // Getters
    /*
     *  @returns    Iterateur pointant sur le premier element cellule du motif
     */
    liste::iterator begin();
    /*
     *  @returns    Iterateur pointant sur le dernier element cellule du motif
     */
    liste::iterator end();
    /*
     *  @returns    Iterateur constant pointant sur le premier element cellule du motif
     */
    liste::const_iterator cbegin() const;
    /*
     *  @returns    Iterateur constant pointant sur le dernier element cellule du motif
     */
    liste::const_iterator cend() const;
    /*
     *  @returns    la plus petite ligne d'une cellule du motif
     */
    size_t min_ligne() const;
    /*
     *  @returns    la plus petite colonne d'une cellule du motif
     */
    size_t min_colonne() const;
    /*
     *  @returns    la plus grande ligne d'une cellule du motif
     */
    size_t max_ligne() const;
    /*
     *  @returns    la plus grande colonne d'une cellule du motif
     */
    size_t max_colonne() const;
};



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
