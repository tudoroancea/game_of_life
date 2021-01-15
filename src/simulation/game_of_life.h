#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#ifdef _WIN32
    #define CLEAR() std::stystem("cls");
#endif
#ifndef _WIN32
    #define CLEAR() std::system("clear");
#endif

#define MAX_LIGNES 400
#define MAX_COLONNES 400

#include <array>
#include <iostream>
#include "motifs.h"
#include "rapidcsv.h"

/**
 * @class GameOfLife
 * @brief Classe qui représente une grille de jeu de la vie qu'on peut faire évoluer, afficher, enregsitrer.
 */
class GameOfLife {
protected :
    /**
     *  @brief  Nombre de générations passées pendant l'evolution
     */
    unsigned int nbr_gen;
    /**
     *  @brief  Grille de (MAX_LIGNES+100)x(MAX_COLONNES+100) qui contient les informations des cellules.
     *  La zone qu'on peut manipuler est [50,MAX_LIGNES+50[x[50,MAX_COLONNES+50[ (les marges restantes sont là pour éviter les effets de bord).
     *  Ces manipulations via l'interface se font avec des coordonnées NE TENANT PAS COMPTE des marges (ie dans [0,MAX_LIGNES[x[0,MAX_COLONNES[ ) et qui sont ensuite translatées.
     */
    std::array<std::array<bool,MAX_LIGNES+100>,MAX_COLONNES+100> grille;
    /**
     *  @brief  Liste des coordonnees qui TIENNENT compte des marges (dans [0,MAX_LIGNES+100[x[0,MAX_COLONNES+100[) des cellules vivantes a un instant donne.
     */
    liste vivantes;

    // Méthodes utilitaires =========================
    /**
     * @brief Méthode utilitaire qui est utilisée comme raccourci dans evolve().
     * Elle vérifie si les cellules voisines de (i,j) n'étaient pas déjà vivantes et si elles doivent le devenir.
     * Si oui elle les rajoute à v
     *
     * @param i,j coordonnées de la cellule de base (TENANT COMPTE DES MARGES, ie dans [0,MAX_LIGNES+100[x[0,MAX_COLONNES+100[)
     * @param v liste des cellules vivantes à laquelle rajouter les voisines qui vont devenir vivantes.
     */
    void verif(size_t const& i, size_t const& j, liste& v);
    /**
     *  @brief  Calcule l'etat suivant d'une cellule (d'une grille potentiellement infinie)
     *  @param i,j coordonnees de la cellule (TENANT COMPTE DES MARGES, ie dans [0,MAX_LIGNES+100[x[0,MAX_COLONNES+100[)
     *  @returns true si la cellule doit devenir vivante ou false si elle ne doit pas ou si elle est en dehors de la zone a faire evoluer
     */
    bool next_state(size_t const& i, size_t const& j) const;
    /**
     *  @brief  Calcule l'etat suivant d'une cellule (d'une grille potentiellement infinie)
     *  @param c coordonnees de la cellule (TENANT COMPTE DES MARGES, ie dans [0,MAX_LIGNES+100[x[0,MAX_COLONNES+100[)
     *  @returns true si la cellule doit devenir vivante ou false si elle ne doit pas ou si elle est en dehors de la zone a faire evoluer
     */
    bool next_state(coord const& c);
    /**
     *  @returns	L'etat de la cellule si elle existe (true si elle est vivante, false si elle est morte), et false sinon
     */
    bool access(size_t const& i, size_t const& j) const;
public :
    // Constructeurs ========================================
    /**
     * @brief Construit un nouveau GameOfLife avec une grille vide.
     */
    GameOfLife();
    /**
     * @brief Construit un nouveau GameOfLife contenant déjà un certain Motif.
     * @param a_marquer Motif à dessiner à la création. Coordonnées dans [0,MAX_LIGNES[x[0,MAX_COLONNES[ qui correspondent à la zone sans marge de la grille
     */
    GameOfLife(Motif const& a_marquer);

