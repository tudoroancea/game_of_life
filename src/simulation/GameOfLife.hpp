#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#ifdef _WIN32
#define CLEAR() std::system("cls");
#endif
#ifndef _WIN32
#define CLEAR() std::system("clear");
#endif

#include <array>
#include <iostream>
#include <unordered_map>
#include "Motif.hpp"
#include "rapidcsv.h"
#include "Cell.hpp"

/**
 * @brief Specialization of std::hash for pairs. Weak version due to the XOR
 */
struct pair_hash {
	template<class T1, class T2>
	std::size_t operator()(std::pair<T1,T2> const& p) const;
};

/**
 * @brief Sert de type de retour pour des méthodes de GameOfLife et GameOfLifeView pour indiquer à MainWindow quelles cellules il faut supprimer et quelles cellules il faut ajouter.
 * Cela évite de recharger toute la grille si on a modifié que peu de cellules
 */
struct golChange {
	Motif toAdd;
	Motif toDelete;
};

/**
 * @class GameOfLife
 * @brief Classe qui représente une grille_ de jeu de la vie qu'on peut faire évoluer, afficher, enregsitrer.
 */
class GameOfLife {
protected :
	/**
	 *  @brief  Nombre de générations passées pendant l'évolution
	 */
	unsigned int generation_;
	/**
	 *  @brief  Grille de (MAX_LIGNES+100)x(MAX_COLONNES+100) qui contient les informations des cellules.
	 *  La zone qu'on peut manipuler est [50,MAX_LIGNES+50[x[50,MAX_COLONNES+50[ (les marges restantes sont là pour éviter les effets de bord).
	 *  Ces manipulations via l'interface se font avec des coordonnées NE TENANT PAS COMPTE des marges (ie dans [0,MAX_LIGNES[x[0,MAX_COLONNES[ ) et qui sont ensuite translatées.
	 */
	std::array<std::array<Cell,MAX_LIGNES+100>,MAX_COLONNES+100> grille_;
	/**
	 *  @brief  Liste des coordonnees qui TIENNENT compte des marges (dans [0,MAX_LIGNES+100[x[0,MAX_COLONNES+100[) des cellules vivantes_ a un instant donne.
	 */
	Liste vivantes_;
	
	// Méthodes utilitaires =========================
	/**
	 * @brief Méthode utilitaire qui est utilisée comme raccourci dans evolve().
	 * Elle vérifie si les cellules voisines de (i,j) n'étaient pas déjà vivantes_ et si elles doivent le devenir.
	 * Si oui elle les rajoute à v
	 *
	 * @param i,j coordonnées de la cellule de base (TENANT COMPTE DES MARGES, ie dans [0,MAX_LIGNES+100[x[0,MAX_COLONNES+100[)
	 * @param v Liste des cellules vivantes_ à laquelle rajouter les voisines qui vont devenir vivantes_.
	 */
	void verif(size_t const& i, size_t const& j, Liste& v, Liste& n_n);
	/**
	 *  @brief  Calcule l'etat suivant d'une cellule (d'une grille_ potentiellement infinie)
	 *  @param i,j coordonnees de la cellule (TENANT COMPTE DES MARGES, ie dans [0,MAX_LIGNES+100[x[0,MAX_COLONNES+100[)
	 *  @returns true si la cellule doit devenir vivante ou false si elle ne doit pas ou si elle est en dehors de la zone a faire evoluer
	 */
	bool next_state(size_t const& i, size_t const& j) const;
	
	/**
	 *  @returns	L'etat de la cellule si elle existe (true si elle est vivante, false si elle est morte), et false sinon
	 */
	bool at(size_t const& i, size_t const& j) const;
	
	/**
	 * @brief Utility array encapsulating the directions of the neighbors of a cell (used in the CC search)
	 */
	const std::array<int,8> dx = {1,1,1,0,0,-1,-1,-1};
	/**
	 * @brief Utility array encapsulating the directions of the neighbors of a cell (used in the CC search)
	 */
	const std::array<int,8> dy = {1,0,-1,1,-1,1,0,-1};

private :
	// Recherche et analyse des structures et composantes connexes de la grille_ ============================================================
	void dfs(std::unordered_map<Coord, size_t, pair_hash>& labels, size_t x, size_t y, size_t label) const;

public :
	// Constructeurs ========================================
	/**
	 * @brief Construit un nouveau GameOfLife avec une grille_ vide.
	 */
	GameOfLife();
	/**
	 * @brief Construit un nouveau GameOfLife contenant déjà un certain Motif.
	 * @param a_marquer Motif à dessiner à la création. Coordonnées dans [0,MAX_LIGNES[x[0,MAX_COLONNES[ qui correspondent à la zone sans marge de la grille_
	 */
	GameOfLife(Motif const& a_marquer);
	
