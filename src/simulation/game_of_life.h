#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H
#include <array>
#include <iostream>
#include "motifs.h"


class GameOfLife {
private :
    /*
     *  @brief  Nombre de Lignes et de Colonnes de la zone visible de la Grille
     */
    unsigned int C,L;
    /*
     *  @brief  Nombre de generations passees pendant l'evolution
     */
    unsigned int nbr_gen;
    /*
     *  @brief  Grille de 500x500 qui contient les informations des cellules. La zone visible doit avoir une taille max de 400x400 pour laisser des marges de 50 de chaque coté pour eviter les effets de bord
     */
    std::array<std::array<bool,500>,500> champ;
    /*
     *  @brief  Liste des coordonnees (dans [0,L+100]x[0,C+100]) des cellules vivantes a un instant donne.
     */
    liste vivantes;
    /*
     *  @brief  Liste des coordonnees (dans [0,L]x[0,C]) des cellules vivantes à l'interieur de la partie visible. Les coordonnees sont reconverties par rapport a l'origine de cette partie visible
     */
    liste vivantes_visibles;

    // Méthodes utilitaires =========================
    /*
     *  @brief  Calcule l'etat suivant d'une cellule (d'une grille potentiellement infinie)
     *  @param i,j coordonnees de la cellule
     *  @returns true si la cellule doit devenir vivante ou false si elle ne doit pas ou si elle est en dehors de la zone a faire evoluer
     */
    bool next_state(size_t i, size_t j);
    /*
     *  @brief  Calcule l'etat suivant d'une cellule (d'une grille potentiellement infinie)
     *  @param c coordonnees de la cellule
     *  @returns true si la cellule doit devenir vivante ou false si elle ne doit pas ou si elle est en dehors de la zone a faire evoluer
     */
    bool next_state(coord const& c);
    /*
     *  @brief  Verifie que la cellule indiquee est morte et a l'interieur de la grille, et si elle doit devenir vivante, la rajoute au tableau donne en parametres et a vivantes_visibles si elle est entre les bonnes bornes (en la translatant au prealable)
     *  @param  i,j coordoonnees (dans [0,L+100]x[0,C+100])
     *  @param  v,v_visibles    liste de cellules vivantes a mettre a jour
     */
    void verif(size_t const& i, size_t const& j, liste& v, liste& v_visibles);
public :
    // Constructeurs ========================================
    GameOfLife(Motif const& a_marquer, unsigned int const& C = 50,unsigned int const& L = 50);
    // modulos pour gérer les cas supérieurs à 400
    GameOfLife(unsigned int const& C = 50, unsigned int const& L = 50);
    // Getter & setters ========================================
    /*
     *  @returns	L'etat de la cellule si elle existe, et false sinon
     */
    bool access(size_t i, size_t j);
    /*
     *  @returns    La liste des cellules vivantes visibles avec coordonnees translatees dans [0,L]x[0,C]
     */
    liste const& get_viv() const;
    /*
     *  @brief   Verifie si la cellule indiquee est pas deja dans la grille et sinon l'y insere (et dans la liste des vivantes)
     *  @param   c  coordonnees par rapport a grille affichee (dans [0,L]x[0,C])
     */
    void add_cell(coord const& c);
    /*
     *  @brief   Verifie si la cellule indiquee est pas deja dans la grille et sinon l'y insere (et dans la liste des vivantes)
     *  @param   i,j    coordonnees par rapport a grille affichee (dans [0,L]x[0,C])
     */
    void add_cell(size_t const& i, size_t const& j);
    /*
     * @brief   Verifie si la cellule indiquee est bien vivante et si oui la supprime de la grille (et dans la liste des vivantes)
     * @param   c   coordonnees par rapport a grille affichee (dans [0,L]x[0,C]))
     */
    void suppr_cell(coord const& c);
    /*
     * @brief   Verifie si la cellule indiquee est bien vivante et si oui la supprime de la grille (et dans la liste des vivantes)
     * @param   i,j   coordonnees par rapport a grille affichee (dans [0,L]x[0,C]))
     */
    void suppr_cell(size_t const& i, size_t const& j);
    /*
     *  @brief  Inverse l'etat de la cellule et met a jour les attributes de GameOfLife
     *  @param  c   coordonnees par rapport a grille affichee (dans [0,L]x[0,C]))
     */
    void inv_cell(coord const& c);
    /*
     *  @brief  Inverse l'etat de la cellule et met a jour les attributes de GameOfLife
     *  @param  i,j   coordonnees par rapport a grille affichee (dans [0,L]x[0,C]))
     */
    void inv_cell(size_t const& i, size_t const& j);
    /*
     *  @brief  Ajoute un motif dans la grille a l'aide de add_cell()
     *  @param  m   Motif a rajouter
     */
    void add_motif(Motif const& m);
    /*
     *  @brief  Supprime les cellules de la grille contenues dans un motif a l'aide de suppr_cell()
     *  @param  m   Motif a enlever
     */
    void suppr_motif(Motif const& m);
    /*
     *  @brief  Efface toute la grille
     */
    void wipe();

    // Evolution de la simulaitton ==============================
    /*
     *  @brief  Fait evoluer la grille en la faisant passer a la generation suivantes et en updatant ses attributs
     */
    void evolve();
    /*
     *  @brief  Fait appel a la methode evolve() et calcules une simulation sur un nombre pre defini de generations, et l'enregistre en .csv. Si une simulation du meme nom existe dejé ne fait rien
     *  @param  nom_simulation  nom a donner a l'enregistrement de la simulation
     *  @param  duree_sim nombre de generations a simuler
     *  @param  categorie   precise dans quel dossier enregistrer la simulation ("presaved" pour presaved, n'importe quoi d'autre pour "local")
     *  @returns    true si la simulation a pu être créée, false sinon
     */
    bool life(std::string const& nom_simulation, unsigned int const& duree_sim, std::string const& categorie = "local");

    // Gestion des motifs ==============================
    /*
	 *  @brief	Enregistre un fichier .csv contenant les coordonnees du motif formé de toutes les cellules visibles dans la grille. Si un fichier du meme nom existe deja, l'ecrase
	 *  @param	nom_motif	Nom du fichier à créer
     *  @param  dossier Le repertoire dans lequel le fichier sera enregistre (si different de local, enregistre dans presaved)
     */
    void save_motif(std::string const& nom_motif, std::string const& dossier = "local") const;
    /*
     *  @brief  Enregistre un fichier .csv local contenant les coordonnees du motif forme de toutes les cellules visibles entre certaines bornes
     *  @param  imin,imax   Bornes sur les lignes (dans [0,L])
     *  @param  jmin,jmax   Bornes sur les colonnes (dans [0,C])
     *  @param  dossier Le repertoire dans lequel le fichier sera enregistre (si different de local, enregistre dans presaved)
     */
    void save_motif(std::string const& nom_motif, size_t imin, size_t imax, size_t jmin, size_t jmax, std::string const& dossier = "local") const;

    // Affichage ========================================
    /*
     *  @brief  Affiche en sortie standard (cli) le contenu du tableau
     *  @param  Flot de sortie
     */
    void print(std::ostream& out = std::cout) const;
};

// Gestion des simulations enregistrees
/*
 *  @returns    Liste des simulations (sans extension) de motifs enregistres localement
 */
std::vector<std::string> existing_local_sims();
std::vector<std::string> existing_presaved_sims();

#endif // GAME_OF_LIFE_H