    // Getters ========================================
    /**
     *  @return    La liste des cellules vivantes visibles avec coordonnees TENANT COMPTE DES MARGES (ie dans [0,MAX_LIGNES+100[x[0,MAX_COLONNES+100[)
     */
    liste const& get_viv() const;
    /**
     * @return référence constante sur la grille des cellules
     */
    std::array<std::array<bool,500>,500> const& get_grille() const;
    /**
     *  @returns    référence sur le nombre de générations
     *  @brief  (on renvoie une référence pour incrémenter le nombre de générations dans …)
     */
    unsigned int& get_nbr_gen();

    // Setters ==========================================
    /**
     *  @brief   Vérifie si la cellule indiquée est pas déjà dans la grille et sinon l'y insère (et dans la liste des vivantes)
     *  @param   c  coordonnées NE TENANT PAS COMPTE DES MARGES (ie dans [0,MAX_LIGNES[x[0,MAX_COLONNES[ )
     *  @returns    référence sur l'instance courante
     */
    GameOfLife& add_cell(coord const& c);
    /**
     *  @brief   Vérifie si la cellule indiquée est pas déjà dans la grille et sinon l'y insére (et dans la liste des vivantes)
     *  @param   i,j    coordonnées NE TENANT PAS COMPTE DES MARGES (ie dans [0,MAX_LIGNES[x[0,MAX_COLONNES[ )
     *  @returns    référence sur l'instance courante
     */
    GameOfLife& add_cell(size_t const& i, size_t const& j);
    /**
     *  @brief   Vérifie si la cellule indiquée est bien vivante et si oui la supprime de la grille (et de la liste des vivantes)
     *  @param   c   coordonnées NE TENANT PAS COMPTE DES MARGES (ie dans [0,MAX_LIGNES[x[0,MAX_COLONNES[ )
     *  @returns    référence sur l'instance courante
     */
    GameOfLife& suppr_cell(coord const& c);
    /**
     *  @brief   Vérifie si la cellule indiquée est bien vivante et si oui la supprime de la grille (et dans la liste des vivantes)
     *  @param   i,j   coordonnées NE TENANT PAS COMPTE DES MARGES (ie dans [0,MAX_LIGNES[x[0,MAX_COLONNES[ )
     *  @returns    référence sur l'instance courante
     */
    GameOfLife& suppr_cell(size_t const& i, size_t const& j);
    /**
     *  @brief  Inverse l'état de la cellule et met à jour les attributs de l'instance
     *  @param  c   coordonnées NE TENANT PAS COMPTE DES MARGES (ie dans [0,MAX_LIGNES[x[0,MAX_COLONNES[ )
     *  @returns    référence sur l'instance courante
     */
    GameOfLife& inv_cell(coord const& c);
    /**
     *  @brief  Inverse l'état de la cellule et met à jour les attributes de GameOfLife
     *  @param  i,j   coordonnées NE TENANT PAS COMPTE DES MARGES (ie dans [0,MAX_LIGNES[x[0,MAX_COLONNES[ )
     *  @returns    référence sur l'instance courante
     */
    GameOfLife& inv_cell(size_t const& i, size_t const& j);
    /**
     *  @brief  Ajoute un motif dans la grille à l'aide de add_cell(). Si ce motif dépasse, ne dessine que ce qui ne dépasse pas.
     *  @param  m   Motif à rajouter (avec coordonnées comprises comme NE TENANT PAS COMPTE DES MARGES (ie dans [0,MAX_LIGNES[x[0,MAX_COLONNES[ ))
     *  @returns    référence sur l'instance courante
     */
    GameOfLife& add_motif(Motif const& m);
    /**
     *  @brief  Supprime les cellules de la grille contenues dans un motif a l'aide de suppr_cell()
     *  @param  m   Motif à enlever (avec coordonnées comprises comme NE TENANT PAS COMPTE DES MARGES (ie dans [0,MAX_LIGNES[x[0,MAX_COLONNES[ ))
     *  @returns    référence sur l'instance courante
     */
    GameOfLife& suppr_motif(Motif const& m);
    /**
     *  @brief  Efface toute la grille
     *  @returns    référence sur l'instance courante
     */
    GameOfLife& wipe();