	virtual ~GameOfLife() = default;
	
	// Getters ========================================
	/**
	 *  @return    La Liste des cellules vivantes_ visibles avec coordonnees TENANT COMPTE DES MARGES (ie dans [0,MAX_LIGNES+100[x[0,MAX_COLONNES+100[)
	 */
	virtual Liste const& vivantes() const;
	/**
	 * @return référence constante sur la grille_ des cellules
	 */
	[[maybe_unused]] const std::array<std::array<Cell, MAX_LIGNES + 100>, MAX_COLONNES + 100>& grille() const;
	/**
	 *  @returns    référence sur le nombre de générations
	 *  @brief  (on renvoie une référence pour incrémenter le nombre de générations dans Simulation::evolve() )
	 */
	unsigned int& generation();
	/**
	 * @return Nombre de cellules vivantes_
	 */
	virtual unsigned int nbCells() const;
	/**
 * @brief Direct recursive method to find the number of connected components (CC) among the living cells
 * @return the number of connected components
 */
	size_t nbr_CC_rec() const;
	/**
	 * @brief Direct iterative method to find the number of connected components (CC) among the living cells
	 * @return the number of connected components
	 */
	size_t nbr_CC_ite() const;
	/**
	 * @brief Direct recursive method to find the connected components (CC) among the living cells
	 * @return a list of connected components
	 */
	std::vector<Motif> find_CC_rec() const;
	/**
	 * @brief Direct iterative method to find the connected components (CC) among the living cells
	 * @return a list of connected components
	 */
	std::vector<Motif> find_CC_ite() const;
	
	// Setters ==========================================
	/**
	 *  @brief   Vérifie si la cellule indiquée est pas déjà dans la grille_ et sinon l'y insére (et dans la Liste des vivantes_)
	 *  @param   i,j    coordonnées NE TENANT PAS COMPTE DES MARGES (ie dans [0,MAX_LIGNES[x[0,MAX_COLONNES[ )
	 *  @returns    référence sur l'instance courante
	 */
	virtual bool addCell(size_t const& i, size_t const& j);
	/**
     *  @brief   Vérifie si la cellule indiquée est bien vivante et si oui la supprime de la grille_ (et dans la Liste des vivantes_)
     *  @param   i,j   coordonnées NE TENANT PAS COMPTE DES MARGES (ie dans [0,MAX_LIGNES[x[0,MAX_COLONNES[ )
     *  @returns    référence sur l'instance courante
     */
	virtual bool deleteCell(size_t const& i, size_t const& j);
	/**
     *  @brief  Inverse l'état de la cellule et met à jour les attributes de GameOfLife
     *  @param  i,j   coordonnées NE TENANT PAS COMPTE DES MARGES (ie dans [0,MAX_LIGNES[x[0,MAX_COLONNES[ )
     *  @returns    référence sur l'instance courante
     */
	virtual bool inverseCell(size_t const& i, size_t const& j);
	/**
	 *  @brief  Ajoute un motif dans la grille_ à l'aide de addCell(). Si ce motif dépasse, ne dessine que ce qui ne dépasse pas.
	 *  @param  motif   Motif à rajouter (avec coordonnées comprises comme NE TENANT PAS COMPTE DES MARGES (ie dans [0,MAX_LIGNES[x[0,MAX_COLONNES[ ))
	 *  @returns    référence sur l'instance courante
	 */
	virtual std::vector<bool> addMotif(Motif const& motif);
	/**
     *  @brief  Supprime les cellules de la grille_ contenues dans un motif a l'aide de deleteCell()
     *  @param  motif   Motif à enlever (avec coordonnées comprises comme NE TENANT PAS COMPTE DES MARGES (ie dans [0,MAX_LIGNES[x[0,MAX_COLONNES[ ))
     *  @returns    référence sur l'instance courante
     */
	virtual std::vector<bool> deleteMotif(Motif const& motif);
	/**
     *  @brief  Efface toute la grille_
     *  @returns    référence sur l'instance courante
     */
	virtual golChange wipe();
	
