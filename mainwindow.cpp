#include "mainwindow.h"
#include <QMouseEvent>
#include <QLineEdit>
#include <QPushButton>
#include <array>
#include <initializer_list>
#include "game_of_life.h"
#include "motifs.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), x_current(-1), y_current(-1), ptr(nullptr), lance(nullptr)
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
    this->resize(520, 600);
    delete x_; delete y_; delete cree;
    for (size_t a(0); a < x; a++)
    {
        std::vector<Cell_> temp;
        for (size_t b(0); b < y; b++)
        {
            temp.push_back(Cell_(a*500/x + 10, b*500/y + 90, 500/x, 500/y));
            x_current = a;
            y_current = b;
            this->update();
        }
        cells2.push_back(temp);
    }
    lance = new QPushButton("lancer", this);
    connect(lance, SIGNAL (clicked()), this, SLOT (lancer_s()));
    lance->show();
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
    if (calque.on_off && x_current >= 0 && y_current >= 0)
    {
        for (auto const& a : calque.alive)
        {
            QRect rect(cells2[a.first][a.second].rect());
            QRect transl(cells2[x_current][y_current].rect());
            rect.moveTo(rect.x() + transl.x() - 10, rect.y() + transl.y() - 90);
            paint->fillRect(rect, QColor(128,128,128,180));
        }
    }

    paint->end();

}

void MainWindow::charger_grille()
{
    motifs::Motif tab;
    for (size_t a(0); a<cells2.size(); a++)
    {
        for (size_t b(0); b<cells2[a].size(); b++)
        {
            if (cells2[a][b].state()) {tab.push_back({a+50,b+50});}
        }
    }
    motifs::Motif test(motifs::lievres);
    test += {200,200};
    ptr = new GameOfLife(tab, cells2.size(), cells2[0].size());
}

void MainWindow::lancer_s()
{
    this->charger_grille();
    timer = startTimer(50);
    lance->hide();
    pause = new QPushButton("pause", this);
    connect(pause, SIGNAL (clicked()), this, SLOT (pause_s()));
    pause->move(425, 0);
    pause->show();
    calque_mod = new QPushButton("pause", this);
    connect(calque_mod, SIGNAL (clicked()), this, SLOT (calque_on_s()));
    calque_mod->move(225, 0);
    calque_mod->show();
    calque.alive = {{0,0}, {1, 2}, {1,1}};
}

void MainWindow::pause_s()
{
    if (timer == 0)
    {
        timer = startTimer(50);
        this->charger_grille();
        pause->setText("pause");
    } else {
        killTimer(timer);
        timer = 0;
        pause->setText("play");
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (((event->x() >= 10) && (event->x() <= 510))&&
        ((event->y() >= 90) && (event->y() <= 590)))
    {
        cells2[(event->x()-10)*cells2.size()/500][(event->y()-90)*cells2[0].size()/500].change_color();
        x_current = (event->x()-10)*cells2.size()/500;
        y_current = (event->y()-90)*cells2[0].size()/500;
        this->update();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (cells2.size() >= 1)
    {
        if (((event->x() >= 10) && (event->x() <= 510))&&
            ((event->y() >= 90) && (event->y() <= 590)))
        {
            x_current = (event->x()-10)*cells2.size()/500;
            y_current = (event->y()-90)*cells2[0].size()/500;
        }
        else {x_current = -1; y_current = -1;}
        this->update();
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (timer != 0)
    {
        Q_UNUSED(event);
        std::array<std::array<bool,500>,500> tab(ptr->life());
        for (size_t a(0); a<cells2.size(); a++)
        {
            for (size_t b(0); b<cells2[a].size(); b++)
            {
                if (tab[a+50][b+50] != cells2[a][b].state()) {cells2[a][b].change_color();}
            }
        }
        this->update();
    }
}

MainWindow::~MainWindow()
{
    delete paint;
    delete ptr;
    delete lance;
    delete pause;
}