    // Evolution de la simulaitton ==============================
    /**
     *  @brief  Fait evoluer la grille en la faisant passer a la génération suivante et en updatant ses attributs
     */
    void evolve();

    // Enregistrement de motifs et simulaions  ==============================
    /**
	 *  @brief	Enregistre un fichier .csv contenant les coordonnées du motif formé de toutes les cellules vivantes de la grille. Si un fichier du même nom existe deja, l'écrase
	 *  @param	nom_motif	Nom du fichier à créer.
     *  @param  categorie La catégorie dans lequel le fichier sera enregistré.
     */
    void save_motif(std::string const& nom_motif, FILE_CATEGORY const& categorie = local) const;
    /**
     *  @brief  Fait appel à la methode evolve() et calcules une simulation sur un nombre pré-défini de generations, et l'enregistre en .csv. Si une simulation du même nom existe déjà ne fait rien.
     *  Voir le fichier convention.md pour la structure de ces fichiers.
     *  @param  nom_simulation  nom à donner à l'enregistrement de la simulation
     *  @param  duree_sim nombre de générations à simuler
     *  @param  FILE_CAT   La catégorie dans lequel le fichier sera enregistré.
     *  @returns    true si la simulation a pu être créée, false sinon
     */
    bool save_sim(std::string const& nom_simulation, unsigned int const& duree_sim, FILE_CATEGORY const& categorie = local);

    // Affichage ========================================
    /**
     *  @brief  Affiche en sortie standard (texte, cli) le contenu de la grille (sans marges)
     *  @param  out Flot de sortie sur lequel afficher
     */
    void print(std::ostream& out = std::cout) const;

    // Informations sur la grille ======================================================
    /**
     * @return Nombre de cellules vivantes
     */
    unsigned int nbr_cell() const;
    /**
     * @brief Utilise l'algorithme sparseCLL du CERN pour trouver les composantes connexes
     *
     * @return std::vector<size_t>
     */
    std::vector<size_t> sparseCLL();
};

// Gestion des simulations enregistrees
/**
 *  @returns    Liste des simulations (sans extension) de motifs enregistres localement
 */
std::vector<std::string> existing_local_sims();
std::vector<std::string> existing_presaved_sims();

/**
 * @class   GameOfLifeView
 * @brief   Sous-classe de GameOfLife qui représente une certaine portion (rectangulaire) de la grille qui est vue et avec laquelle on peut intéragir
 */
class GameOfLifeView : public GameOfLife {
    private :
        /**
         * @brief Coordonnées NE TENANT PAS COMPTE DES MARGES (ie dans [0,MAX_LIGNES[x[0,MAX_COLONNES[ ) qui représentent la zone visible de la grille.
         * On peut modifier le contenu de celle-ci via l'interface en utilisant des coordonnées dans [0,Lmax-Lmin[x[0,Cmax-Cmin[.
         */
        unsigned int Lmin, Lmax, Cmin, Cmax;
        /**
         * @brief Liste des coordonnées (dans [0,Lmax-Lmin[x[0,Cmax-Cmin[ ) des cellules qui sont vivantes et visibles à un instant donné.
         */
        liste vivantes_visibles;

        // Méthodes utilitaires ============================================================
        /**
         * @brief Méthode utilitaire qui est utilisée comme raccourci dans evolve(). ELle masque celle de la super-classe GameOfLife.
         * Elle vérifie si les cellules voisines de (i,j) n'étaient pas déjà vivantes et si elles doivent le devenir.
         * Si oui elle les rajoute à v et à v_visibles.
         *
         * @param i,j coordonnées de la cellule de base (TENANT COMPTE DES MARGES, ie dans [0,MAX_LIGNES+100[x[0,MAX_COLONNES+100[)
         * @param v liste des cellules vivantes à laquelle rajouter les voisines qui vont devenir vivantes.
         * @param v_visibles    liste des cellules vivantes visibles à laquelle rajouter les voisines qui vont devenir vivantes et visibles.
         */
        void verif(size_t const& i, size_t const& j, liste& v, liste& v_visibles);
    public :
        // Constructeurs ============================================================
        /**
         * @brief Construit un nouvel GameOfLifeView avec vue dans la fenêtre [Lmin, Lmax[x[Cmin, Cmax[.
         * @param lmin futur Lmin
         * @param lmax futur Lmax
         * @param cmin futur Cmin
         * @param cmax futur Cmax
         */
        GameOfLifeView(unsigned int const& lmin = MAX_LIGNES/2-24,
                       unsigned int const& lmax = MAX_LIGNES/2+26,
                       unsigned int const& cmin = MAX_COLONNES/2-24,
                       unsigned int const& cmax = MAX_COLONNES/2+26);

