#include "mainwindow.h"
#include <QMouseEvent>
#include <QLineEdit>
#include <QPushButton>
#include <array>
#include <initializer_list>
#include <QComboBox>
#include "game_of_life.h"
#include "motifs.h"
#include <iostream>
#include <QGraphicsOpacityEffect>
#include <QAbstractItemView>

Combobox::Combobox(QWidget* parent) : QComboBox(parent)
{
    timer = new QTimer();
    timer->start(10);
    connect(timer, SIGNAL(timeout()), this, SLOT(time_event()));
    this->view()->installEventFilter(this);
}

void Combobox::showPopup()
{
    emit focus(true);
    QComboBox::showPopup();
}

void Combobox::hidePopup()
{
    emit focus(false);
    QComboBox::hidePopup();
}

void Combobox::time_event()
{
    timer->stop();
    timer->start(10);
    emit time_e();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), x_current(-1), y_current(-1), ptr(nullptr), lance(nullptr),
      ctrl_on(false), x_first(-1), y_first(-1), x_end(-1), y_end(-1), nb_col(0),
      nb_lines(0), simul_on(false), cells2(nullptr), pos_souris(nullptr), x_prec(-1),
      y_prec(-1), pause(nullptr), calques(nullptr), frame_on(false)
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
   std::cout << "End_build" << std::endl;
   state = 0;
}

void MainWindow::creer()
{
    this->resize(520, 600);
    delete x_; delete y_; delete cree;
    lance = new QPushButton("lancer", this);
    connect(lance, SIGNAL (clicked()), this, SLOT (lancer_s()));
    lance->show();
    pos_souris = new QLabel(this);
    pos_souris->move(480, 580);
    pos_souris->show();
    ptr = new GameOfLife(nb_lines, nb_col);
    cells2 = &(ptr->get_viv());
}

void MainWindow::creer_s() { nb_lines = x_->text().toUInt(); nb_col = y_->text().toUInt(); this->creer(); state = 1;}

void MainWindow::focus_frame(bool b) {frame_on = b;}

void MainWindow::combo_time(int i)
{
    Q_UNUSED(i);
    //std::cout << calques->itemText(calques->view()->currentIndex().row()).toStdString() << std::endl;
    if (calques->itemText(calques->view()->currentIndex().row()) == "test1")
    {
        map->setStyleSheet("background-color: green");
    }
    else if (calques->itemText(calques->view()->currentIndex().row()) == "test2")
    {
        map->setStyleSheet("background-color: red");
    }
    //load(map, calques->itemText(calques->view()->currentIndex().row()));
    map->move(QCursor::pos());
    if (frame_on) {map->show(); map->raise(); }
    else {map->hide();}
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    if (state == 1)
    {
        Q_UNUSED(event);
        paint->begin(this);
        QRect rect(10, 90, 500, 500);
        paint->fillRect(rect, Qt::black);
        bool in(false);
        for (auto a : *cells2)
        {
            if (a.first < nb_lines && a.second < nb_col)
            {
                Cell_ b(a.first*500/nb_lines + 10, a.second*500/nb_col + 90, 500/nb_lines, 500/nb_col);
                b.change_color();
                if (b.state()) {paint->fillRect(b.rect(), b.color());}
                if (a.first == x_current && a.second == y_current) {in = true;}
            }
        }
        if (in)
        {
            paint->setPen(Qt::black);
            QRect rect(x_current*500/nb_lines + 10, y_current*500/nb_col + 90, 500/nb_lines, 500/nb_col);
            rect.setWidth(rect.width() - 1);
            rect.setHeight(rect.height() - 1);
            paint->drawRect(rect);
        }
        else if (x_current >= 0 && y_current >= 0)
        {
            paint->setPen(Qt::white);
            QRect rect(x_current*500/nb_lines + 10, y_current*500/nb_col + 90, 500/nb_lines, 500/nb_col);
            rect.setWidth(rect.width() - 1);
            rect.setHeight(rect.height() - 1);
            paint->drawRect(rect);
        }

        if (ctrl_on)
        {
            int min_x, max_x, min_y, max_y;
            if (x_current >= x_first) {min_x = x_first; max_x = x_current;}
            else {max_x = x_first; min_x = x_current;}
            if (y_current >= y_first) {min_y = y_first; max_y = y_current;}
            else {max_y = y_first; min_y = y_current;}
            if (min_x >= 0 && max_x < nb_lines && min_y >= 0 && max_y < nb_col)
            {
                for (size_t a(min_x); a<=max_x; a++)
                {
                    for (size_t b(min_y); b<=max_y; b++)
                    {
                        QRect rect(a*500/nb_lines + 10, b*500/nb_col + 90, 500/nb_lines, 500/nb_col);
                        paint->fillRect(rect, QColor(0,0,100,180));
                    }
                }
            }
        }
        else if (calque.on_off && x_current >= 0 && y_current >= 0)
        {
            for (auto const& a : calque.alive)
            {
                if (a.first + x_current - calque.translate.first >=0        &&
                    a.second + y_current - calque.translate.second >= 0     &&
                    a.first + x_current - calque.translate.first < nb_lines &&
                    a.second + y_current - calque.translate.second < nb_col)
                {
                    QRect rect((a.first + x_current - calque.translate.first)*500/nb_lines + 10, (a.second + y_current - calque.translate.second)*500/nb_col + 90, 500/nb_lines, 500/nb_col);
                    paint->fillRect(rect, QColor(128,128,128,180));
                    pos_souris->setText(QString::number(calque.translate.first) + " " + QString::number(calque.translate.second));
                }
            }
        }
        paint->end();
    }
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
        if (min_x >= 0 && max_x < nb_lines && min_y >= 0 && max_y < nb_col)
        {
            Calque temp;
            for (auto a : *cells2)
            {
                if (a.first >= min_x && a.first <= max_x && a.second >= min_y && a.second <= max_y)
                {
                    temp.alive.push_back(a);
                }
            }
            translate(temp);
            calque.alive = temp.alive;//voué à changer
            calque.translate = temp.translate;
        }
    }
}

