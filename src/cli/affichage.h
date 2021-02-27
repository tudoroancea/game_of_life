#ifndef AFFICHAGE_H
#define AFFICHAGE_H

class CliApp {
    private :
    public :
        CliApp() = default;
        /**
         *  @brief  execute l'application cli pour visualiser une simulation pre-enregistree ou en creer une d'une duree fixe
         *  @returns 0 si l'éxecution s'est faite sans erreurs, -1 sinon
         */
        int exec();
};

#endif // AFFICHAGE_H