        // Getters  ==================================================================
        /**
         * @brief masque la version de la super-classe GameOfLife pour ne renvoyer que les cellules vivantes visibles (avec les coordonnées dans [0,Lmax-Lmin[x[0,Cmax-Cmin[.
         * @return référence constante sur la liste des vivantes visibles
         */
        liste const& get_viv() const;
        /**
         * @return nombre de cellules vivantes visibles
         */
        unsigned int nbr_cell() const;
        /**
         * @return nombre de lignes dans la partie visible (Lmax-Lmin)
         */
        unsigned int nbr_lignes() const;
        /**
         * @return nombre de colonnes dans la partie visible (Cmax-Cmin)
         */
        unsigned int nbr_colonnes() const;
        unsigned int get_Lmin() const;
        unsigned int get_Cmin() const;
        unsigned int get_Lmax() const;
        unsigned int get_Cmax() const;
        // Setters du jeu =======================================================================
        /**
         *  @brief   Vérifie si la cellule indiquée n'est pas déjà dans la grille et sinon l'y insère. Ne marche que pour des cellules dans la partie visible.
         *  @param   c  coordonnées par rapport à la partie visible (ie dans [0,Lmax-Lmin[x[0,Cmax-Cmin[ )
         *  @returns référence sur l'instance courante
         */
        GameOfLifeView& add_cell(coord const& c);
        /**
         *  @brief   Vérifie si la cellule indiquée n'est pas déjà dans la grille et sinon l'y insère. Ne marche que pour des cellules dans la partie visible.
         *  @param   i,j  coordonnées par rapport à la partie visible (ie dans [0,Lmax-Lmin[x[0,Cmax-Cmin[ )
         *  @returns référence sur l'instance courante
         */
        GameOfLifeView& add_cell(size_t const& i, size_t const& j);
        /**
         *  @brief   Vérifie si la cellule indiquée est bien vivante dans la grille et si oui la supprime. Ne marche que pour des cellules dans la partie visible.
         *  @param   c  coordonnées par rapport à la partie visible (ie dans [0,Lmax-Lmin[x[0,Cmax-Cmin[ )
         *  @returns référence sur l'instance courante
         */
        GameOfLifeView& suppr_cell(coord const& c);
        /**
         *  @brief   Vérifie si la cellule indiquée est bien vivante dans la grille et si oui la supprime. Ne marche que pour des cellules dans la partie visible.
         *  @param   i,j  coordonnées par rapport à la partie visible (ie dans [0,Lmax-Lmin[x[0,Cmax-Cmin[ )
         *  @returns référence sur l'instance courante
         */
        GameOfLifeView& suppr_cell(size_t const& i, size_t const& j);
        /**
         *  @brief  Inverse l'état de la cellule. Ne marche que pour des cellules dans la partie visible.
         *  @param   c  coordonnées par rapport à la partie visible (ie dans [0,Lmax-Lmin[x[0,Cmax-Cmin[ )
         *  @returns référence sur l'instance courante
         */
        GameOfLifeView& inv_cell(coord const& c);
        /**
         *  @brief  Inverse l'état de la cellule. Ne marche que pour des cellules dans la partie visible.
         *  @param   i,j  coordonnées par rapport à la partie visible (ie dans [0,Lmax-Lmin[x[0,Cmax-Cmin[ )
         *  @returns référence sur l'instance courante
         */
        GameOfLifeView& inv_cell(size_t const& i, size_t const& j);
        /**
         *  @brief  Ajoute un motif dans la partie visible de la grille à l'aide de add_cell(). Si ce motif dépasse, ne dessine que ce qui ne dépasse pas.
         *  @param  m   Motif à rajouter (avec coordonnées par rapport à la partie visible (ie dans [0,Lmax-Lmin[x[0,Cmax-Cmin[ ))
         *  @returns    référence sur l'instance courante
         */
        GameOfLifeView& add_motif(Motif const& m);
        /**
         *  @brief  Supprime les cellules de la partie visible de la grille contenues dans un motif a l'aide de suppr_cell().
         *  @param  m   Motif à enlever (avec coordonnéespar rapport à la partie visible (ie dans [0,Lmax-Lmin[x[0,Cmax-Cmin[ ))
         *  @returns    référence sur l'instance courante
         */
        GameOfLifeView& suppr_motif(Motif const& m);
        /**
         *  @brief  Efface toute la partie visible de la grille.
         *  @returns    référence sur l'instance courante
         */
        GameOfLifeView& wipe();

