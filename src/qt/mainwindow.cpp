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
#include <fstream>
#include <string>

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

Combobox::~Combobox()
{
    delete timer;
}

Frame::Frame(QWidget* parent) : QFrame(parent) {}

void Frame::load(std::string s, bool local)
{
    if (s != "")
    {
        if (local) {a_dessiner = Motif(s);}
        else {a_dessiner = Motif(s, "presaved");}
        a_dessiner.recalibrate();
        this->resize((a_dessiner.max_ligne()+1)*10, (a_dessiner.max_colonne()+1)*10);
    }
}

void Frame::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if (this->isVisible())
    {
        QPainter paint(this);
        for (auto a : a_dessiner)
        {
            QRect rect((a.first)*10, (a.second)*10, 10, 10);
            paint.fillRect(rect, QColor(128,128,128,180));
        }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), x_current(-1), y_current(-1), ptr(nullptr), lance(nullptr),
      ctrl_on(false), x_first(-1), y_first(-1), x_end(-1), y_end(-1), nb_col(0),
      nb_lines(0), simul_on(false), cells2(nullptr), pos_souris(nullptr), x_prec(-1),
      y_prec(-1), pause(nullptr), calques(nullptr), frame_on(false), save_game(nullptr), 
      detail_selectionne(nullptr), calque_mod(nullptr), new_taille(nullptr), new_entree(nullptr),
      new_state(false)
{
    setMouseTracking(true);
    this->resize(520, 90);
    this->move(10, 10);
    new_sim = new QPushButton("Nouvelle simulation", this);
    new_sim->resize(150, 35);
    new_sim->move(355, 15);
    connect(new_sim, SIGNAL (clicked()), this, SLOT (creer_s()));
    paint = new QPainter(this);
    this->update();
    state = 0;
    nb_motifs_locaux = 0;
    QLabel* sim_choix = new QLabel("Simulations enregistrées :", this);
    labels["sim_choix"] = sim_choix;
    sim_choix->resize(200, 20);
    sim_choix->move(20, 5);
    sim_loc = new QComboBox(this);
    std::vector<std::string> simul(existing_local_sims());
    for (std::string a : simul) {sim_loc->addItem(QString::fromStdString(a));}
    sim_loc->addItem("--local--");
    sim_loc->move(10, 30);
    sim_loc->resize(150, 25);
    sim_presaved = new QComboBox(this);
    simul = existing_local_sims();
    for (std::string a : simul) {sim_presaved->addItem(QString::fromStdString(a));}
    sim_presaved->addItem("--presaved--");
    sim_presaved->move(170, 30);
    sim_presaved->resize(150, 25);
    sim_lance = new QPushButton("Lancer", this);
    sim_lance->resize(70, 25);
    sim_lance->move(250, 60);
    connect(sim_lance, SIGNAL (clicked()), this, SLOT (lancer_saved_s()));    
}

void MainWindow::creer()
{
    this->resize(520, 600);
    sim_loc->hide(); sim_presaved->hide(); sim_lance->hide();
    new_sim->hide(); new_taille->hide(); new_entree->hide();
    labels["sim_choix"]->hide();
    lance = new QPushButton("lancer", this);
    connect(lance, SIGNAL (clicked()), this, SLOT (lancer_s()));
    lance->move(400, 10);
    lance->show();
    pos_souris = new QLabel(this);
    pos_souris->move(480, 580);
    pos_souris->show();
    ptr = new GameOfLife(nb_lines, nb_col);
    cells2 = &(ptr->get_viv());
    QLabel* calques_saved = new QLabel("Calques disponibles :", this);
    labels["calques_saved"] = calques_saved;
    calques_saved->resize(140, 20);
    calques_saved->move(10, 5);
    calques_saved->show();
    QLabel* mode = new QLabel("mode :", this);
    labels["calques_mode"] = mode;
    mode->move(10, 55);
    mode->show();
    calques = new Combobox(this);
    calques->setParent(this);
    calques->addItem("--select--");
    std::vector<std::string> motifs_locaux(existing_local_motifs());
    for (std::string a : motifs_locaux) {calques->addItem(QString::fromStdString(a));}
    nb_motifs_locaux = calques->count();
    calques->insertSeparator(nb_motifs_locaux);
    std::vector<std::string> motifs_presaved(existing_presaved_motifs());
    for (std::string a : motifs_presaved) {calques->addItem(QString::fromStdString(a));}
    calques->move(10, 30);
    calques->resize(140, 25);
    calques->show();
    connect(calques, SIGNAL(time_e(int)), this, SLOT(combo_time(int)));
    connect(calques, SIGNAL(focus(bool)), this, SLOT(focus_frame(bool)));
    connect(calques, SIGNAL(currentTextChanged(const QString&)), this, SLOT(item_changed_s(const QString&)));
    detail_selectionne = new Frame();
    detail_selectionne->resize(50, 50);
    detail_selectionne->move(400, 400);
    detail_selectionne->setWindowFlag(Qt::ToolTip, true);
    detail_selectionne->setStyleSheet("background-color: white");
    this->setUpdatesEnabled(true);
    detail_selectionne->hide();
    calque_mod = new QPushButton("calque actif", this);
    connect(calque_mod, SIGNAL (clicked()), this, SLOT (calque_switch_s()));
    calque_mod->move(60, 60);
    calque_mod->resize(90, 25);
    calque_mod->show();
    calque.alive = Motif({{0,0}, {1,0}, {2, 0}});
    calque.alive.translate(calque.alive.max_ligne(), calque.alive.max_colonne());
    calque.alive.translate(calque.alive.max_colonne(), calque.alive.max_ligne());
    translate(calque);    
}

