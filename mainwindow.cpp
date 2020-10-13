#include "mainwindow.h"
#include <QMouseEvent>
#include <QLineEdit>
#include <QPushButton>
#include <array>
#include <initializer_list>
#include "game_of_life.h"
#include "motifs.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), x_current(-1), y_current(-1), ptr(nullptr), lance(nullptr), ctrl_on(false), x_first(-1), y_first(-1), x_end(-1), y_end(-1)
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
    pos_souris = new QLabel(this);
    pos_souris->move(480, 580);
    pos_souris->show();
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

    if (ctrl_on)
    {
        int min_x, max_x, min_y, max_y;
        if (x_current >= x_first) {min_x = x_first; max_x = x_current;}
        else {max_x = x_first; min_x = x_current;}
        if (y_current >= y_first) {min_y = y_first; max_y = y_current;}
        else {max_y = y_first; min_y = y_current;}
        if (min_x >= 0 && max_x < cells2.size() && min_y >= 0 && max_y < cells2[0].size())
        {
            for (size_t a(min_x); a<=max_x; a++)
            {
                for (size_t b(min_y); b<=max_y; b++)
                {
                    paint->fillRect(cells2[a][b].rect(), QColor(0,0,100,180));
                }
            }
        }
    }
    else if (calque.on_off && x_current >= 0 && y_current >= 0)
    {
        for (auto const& a : calque.alive)
        {
            if (a.first + x_current >=0     &&
                a.second + y_current >= 0   &&
                a.first + x_current < cells2.size()   &&
                a.second + y_current < cells2[0].size())
            {
                QRect rect(cells2[a.first + x_current][a.second + y_current].rect());
                paint->fillRect(rect, QColor(128,128,128,180));
                pos_souris->setText(QString::number(calque.alive[0].first + x_current) + " " + QString::number(calque.alive[0].second + y_current));
            }
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

void MainWindow::charger_calque()
{
    x_end = x_current; y_end = y_current;
    if (x_first != x_end || y_first != y_end)
    {
        int min_x, max_x, min_y, max_y;
        if (x_end >= x_first) {min_x = x_first; max_x = x_end;}
        else {max_x = x_first; min_x = x_end;}
        if (y_end >= y_first) {min_y = y_first; max_y = y_end;}
        else {max_y = y_first; min_y = y_end;}
        if (min_x >= 0 && max_x < cells2.size() && min_y >= 0 && max_y < cells2[0].size())
        {
            motifs::calque temp;
            for (size_t a(min_x); a<=max_x; a++)
            {
                for (size_t b(min_y); b<=max_y; b++)
                {
                    if (cells2[a][b].state())
                    {
                        temp.alive.push_back({a,b});
                    }
                }
            }
            motifs::translate(temp);
            calque.alive = temp.alive;//voué à changer
        }
    }
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
    calque_mod = new QPushButton("calque_switch", this);
    connect(calque_mod, SIGNAL (clicked()), this, SLOT (calque_switch_s()));
    calque_mod->move(225, 0);
    calque_mod->show();
    calque.alive = {{-1,-1}, {0, 0}, {1,0}};
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
        if (calque.on_off)
        {
            for (auto const& a : calque.alive)
            {
                if ((event->x()-10)*cells2.size()/500 + a.first >= 0            &&
                    (event->y()-90)*cells2.size()/500 + a.second >= 0           &&
                    (event->x()-10)*cells2.size()/500 + a.first < cells2.size() &&
                    (event->y()-90)*cells2.size()/500 + a.second < cells2[0].size())
                {
                    Cell_& current(cells2[(event->x()-10)*cells2.size()/500 + a.first][(event->y()-90)*cells2[0].size()/500 + a.second]);
                    if (!current.state())
                    {
                        current.change_color();
                    }
                }
            }
        }
        else { cells2[(event->x()-10)*cells2.size()/500][(event->y()-90)*cells2[0].size()/500].change_color();}
        x_current = (event->x()-10)*cells2.size()/500;
        y_current = (event->y()-90)*cells2[0].size()/500;
        this->update();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{  
    if (cells2.size() >= 1)
    {
        if (!(event->modifiers() == Qt::ControlModifier)) {ctrl_on = false;}
        if (((event->x() >= 10) && (event->x() <= 510))&&
            ((event->y() >= 90) && (event->y() <= 590)))
        {
            x_current = (event->x()-10)*cells2.size()/500;
            y_current = (event->y()-90)*cells2[0].size()/500;
        }
        else {x_current = -1; y_current = -1;}
        this->update();
    }
    pos_souris->setText(QString::number(x_current) + " " + QString::number(y_current));

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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier)
    {
        ctrl_on = true;
        if (ctrl_on)
        {
            x_first = x_current;
            y_first = y_current;
        }
    }
    if (event->key() == Qt::Key_C) {this->charger_calque();}
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier)
    {

    }
}

MainWindow::~MainWindow()
{
    delete paint;
    delete ptr;
    delete lance;
    delete pause;
    delete calque_mod;
    delete pos_souris;
}