        // Evolution du jeu ==========================================================================================
        /**
         *  @brief  Fait evoluer toute la grille en la faisant passer a la génération suivante et en updatant ses attributs.
         *  Masque la version de la super-classe GameOfLife pour aussi mettre à jour la liste des cellules vivantes et visibles.
         */
        void evolve();

        // Modification de la vue ====================================================================================
        /**
         *  @brief  Redimensionne la partie visible de la grille et met à jour la liste des cellules vivantes visibles
         *  @param  lmin    nouveau Lmin (dans [0,MAX_LIGNES[)
         *  @param  Lmax    nouveau Lmax (dans [0,MAX_LIGNES[)
         *  @param  Cmin    nouveau Cmin (dans [0,MAX_COLONNES[)
         *  @param  Cmax    nouveau Cmax (dans [0,MAX_COLONNES[)
         *  @returns    reference sur l'instance courante
         */
        GameOfLifeView& resize(unsigned int const& lmin, unsigned int const& lmax, unsigned int const& cmin, unsigned int const& cmax);
        /**
         * @brief   Translate la partie visible d'un certain vecteur et met à jour la liste des cellules vivantes visibles
         * @param   l   nombre de lignes à rajouter (négatif si on veut en enlever)
         * @param   c   nombre de colonnes à rajouter (négatif si on veut en enlever)
         * @return  référence sur l'instance courante
         */
        GameOfLifeView& translate(int const& l, int const& c);
        /**
         * @brief Zoom/dézoom la vue de manière fixe, en réduisant/augmentant la partie visible de 10 de chaque côté
         * @param grossissment  zoom si positif, dézoom si négatif
         * @return GameOfLifeView&
         */
        GameOfLifeView& zoom(double const& grossissement);
        /**
         * @brief Zoom la vue autour d'un certain point
         * @param centre    coordonnées par rapport à la grille (ie dans [0,Lmax-Lmin[x[0,Cmax-Cmin[) du centre du zoom
         * @param grossissement  grossissement du zoom. Zoom in si entre 0 et 1, zoom out (dézoom) si supérieur à 1.
         * @return  référence sur l'instance courante
         */
        GameOfLifeView& zoom(coord const& centre, double grossissement);
        /**
         * @brief Zoom la vue autour d'un certain point
         * @param x,y    coordonnées par rapport à la grille (ie dans [0,Lmax-Lmin[x[0,Cmax-Cmin[) du centre du zoom
         * @param grossissement  grossissement du zoom. Zoom in si entre 0 et 1, zoom out (dézoom) si supérieur à 1.
         * @return  référence sur l'instance courante
         */
        GameOfLifeView& zoom(size_t const& x, size_t const& y, double grossissement);

