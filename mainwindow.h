#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cell.h"
#include "cell_.h"
#include <QLabel>
#include <vector>
#include <QLineEdit>
#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>
#include <QTime>
#include "game_of_life.h"

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

public slots:
    void creer_s();
    void lancer_s();


private:
    std::vector<std::vector<Cell_>> cells2;
    QLineEdit* x_;
    QLineEdit* y_;
    QPushButton* cree;
    QPainter* paint;
    unsigned int x_current;
    unsigned int y_current;
    int timer;
    GameOfLife<400, 400>* ptr;
};
#endif // MAINWINDOW_H
/* Etape 1 : bon en gros l'idée c'est de faire la classe qui va permettre l'affichage des
 * cases.
 * */