	/**
     *  @brief  Fait evoluer la grille_ en la faisant passer a la génération suivante et en updatant ses attributs
     */
	virtual golChange evolve();
	
	// Enregistrement de motifs et simulaions  ==============================
	/**
	 *  @brief	Enregistre un fichier .csv contenant les coordonnées du motif formé de toutes les cellules vivantes_ de la grille_. Si un fichier du même nom existe deja, ne fait rien
	 *  @param	nomMotif	Nom du fichier à créer.
	 *  @param  categorie La catégorie dans lequel le fichier sera enregistré.
	 *  @return true si on a réussi à enregistrer le motif, false sinon
	 */
	virtual bool saveMotif(std::string const& nomMotif, FILE_CATEGORY const& categorie = local) const;
	/**
	 *  @brief  Fait appel à la methode evolve() et calcule une simulation sur un nombre pré-défini de generations, et l'enregistre en .csv. Si une simulation du même nom existe déjà ne fait rien.
	 *  Voir le fichier convention.md pour la structure de ces fichiers.
	 *  @param  nom  nom à donner à l'enregistrement de la simulation
	 *  @param  duree nombre de générations à simuler
	 *  @param  FILE_CAT   La catégorie dans lequel le fichier sera enregistré.
	 *  @returns    true si la simulation a pu être créée, false sinon
	 */
	bool saveSim(std::string const& nom, unsigned int const& duree, FILE_CATEGORY const& categorie = local);
	
	// Affichage ========================================
	/**
	 *  @brief  Affiche en sortie standard (texte, cli) le contenu de la grille_ (sans marges)
	 *  @param  out Flot de sortie sur lequel afficher
	 */
	void print(std::ostream& out = std::cout) const;
};

// Gestion des simulations enregistrées =============================================
/**
 *  @returns    Liste des simulations (sans extension) de motifs enregistres localement
 */
std::vector<std::string> existingLocalSims();
std::vector<std::string> existingPresavedSims();


/**
 * @class   GameOfLifeView
 * @brief   Sous-classe de GameOfLife qui représente une certaine portion (rectangulaire) de la grille_ qui est vue et avec laquelle on peut intéragir
 */
class GameOfLifeView : public GameOfLife {
private :
	/**
	 * @brief Coordonnées NE TENANT PAS COMPTE DES MARGES (ie dans [0,MAX_LIGNES[x[0,MAX_COLONNES[ ) qui représentent la zone visible de la grille_.
	 * On peut modifier le contenu de celle-ci via l'interface en utilisant des coordonnées dans [0,Lmax_-Lmin_[x[0,Cmax_-Cmin_[.
	 */
	unsigned int Lmin_, Lmax_, Cmin_, Cmax_;
	/**
	 * @brief Liste des coordonnées (dans [0,Lmax_-Lmin_[x[0,Cmax_-Cmin_[ ) des cellules qui sont vivantes_ et visibles à un instant donné.
	 */
	Liste vivantes_visibles;
	
	// Méthodes utilitaires ============================================================
	/**
	 * @brief Méthode utilitaire qui est utilisée comme raccourci dans evolve(). ELle masque celle de la super-classe GameOfLife.
	 * Elle vérifie si les cellules voisines de (i,j) n'étaient pas déjà vivantes_ et si elles doivent le devenir.
	 * Si oui elle les rajoute à v et à v_visibles.
	 *
	 * @param i,j coordonnées de la cellule de base (TENANT COMPTE DES MARGES, ie dans [0,MAX_LIGNES+100[x[0,MAX_COLONNES+100[)
	 * @param v Liste des cellules vivantes_ à laquelle rajouter les voisines qui vont devenir vivantes_.
	 * @param v_visibles    Liste des cellules vivantes_ visibles à laquelle rajouter les voisines qui vont devenir vivantes_ et visibles.
	 */
	void verif(size_t const& i, size_t const& j, Liste& v, Liste& v_visibles, Liste& n_n);
public :
	// Constructeurs ============================================================
	/**
	 * @brief Construit un nouvel GameOfLifeView avec vue dans la fenêtre [Lmin_, Lmax_[x[Cmin_, Cmax_[.
	 * @param lmin futur Lmin_
	 * @param lmax futur Lmax_
	 * @param cmin futur Cmin_
	 * @param cmax futur Cmax_
	 */
	GameOfLifeView(unsigned int const& lmin = 0,
	               unsigned int const& lmax = MAX_LIGNES,
	               unsigned int const& cmin = 0,
	               unsigned int const& cmax = MAX_COLONNES);
	