void MainWindow::creer_s() 
{ 
    if (new_state)
    {
        nb_lines = new_entree->text().toUInt(); 
        nb_col = new_entree->text().toUInt(); 
        this->creer(); 
        state = 1;
        new_state = false;
    }
    else 
    {
        new_state = true;
        new_sim->resize(50, 25);
        new_sim->move(405, 15);
        new_sim->setText("  Créer  ");
        new_taille = new QLabel("Taille : ", this);
        new_taille->move(380, 45);
        new_taille->show();
        new_entree = new QLineEdit(this);
        new_entree->resize(50, 25);
        new_entree->move(430, 50);
        new_entree->show();
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    paint->begin(this);
    if (state == 1)
    {
        Q_UNUSED(event);
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
    }
    else 
    {
        paint->drawLine(335, 5, 335, 85);
    }
    paint->end();   
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
                    if ((event->x()-10)*nb_lines/500 + a.first - calque.translate.first >= 0        &&
                        (event->y()-90)*nb_col/500 + a.second - calque.translate.second >= 0        &&
                        (event->x()-10)*nb_lines/500 + a.first  - calque.translate.first < nb_lines &&
                        (event->y()-90)*nb_col/500 + a.second - calque.translate.second < nb_col)
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
                if (x_prec != -1 && y_prec != -1)
                {
                    liste a_ajouter(segment({x_prec, y_prec}, {x_current, y_current}));
                    for (auto const& a : a_ajouter)
                    {
                        //std::cout << "\\" << a.first << " " << a.second << std::endl;
                        ptr->add_cell(a);
                    }
                }
                ptr->inv_cell({x_current, y_current});
            }
        }
        else {x_current = -1; y_current = -1;}
        this->update();
    }
    if (state != 0) {pos_souris->setText(QString::number(event->x()) + " " + QString::number(y_current));}
}

void MainWindow::wheelEvent(QWheelEvent* event)
{
    QPoint delta(event->angleDelta());
    double mouv(delta.y()/12);
    if ((mouv > 0 && nb_lines <= 390 && nb_col <= 390)
        || (mouv < 0 && nb_lines >= 20 && nb_col >= 20))
    {
        nb_lines += mouv;
        nb_col += mouv;
        ptr->resize(nb_lines, nb_col);
        this->update();
    }
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
    if (event->key() == Qt::Key_R && calque.on_off)
    {

        calque.alive.rotate(1, *(calque.alive.cbegin()));
        translate(calque);
        //for (auto a : calque.alive){std::cout << a.first << " " << a.second << " | ";} std::cout << std::endl;
        update();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier)
    {

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
            calque.alive.translate(calque.alive.max_ligne(), calque.alive.max_colonne());
            calque.alive.translate(calque.alive.max_colonne(), calque.alive.max_ligne());
            translate(calque);
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
    save_game = new QPushButton("save", this);
    connect(save_game, SIGNAL (clicked()), this, SLOT (save_game_s()));
    save_game->move(350, 0);
    save_game->show();
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

void MainWindow::calque_switch_s()
{
    if (calque.on_off)
    {
        calque_mod->setDown(false);
    }
    else {calque_mod->setDown(true);}
    calque.on_off = 1 - calque.on_off;
}

void MainWindow::combo_time(int i)
{
    Q_UNUSED(i);
    if (frame_on &&
        QCursor::pos().x() >= calques->mapToGlobal(calques->view()->pos()).x()                            &&
        QCursor::pos().y() >= calques->mapToGlobal(calques->view()->pos()).y() + calques->height()                             &&
        QCursor::pos().x() <= calques->mapToGlobal(calques->view()->pos()).x() + calques->view()->width()  &&
        QCursor::pos().y() <= calques->mapToGlobal(calques->view()->pos()).y() + calques->view()->height() + calques->height())
    {
        if (calques->itemText(calques->view()->currentIndex().row()) != "--select--")
        {
            detail_selectionne->show();
            detail_selectionne->load(calques->itemText(calques->view()->currentIndex().row()).toStdString(), (calques->view()->currentIndex().row() <= nb_motifs_locaux));
            detail_selectionne->move(QCursor::pos());
            detail_selectionne->raise();
        }
        else {detail_selectionne->resize(1,1);}
    }
    else {detail_selectionne->hide();}
}

void MainWindow::focus_frame(bool b) 
{
    if (b) {calques->setCurrentText("--select--");}
    frame_on = b;
    if (calque.on_off) {calque_mod->setDown(true);}
}

void MainWindow::item_changed_s(QString const& entree)
{
    if (entree != "" && entree != "--select--")
    {
        std::string emplacement("presavec");
        if ((calques->view()->currentIndex().row() <= nb_motifs_locaux)) {emplacement = "local";}
        calque.alive = Motif(entree.toStdString(), emplacement);
        calque.alive.translate(calque.alive.max_ligne(), calque.alive.max_colonne());
        calque.alive.translate(calque.alive.max_colonne(), calque.alive.max_ligne());
        translate(calque);
        this->setFocus();
    }
}

void MainWindow::save_game_s()
{

}

MainWindow::~MainWindow()
{
    delete sim_loc;
    delete sim_presaved;
    delete sim_lance;
    delete new_sim;
    delete new_taille;
    delete new_entree;
    delete paint;
    delete lance;
    delete pause;
    delete calque_mod;
    delete pos_souris;
    delete calques;
    delete detail_selectionne;
    delete ptr;
    delete save_game;
    for (auto& a : labels) {delete a.second;}
}
