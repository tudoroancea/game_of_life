#ifndef MOTIF_H
#define MOTIF_H

#include <array>
#include <vector>
#include <string>
#include <filesystem>


typedef std::pair<size_t,size_t> Coord;
typedef std::vector<Coord> Liste;
size_t dist(size_t const& a, size_t const& b);
std::ostream& operator<<(std::ostream& out, Coord const& c);

enum FILE_CATEGORY{local, presaved};
std::ostream& operator<<(std::ostream& out, FILE_CATEGORY const& cat);

#define X(c) c.first
#define Y(c) c.second


class Motif {
private :
	/**
	 *  @brief  Liste des cellules vivantes_ composant le motif (dans une grille_ potentiellement inifnie)
	 */
	Liste cellules;

public :
	// Constructeurs & Destructeurs ===================================================================================================
	/**
	 *  @brief  cree un motif vide
	 */
	Motif() = default;
	Motif(Motif const&) = default;
	/**
	 *  @brief  cree un motif compose des coordonnees donnees en argument
	 */
	Motif(std::initializer_list<Coord> liste);
	/**
	 *  @brief  cree un motif compose des coordonnees donnees en argument
	 */
	explicit Motif(Liste const& liste);
	/**
	 *  @brief  Construit à partir un Motif enregistré sous la forme d'un fichier .csv. Si ce fichier n'exitse pas, construit un motif vide
	 *  @param  fichier nom du fichier sans extension
	 *  @param  categorie   Dossier dans lequel chercher. Si different de local, on cherche dans presaved
	 */
	explicit Motif(std::string const& fichier, FILE_CATEGORY const& categorie = local);
	/**
	 *  @brief  Construit à partir un Motif enregistré sous la forme d'un fichier .csv. Si ce fichier n'exitse pas, construit un motif vide
	 *  @param  chemin  path du fichier sans extension
	 */
	explicit Motif(std::filesystem::path const& chemin);
	
	// Méthodes de modification ===================================================================================================
	/**
	 *  @brief  Tourne un motif d'un multiple de 90° autour d'un point specifie
	 *  @param  angle   nombre de 1/4 de tours a faire (positif = sens trigo, negatif = sens horaire)
	 *  @param  centre  coordonnees du centre de la rotation
	 *  @returns    reference sur l'instance courante
	 */
	Motif& rotate(int const& angle, Coord const& centre);
	/**
	 *  @brief  Tourne un motif d'un multiple de 90° autour du centre de gravite du plus petit rectangle contenant le motif
	 *  @param  angle   nombre de 1/4 de tours a faire (positif = sens trigo, negatif = sens horaire)
	 *  @returns    reference sur l'instance courante
	 */
	Motif& rotate(int const& angle);
	/**
	 *  @brief  rajoute une cellule au motif
	 *  @param  p   cellule a rajouter
	 */
	Motif& push_back(Coord const& p);
	/**
	 *  @brief  Concatene la listes d'un autre motif
	 *  @param  autre  motif a ajouter
	 *  @returns    retourne une reference sur l'instance courante
	 */
	Motif& append(Motif const& autre);
	/**
	 *  @brief  Concatene une Liste de cellules
	 *  @param  autre  Liste de cellules a rajouter
	 *  @returns    retourne une reference sur l'instance courante
	 */
	Motif& append(Liste const& autre);
	/**
	 *  @brief  Concatene une Liste de cellules
	 *  @param  autre  Liste de cellules a rajouter
	 *  @returns    retourne une reference sur l'instance courante
	 */
	Motif& append(std::initializer_list<Coord> const& autre);
	/**
	 *  @brief  Translate le motif. Si le resultat doit avoir des coordonnees positives, reduit la translation
	 *  @param  x,y coordonnees du vecteur par lequel on translate
	 *  @returns    retourne une reference sur l'instance courante
	 */
	Motif& translate(int const& x, int const& y);
	/**
	 *  @brief  Translate le motif. Si le resultat doit avoir des coordonnees positives, reduit la translation
	 *  @param  p   vecteur de translation
	 *  @returns    retourne une reference sur l'instance courante
	 */
	Motif& translate(std::pair<int,int> const& p);
	/**
	 *  @brief  Translate le motif pour le rapprocher le plus possible de l'origine
	 *  @returns    retourne une reference sur l'instance courante
	 */
	Motif& recalibrate();
	/**
	 * @brief Efface toutes les cellules du motif pour le rendre vide
	 * @return retourne une référence sur l'instance courante
	 */
	Motif& clear();
	
