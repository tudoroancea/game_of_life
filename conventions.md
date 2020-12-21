## Pour la classe Simulation
On a à chaque fois un dossier nommé « sim-NOM_SIMULATION » contenant 2 fichiers:
1. NOM_SIMULATION-info.csv (array au format Rapidcsv à 2 colonnes et durée simulation +5 lignes):\
Ligne 0 : « ligne,colonne » \
Ligne 1 : nombre de lignes, nombre de colonnes\
Ligne 2 : nombre de générations/durée simulation\
Lignes 3 à durée simulation+3 : ligne du fichier content à laquelle commence le motif de la génération i (0≤i≤durée simulation)\
Ligne durée simulation + 4 : dernière ligne du fichier content
2. NOM_SIMULATION-content.csv (array au format Rapidcsv à 2 colonnes et beaucoup de lignes) :\
Sur chaque ligne des coordonnées d’une cellule