	virtual ~GameOfLifeView() = default;
	// Getters  ==================================================================
	/**
	 * @brief masque la version de la super-classe GameOfLife pour ne renvoyer que les cellules vivantes_ visibles (avec les coordonnées dans [0,Lmax_-Lmin_[x[0,Cmax_-Cmin_[.
	 * @return référence constante sur la Liste des vivantes_ visibles
	 */
	Liste const& vivantes() const override;
	/**
	 * @return nombre de cellules vivantes_ visibles
	 */
	unsigned int nbCells() const override;
	/**
	 * @return nombre de colonnes dans la partie visible (Cmax_-Cmin_)
	 */
	inline unsigned int nbColumns() const {return Cmax_-Cmin_;}
	/**
	 * @return nombre de colonnes dans la partie visible (Cmax_-Cmin_)
	 */
	inline unsigned int nbLines() const {return Lmax_-Lmin_;}
	unsigned int Lmin() const;
	unsigned int Cmin() const;
	unsigned int Lmax() const;
	unsigned int Cmax() const;
	
	// Setters =======================================================================
	/**
	 *  @brief   Vérifie si la cellule indiquée n'est pas déjà dans la grille_ et sinon l'y insère. Ne marche que pour des cellules dans la partie visible.
	 *  @param   i,j  coordonnées par rapport à la partie visible (ie dans [0,Lmax_-Lmin_[x[0,Cmax_-Cmin_[ )
	 *  @returns référence sur l'instance courante
	 */
	bool addCell(size_t const& i, size_t const& j) override;
	/**
	 *  @brief   Vérifie si la cellule indiquée est bien vivante dans la grille_ et si oui la supprime. Ne marche que pour des cellules dans la partie visible.
	 *  @param   i,j  coordonnées par rapport à la partie visible (ie dans [0,Lmax_-Lmin_[x[0,Cmax_-Cmin_[ )
	 *  @returns référence sur l'instance courante
	 */
	bool deleteCell(size_t const& i, size_t const& j) override;
	/**
	 *  @brief  Inverse l'état de la cellule. Ne marche que pour des cellules dans la partie visible.
	 *  @param   i,j  coordonnées par rapport à la partie visible (ie dans [0,Lmax_-Lmin_[x[0,Cmax_-Cmin_[ )
	 *  @returns référence sur l'instance courante
	 */
	bool inverseCell(size_t const& i, size_t const& j) override;
	/**
	 *  @brief  Ajoute un motif dans la partie visible de la grille_ à l'aide de addCell(). Si ce motif dépasse, ne dessine que ce qui ne dépasse pas.
	 *  @param  motif   Motif à rajouter (avec coordonnées par rapport à la partie visible (ie dans [0,Lmax_-Lmin_[x[0,Cmax_-Cmin_[ ))
	 *  @returns    référence sur l'instance courante
	 */
	std::vector<bool> addMotif(Motif const& motif) override;
	/**
	 *  @brief  Supprime les cellules de la partie visible de la grille_ contenues dans un motif a l'aide de deleteCell().
	 *  @param  motif   Motif à enlever (avec coordonnéespar rapport à la partie visible (ie dans [0,Lmax_-Lmin_[x[0,Cmax_-Cmin_[ ))
	 *  @returns    référence sur l'instance courante
	 */
	std::vector<bool> deleteMotif(Motif const& motif) override;
	/**
	 *  @brief  Efface toute la partie visible de la grille_.
	 *  @returns    référence sur l'instance courante
	 */
	golChange wipe() override;
	/**
	 *  @brief  Fait evoluer toute la grille_ en la faisant passer a la génération suivante et en updatant ses attributs.
	 *  Masque la version de la super-classe GameOfLife pour aussi mettre à jour la Liste des cellules vivantes_ et visibles.
	 */
	golChange evolve() override;
	
	// Enregistrement de motifs et simulaions  ==============================
	/**
	 *  @brief	Enregistre un fichier .csv contenant les coordonnées du motif formé de toutes les cellules vivantes_ dans la partie visible de la grille_.
	 *  Si un fichier du même nom existe deja, ne fait rien.
	 *  Les coordonnées des cellules sont par rapport à la partie visible (ie dans [0,Lmax_-Lmin_[x[0,Cmax_-Cmin_[ )
	 *  @param	nomMotif	Nom du fichier à créer.
	 *  @param  categorie La catégorie dans lequel le fichier sera enregistré.
	 *  @return true si on a réussi à enregistrer le motif, false sinon
	 */
	bool saveMotif(std::string const& nomMotif, FILE_CATEGORY const& categorie = local) const override;
	