	// Getters ===================================================================================================
	/**
	 *  @returns    Iterateur pointant sur le premier element cellule du motif
	 */
	Liste::iterator begin();
	/**
	 *  @returns    Iterateur pointant sur le dernier element cellule du motif
	 */
	Liste::iterator end();
	/**
	 *  @returns    Iterateur constant pointant sur le premier element cellule du motif
	 */
	[[nodiscard]] Liste::const_iterator cbegin() const;
	/**
	 *  @returns    Iterateur constant pointant sur le dernier element cellule du motif
	 */
	[[nodiscard]] Liste::const_iterator cend() const;
	
	Coord const& operator[](size_t const& r);
	Liste const& cells() const;
	
	// Infos ===================================================================================================
	/**
	 *  @returns    la plus petite ligne d'une cellule du motif
	 */
	[[nodiscard]] size_t min_ligne() const;
	/**
	 *  @returns    la plus petite colonne d'une cellule du motif
	 */
	[[nodiscard]] size_t min_colonne() const;
	/**
	 *  @returns    la plus grande ligne d'une cellule du motif
	 */
	[[nodiscard]] size_t max_ligne() const;
	/**
	 *  @returns    la plus grande colonne d'une cellule du motif
	 */
	[[nodiscard]] size_t max_colonne() const;
	/**
	 * @return la taille de la Liste de cellules
	 */
	size_t size() const;
	/**
	 * @param c coordonnees à traiter
	 * @return true si le motif contient c
	 * @return false sinon
	 */
	[[maybe_unused]] [[nodiscard]] bool contient(Coord const& c) const;
	/**
	 * @param c
	 * @return true
	 * @return false
	 */
	[[nodiscard]] bool a_pour_voisin(Coord const& c) const;
};
bool sont_voisins(Coord const& a, Coord const& b);

// Gestion des motifs enregsitres ===================================================================================================
/**
 *  @returns    Liste des fichiers (sans extension) de motifs enregistres localement
 */
std::vector<std::string> existing_local_motifs();
/**
 *  @returns    Liste des fichiers (sans extension) de motifs pre-enregistres
 */
std::vector<std::string> existing_presaved_motifs();

// Calques Qt ===================================================================================================
struct Calque {
	Motif alive;
	std::pair<size_t, size_t> translate = {0,0};
	bool on_off = false;
};
void translate(Calque& calque);

// Construction de motifs géométriques ===================================================================================================
/**
 *  @brief  Renvoie les coordonnees d'un segment reliant 2 points en utilisant l'algorithme de Bresenham
 *  @param x1,y1    coordonnees (dans [0,L[x[0,C[) du premier point
 *  @param x2,y2    coordonnees (dans [0,L[x[0,C[) du second point
 *  @returns    Liste de coordonnees
 */
Liste segment(size_t x1, size_t y1, size_t const& x2, size_t const& y2);
/**
 *  @brief  Renvoie les coordonnees d'un segment reliant 2 points en utilisant l'algorithme de Bresenham
 *  @param a    coordonnees (dans [0,L[x[0,C[) du premier point
 *  @param b    coordonnees (dans [0,L[x[0,C[) du second point
 *  @returns    Liste de coordonnees
 */
Liste segment(Coord a, Coord const& b);

#endif // MOTIF_H