        // Affichage ====================================================================================
        /**
         *  @brief  Affiche en sortie standard (texte, cli) la partie visible de la grille
         *  @param  out Flot de sortie sur lequel afficher
         *  @param  avec_grille si true affiche également la grille entière à droite de la partie visible.
         */
        void print(std::ostream& out = std::cout, bool avec_grille = false) const;
};

/**
 *  @class  Simulation
 *  @brief  API qui permet de charger une simulation déjà existante enregistrée localement et de l'afficher.
 */
class Simulation {
    private :
        /**
         *  @brief  nom de la simulation
         */
        std::string nom;
        /**
         *  @brief  GOL qui contiendra les cellules à afficher
         */
        GameOfLifeView* grille;
        /**
         *  @brief  Nombre de générations que doit durer la simulation
         */
        unsigned int nbr_gen;
        /**
         *  @brief  path du fichier de configuration <nom_sim>-info.csv
         */
        std::filesystem::path info_path;
        /**
         *  @brief  Document rapidcsv pour parse le fichier <nom_sim>-info.csv
         */
        rapidcsv::Document info_file;
        /**
         *  @brief  path du fichier de contenu <nom_sim>-content.csv
         */
        std::filesystem::path content_path;
        /**
         *  @brief  Document rapidcsv pour parse le fichier <nom_sim>-content.csv
         */
        rapidcsv::Document content_file;
    public :
        /**
         *  @brief  types d'erreurs à lancer pour être attrapées lors de la création de simulations
         */
        enum Error{NON_EXISTING_SIM, NON_EXISTING_INFO, NON_EXISTING_CONTENT, INCOMPLETE_INFO, INCOMPLETE_CONTENT, INCOMPATIBLE_DIMENSIONS};

        // Constructeurs & Destructeurs ========================================================================================================
        /**
         *  @brief  construit une simulation vide (pointeur grille NULL, pas de nom et path sur DATA_PATH)
         */
        Simulation();
        /**
         *  @brief  remplit la simulation des informations nécessaires. Lance si nécessaire une Simulation::Error
         *  @param  nom_sim nom de la simulation à chercher
         *  @param  categorie   dossier où chercher
         *  @returns    référence sur l'instance courante
         */
        Simulation& load(std::string const& nom_sim, FILE_CATEGORY const& categorie = local);
        Simulation& release();
        ~Simulation();

        // Getters ========================================================================================================
        /**
         *  @brief  pretty self-explanatory
         */
        std::string const& get_nom() const;
        /**
         *  @brief  S'il y a un problème d'accès au fichiers, lance une erreur INCOMPLETE_INFO ou INCOMPLETE_CONTENT
         *  @param  num_gen numero de la génération
         *  @returns    motif numero num_gen
         */
        Motif get_motif(unsigned int const& num_gen) const;
        /**
         *  @returns    la liste des cellules vivantes visibles de la grille sous-jacente
         */
        liste get_viv() const;
        /**
         *  @returns    true si la simulation est terminée (il n'y a plus de motifs à charger), false sinon
         */
        bool finie() const;

        // Evolution de la grille ========================================================================================================
        /**
         *  @brief  fait évoluer la grille en chargeant le motif suivant
            *  @returns    true si l'évolution a pu se faire, false si la simulation est arrivée à la fin
         */
        bool evolve();
        /**
         * @brief Fait appel à la méthode translate de grille.
         * @param l
         * @param c
         * @return Simulation&
         */
        Simulation& translate(int const& l, int const& c);
        Simulation& resize(unsigned int const& lmin, unsigned int const& lmax, unsigned int const& cmin, unsigned int const& cmax);

        // Affichage ========================================================================================================
        /**
         *  @brief  fait appel à la méthode print() de grille pour l'afficher
         *  @param  out flot de sortie sur lequel afficher la grille
         */
        void print(std::ostream& out = std::cout, bool avec_grille = false) const;

};


/**
 * @brief   Renvoie une repartition des cellules telle que chaque morceau est connexe (au sens des voisins)
 *
 * @return std::vector<Motif>
 */
std::vector<Motif> composants_connexes(GameOfLife const& jeu);



#endif // GAME_OF_LIFE_H