void MainWindow::lancer_s()
{
    timer = startTimer(50);
    lance->hide();
    pause = new QPushButton("pause", this);
    connect(pause, SIGNAL (clicked()), this, SLOT (pause_s()));
    pause->move(425, 0);
    pause->show();
    calques = new Combobox(this);
    calques->setParent(this);
    calques->addItem("test1");
    calques->addItem("test2");
    calques->move(10, 0);
    calques->resize(100, 20);
    calques->show();
    connect(calques, SIGNAL(time_e(int)), this, SLOT(combo_time(int)));
    connect(calques, SIGNAL(focus(bool)), this, SLOT(focus_frame(bool)));
    map = new QFrame();
    map->resize(50, 50);
    map->move(400, 400);
    map->setWindowFlag(Qt::ToolTip, true);
    map->setStyleSheet("background-color: red");
    QGraphicsOpacityEffect op(map);
    op.setOpacity(1.0);
    map->setGraphicsEffect(&op);
    map->setAutoFillBackground(true);
    map->hide();
    calque_mod = new QPushButton("calque_switch", this);
    connect(calque_mod, SIGNAL (clicked()), this, SLOT (calque_switch_s()));
    calque_mod->move(225, 0);
    calque_mod->show();
    calque.alive = grenouille;
    translate(calque);
    simul_on = true;
}

void MainWindow::pause_s()
{
    if (timer == 0)
    {
        timer = startTimer(50);
        simul_on = true;
        pause->setText("pause");
    } else {
        killTimer(timer);
        timer = 0;
        pause->setText("play");
        simul_on = false;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (!simul_on)
    {
        if (((event->x() >= 10) && (event->x() <= 510))&&
            ((event->y() >= 90) && (event->y() <= 590)))
        {
            if (calque.on_off)
            {
                for (auto const& a : calque.alive)
                {
                    if ((event->x()-10)*nb_lines/500 + a.first - calque.translate.first >= 0             &&
                        (event->y()-90)*nb_lines/500 + a.second - calque.translate.second >= 0           &&
                        (event->x()-10)*nb_lines/500 + a.first  - calque.translate.first < nb_lines &&
                        (event->y()-90)*nb_lines/500 + a.second - calque.translate.second < nb_col)
                    {
                        ptr->add_cell({(event->x()-10)*nb_lines/500 + a.first  - calque.translate.first,(event->y()-90)*nb_col/500 + a.second - calque.translate.second});
                    }
                }
            }
            else { ptr->inv_cell({(event->x()-10)*nb_lines/500, (event->y()-90)*nb_col/500});}
            x_prec = x_current;
            y_prec = y_current;
            x_current = (event->x()-10)*nb_lines/500;
            y_current = (event->y()-90)*nb_col/500;
            this->update();
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (nb_lines >= 1)
    {
        if (!(event->modifiers() == Qt::ControlModifier)) {ctrl_on = false;}
        if (((event->x() >= 10) && (event->x() <= 510))&&
            ((event->y() >= 90) && (event->y() <= 590)))
        {
            x_prec = x_current;
            y_prec = y_current;
            x_current = (event->x()-10)*nb_lines/500;
            y_current = (event->y()-90)*nb_col/500;
            if (event->buttons() == Qt::LeftButton && (x_prec != x_current || y_prec != y_current) && !simul_on)
            {
                ptr->inv_cell({x_current, y_current});
            }
        }
        else {x_current = -1; y_current = -1;}
        this->update();
    }
    pos_souris->setText(QString::number(event->x()) + " " + QString::number(y_current));
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (timer != 0)
    {
        Q_UNUSED(event);
        ptr->evolve();
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
    delete calques;
}

void load(QFrame* m, QString const& s)
{

}

