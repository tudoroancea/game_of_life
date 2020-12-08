#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cell_.h"
#include <QLabel>
#include <vector>
#include <QLineEdit>
#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>
#include <QTime>
#include <QComboBox>
#include <QAction>
#include <QFrame>
#include <QModelIndex>
#include <QString>
#include <QTimer>
#include "game_of_life.h"
#include "motifs.h"

class Combobox : public QComboBox
{
    Q_OBJECT
private:
    QTimer* timer;
public:
    Combobox(QWidget* parent = nullptr);
    void showPopup() override;
    void hidePopup() override;
    ~Combobox();
public slots:
    void time_event();
signals:
    void time_e(int i = 1);
    void focus(bool);
};

class Frame : public QFrame
{
    Q_OBJECT
private:
    Motif a_dessiner;
public:
    Frame(QWidget *parent = nullptr);
    void load(std::string s, bool local = true);
    void paintEvent(QPaintEvent * event) override;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void creer();

    void paintEvent(QPaintEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void wheelEvent(QWheelEvent* event) override;

    void timerEvent(QTimerEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;

    void keyReleaseEvent(QKeyEvent* event) override;

    void charger_calque();

public slots:
    void creer_s();
    void lancer_saved_s() {} // décor de cinéma
    void lancer_s();
    void pause_s();
    void calque_switch_s() {calque.on_off = 1 - calque.on_off;}
    void combo_time(int);
    void focus_frame(bool);
    void item_changed_s(const QString&);
    void save_game_s();


private:
    QComboBox* sim_loc;
    QComboBox* sim_presaved;
    QLabel* sim_choix;
    QPushButton* sim_lance;
    QPushButton* new_sim;
    QLabel* new_taille;
    QLineEdit* new_entree;
    bool new_state;
    liste const* cells2;
    QPushButton* lance;
    QPushButton* pause;
    QPushButton* calque_mod;
    QPushButton* save_game;
    QPainter* paint;
    QLabel* pos_souris;
    Combobox* calques;
    Frame* detail_selectionne;
    size_t nb_lines;
    size_t nb_col;
    int x_current;
    int y_current;
    int x_prec;
    int y_prec;
    int x_first;
    int y_first;
    int x_end;
    int y_end;
    int timer;
    GameOfLife* ptr;
    Calque calque;
    bool ctrl_on;
    bool simul_on;
    int state;
    bool frame_on;
    int nb_motifs_locaux;
};

#endif // MAINWINDOW_H
/* Ajouter le nombre de générations
 * Ajouter la sauvegarde dans un fichier
 * Pour la sauvegarde de la simulation deux possibilités
 * si on peut ouvrir le dialogue interface et récupérer
 * le nom entré alors super. Sinon il faut une première 
 * fenêtre pour demander un nom et ensuite ouvrir 
 * le dialogue interface avec ce nom proposé 
 *  Dans tous les cas il faut ouvrir le dialogue interface

 */