	// Affichage ====================================================================================
	/**
	 *  @brief  Affiche en sortie standard (texte, cli) la partie visible de la grille_
	 *  @param  out Flot de sortie sur lequel afficher
	 *  @param  avec_grille si true affiche également la grille_ entière à droite de la partie visible.
	 */
	void print(std::ostream& out = std::cout, bool avec_grille = false) const;
	/**
	 *  @brief  Redimensionne la partie visible de la grille_ et met à jour la Liste des cellules vivantes_ visibles
	 *  @param  lmin    nouveau Lmin_ (dans [0,MAX_LIGNES[)
	 *  @param  Lmax    nouveau Lmax_ (dans [0,MAX_LIGNES[)
	 *  @param  Cmin    nouveau Cmin_ (dans [0,MAX_COLONNES[)
	 *  @param  Cmax    nouveau Cmax_ (dans [0,MAX_COLONNES[)
	 *  @returns    reference sur l'instance courante
	 */
	GameOfLifeView& resize(unsigned int const& lmin, unsigned int const& lmax, unsigned int const& cmin, unsigned int const& cmax);
	/**
	 * @brief   Translate la partie visible d'un certain vecteur et met à jour la Liste des cellules vivantes_ visibles
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
	 * @param x,y    coordonnées par rapport à la grille_ (ie dans [0,Lmax_-Lmin_[x[0,Cmax_-Cmin_[) du centre du zoom
	 * @param grossissement  grossissement du zoom. Zoom in si entre 0 et 1, zoom out (dézoom) si supérieur à 1.
	 * @return  référence sur l'instance courante
	 */
	GameOfLifeView& zoom(size_t const& x, size_t const& y, double grossissement);
};

/**
 *  @class  Simulation
 *  @brief  API qui permet de charger une simulation déjà existante enregistrée localement et de l'afficher.
 */
class Simulation {
private :
	/**
	 *  @brief  GOLView qui contiendra les cellules à afficher
	 */
	GameOfLifeView* grille;
	std::filesystem::path config_path;
	rapidcsv::Document* config_file;

public :
	/**
	 *  @brief  types d'erreurs à lancer pour être attrapées lors de la création de simulations
	 */
	enum Error{NON_EXISTING_SIM, INCOMPLETE_SIM, INCOMPATIBLE_DIMENSIONS};
	
	// Constructeurs & Destructeurs ========================================================================================================
	/**
	 *  @brief  construit une simulation vide (pointeur grille_ NULL, pas de nom et path sur DATA_PATH)
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
	 *  @brief  S'il y a un problème d'accès au fichiers, lance une erreur INCOMPLETE_INFO ou INCOMPLETE_CONTENT
	 *  @param  num_gen numero de la génération
	 *  @returns    motif numero num_gen
	 */
	Motif motif(unsigned int const& num_gen) const;
	/**
	 *  @returns    la Liste des cellules vivantes_ visibles de la grille_ sous-jacente
	 */
	Liste const& vivantes() const;
	/**
	 *  @returns    true si la simulation est terminée (il n'y a plus de motifs à charger), false sinon
	 */
	bool finie() const;
	
	unsigned int duree() const;
	
	// Evolution de la grille_ ========================================================================================================
	/**
	 *  @brief  fait évoluer la grille_ en chargeant le motif suivant
		*  @returns    true si l'évolution a pu se faire, false si la simulation est arrivée à la fin
	 */
	bool evolve();
	/**
	 * @brief Fait appel à la méthode translate de grille_.
	 * @param l
	 * @param c
	 * @return Simulation&
	 */
	Simulation& translate(int const& l, int const& c);
	Simulation& resize(unsigned int const& lmin, unsigned int const& lmax, unsigned int const& cmin, unsigned int const& cmax);
	
	// Affichage ========================================================================================================
	/**
	 *  @brief  fait appel à la méthode print() de grille_ pour l'afficher
	 *  @param  out flot de sortie sur lequel afficher la grille_
	 */
	void print(std::ostream& out = std::cout, bool avec_grille = false) const;
};

#endif // GAME_OF_LIFE_H
