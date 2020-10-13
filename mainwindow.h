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
#include "game_of_life.h"
#include "motifs.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void creer(unsigned int x, unsigned int y);

    void paintEvent(QPaintEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void timerEvent(QTimerEvent* event) override;

    void charger_grille();

public slots:
    void creer_s();
    void lancer_s();
    void pause_s();
    void calque_on_s() {calque.on_off = true;}


private:
    std::vector<std::vector<Cell_>> cells2;
    QLineEdit* x_;
    QLineEdit* y_;
    QPushButton* cree;
    QPushButton* lance;
    QPushButton* pause;
    QPushButton* calque_mod;
    QPainter* paint;
    int x_current;
    int y_current;
    int timer;
    GameOfLife* ptr;
    motifs::calque calque;
};
#endif // MAINWINDOW_H
/* Ajouter le nombre de générations
 * Ajouter la sauvegarde dans un fichier
 * Ajouter l'ajout de forme par calque
 * copié-collé ?
 */
