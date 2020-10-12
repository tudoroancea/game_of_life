#include "mainwindow.h"
#include <QMouseEvent>
#include <QLineEdit>
#include <QPushButton>
#include <array>
#include <initializer_list>
#include "game_of_life.h"
#include "motifs.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), x_current(0), y_current(0), ptr(nullptr)
{
    setMouseTracking(true);
    this->resize(150, 30);
    x_ = new QLineEdit(this);
    x_->resize(50, 30);
    y_ = new QLineEdit(this);
    y_->resize(50, 30);
    y_->move(50, 0);
    cree = new QPushButton("créer", this);
    cree->resize(50, 30);
    cree->move(100, 0);
    connect(cree, SIGNAL (clicked()), this, SLOT (creer_s()));
    paint = new QPainter(this);
    this->update();
}

void MainWindow::creer(unsigned int x, unsigned int y)
{
    this->resize(420, 500);
    delete x_; delete y_; delete cree;
    for (size_t a(0); a < x; a++)
    {
        std::vector<Cell_> temp;
        for (size_t b(0); b < y; b++)
        {
            temp.push_back(Cell_(a*400/x + 10, b*400/y + 90, 400/x, 400/y));
            x_current = a;
            y_current = b;
            this->update();
        }
        cells2.push_back(temp);
    }
}

void MainWindow::creer_s() { this->creer(x_->text().toUInt(), y_->text().toUInt()); }


void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    paint->begin(this);

    for (size_t a(0); a < cells2.size(); a++)
    {
        for (size_t c(0); c < cells2[a].size(); c++)
        {
            Cell_ b = cells2[a][c];
            paint->fillRect(b.rect(), b.color());
            if (a == x_current && c == y_current)
            {
                if (b.color() == Qt::black)
                {
                    paint->setPen(Qt::white);
                } else { paint->setPen(Qt::black); }
                QRect rect(b.rect());
                rect.setWidth(rect.width() - 1);
                rect.setHeight(rect.height() - 1);
                paint->drawRect(rect);
            }
        }
    }

    paint->end();

}

void MainWindow::lancer_s()
{
    motifs::Motif tab;
    for (size_t a(0); a<cells2.size(); a++)
    {
        for (size_t b(0); b<cells2[a].size(); b++)
        {
            if (cells2[a][b].state()) {tab.push_back({a,b});}
        }
    }
    ptr = new GameOfLife<400,400>(tab);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (((event->x() >= 10) && (event->x() <= 410))&&
        ((event->y() >= 90) && (event->y() <= 490)))
    {
        cells2[(event->x()-10)*cells2.size()/400][(event->y()-90)*cells2[0].size()/400].change_color();
        x_current = (event->x()-10)*cells2.size()/400;
        y_current = (event->y()-90)*cells2[0].size()/400;
        this->update();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (cells2.size() >= 1)
    {
        if (((event->x() >= 10) && (event->x() <= 410))&&
            ((event->y() >= 90) && (event->y() <= 490)))
        {
            x_current = (event->x()-10)*cells2.size()/400;
            y_current = (event->y()-90)*cells2[0].size()/400;
            this->update();
        }
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

}

MainWindow::~MainWindow()
{
    delete paint;
}

