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
#include <QFile>
#include <QPalette>
#include <time.h>
#include <filesystem>

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
        else {a_dessiner = Motif(s, presaved);}
        a_dessiner.recalibrate();
        this->resize((a_dessiner.max_ligne()+1)*10, (a_dessiner.max_colonne()+1)*10);
        this->update();
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
    : QMainWindow(parent), new_taille(nullptr), new_entree(nullptr), new_state(false),
      calques(nullptr), reload_calques(nullptr),
      pause(nullptr),
      save_game(nullptr), pos_souris(nullptr), detail_selectionne(nullptr), nb_lines(0),
      nb_col(0), x_current(-1), y_current(-1), x_prec(-1), y_prec(-1), x_first(-1), y_first(-1),
      x_end(-1), y_end(-1), d_x(0), d_y(0), ptr({nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 0}), ctrl_on(false), simul_on(false),
      frame_on(false), info_on(false), delta_pix_prec(0)
{
    std::filesystem::current_path(std::filesystem::path(std::string(QT_PATH)));
    init_styles();    
    setMouseTracking(true);
    this->resize(520, 90);
    this->move(10, 10);
    QPushButton* new_sim = new QPushButton("Nouvelle simulation", this);
    buttons["new_sim"] = new_sim;
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
    sim_loc->addItem("--local--");
    std::vector<std::string> simul(existing_local_sims());
    for (std::string a : simul) {sim_loc->addItem(QString::fromStdString(a));}
    sim_loc->move(10, 30);
    sim_loc->resize(150, 25);
    sim_presaved = new QComboBox(this);
    sim_presaved->addItem("--presaved--");
    simul = existing_local_sims();
    for (std::string a : simul) {sim_presaved->addItem(QString::fromStdString(a));}
    sim_presaved->move(170, 30);
    sim_presaved->resize(150, 25);
    QPushButton* sim_lance = new QPushButton("Lancer", this);
    buttons["sim_lance"] = sim_lance;
    sim_lance->resize(70, 25);
    sim_lance->move(250, 60);
    connect(sim_lance, SIGNAL (clicked()), this, SLOT (lancer_saved_s()));
    #ifdef OVERFLOW_WARNINGS
        std::cout << "debug" << std::endl;
    #endif
}

void MainWindow::init_styles()
{
    QFile style_sh(":/style_sheet.qss");
    style_sh.open(QFile::ReadOnly);

    QString style = QLatin1String(style_sh.readAll());

    this->setStyleSheet(style);
}

void MainWindow::creer()
{
    this->resize(520, 600);
    sim_loc->hide(); sim_presaved->hide(); buttons["sim_lance"]->hide();
    buttons["new_sim"]->hide(); new_taille->hide(); new_entree->hide();
    labels["sim_choix"]->hide();
    //QString::fromWCharArray(L"\x27f2")
    QPushButton* lance = new QPushButton("Lancer", this);
    buttons["lance_new"] = lance;
    connect(lance, SIGNAL (clicked()), this, SLOT (lancer_s()));
    lance->move(410, 10);
    lance->show();
    pos_souris = new QLabel(this);
    pos_souris->move(290, 0);//480, 580
    pos_souris->hide();
    ptr.lmin = (MAX_LIGNES - nb_lines)/2;
    ptr.cmin = (MAX_COLONNES - nb_col)/2;
    ptr.lmax = ptr.lmin + nb_lines;
    ptr.cmax = ptr.cmin + nb_col;
    ptr.vue = new GameOfLifeView(ptr.lmin, ptr.lmax, ptr.cmin, ptr.cmax);
    if (nb_lines == 0) {nb_lines = 1;}
    if (nb_col == 0) {nb_col = 1;}
    ptr.size_cell = 500/nb_lines;
    ptr.px_x = 500;
    ptr.px_y = 500;
    ptr.nb_l_prec = nb_lines;
    ptr.nb_c_prec = nb_col;
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
    charger_calques();
    calques->move(10, 30);
    calques->resize(115, 25);
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
    QPushButton* calque_mod = new QPushButton("calque actif", this);
    buttons["calque_mod"] = calque_mod;
    connect(calque_mod, SIGNAL (clicked()), this, SLOT (calque_switch_s()));
    calque_mod->move(60, 60);
    calque_mod->resize(90, 25);
    calque_mod->show();
    reload_calques = new QPushButton(QString::fromWCharArray(L"\x27f3"), this);
    connect(reload_calques, SIGNAL(clicked()), this, SLOT(reload_calques_s()));
    reload_calques->resize(25, 25);
    reload_calques->move(125, 30);
    reload_calques->show();
    calque.alive = Motif({{0,0}, {1,0}, {2, 0}});
    calque.alive.translate(calque.alive.max_ligne(), calque.alive.max_colonne());
    calque.alive.translate(calque.alive.max_colonne(), calque.alive.max_ligne());
    translate(calque);
    QLabel* eps = new QLabel(" ", this);
    labels["eps"] = eps;
    eps->move(290, 15);
    eps->hide();

    this->setFocus();
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
        QPushButton* new_sim = buttons["new_sim"];
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
        new_entree->setFocus();
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    paint->begin(this);
    if (state == 1)
    {
        Q_UNUSED(event);
        QRect rect1(10, 90, ptr.px_x, ptr.px_y);
        paint->fillRect(rect1, Qt::black);
        bool ok_x(false);
        if (nb_lines*ptr.size_cell < ptr.px_x)
        {
            int width_(ptr.px_x - nb_lines*ptr.size_cell - 1);
            d_x = width_/2;
            QRect rect2(11+nb_lines*ptr.size_cell + d_x, 90, (width_+1)/2, ptr.px_y);
            QRect rect3(11, 90, d_x, ptr.px_y);
            paint->fillRect(rect2, QColor(180,0,0,100));
            paint->fillRect(rect3, QColor(180,0,0,100));            
            ok_x = true;            
        }
        else {d_x = 0;}
        if (nb_col*ptr.size_cell < ptr.px_y)
        {
            int height_(ptr.px_y - nb_col*ptr.size_cell);
            d_y = height_/2;
            QRect rect2(0,0,0,0);
            QRect rect3(0,0,0,0);
            if (ok_x)
            {
                rect2 = QRect(11 + d_x, 90+nb_col*ptr.size_cell + d_y, nb_lines*ptr.size_cell, (height_+1)/2);
                rect3 = QRect(11 + d_x, 90, nb_lines*ptr.size_cell, d_y);
            }
            else 
            {
                rect2 = QRect(11 + d_x, 90+nb_col*ptr.size_cell + d_y, ptr.px_x,(height_+1)/2);
                rect2 = QRect(11 + d_x, 90, ptr.px_x - 1, d_y);
            }
            paint->fillRect(rect2, QColor(180,0,0,100));
            paint->fillRect(rect3, QColor(180,0,0,100));
        }  
        else {d_y = 0;}      
        bool in(false);
        for (auto a : ptr.vue->get_viv())
        {
            if (a.first < nb_lines && a.second < nb_col)
            {
                Cell_ b(a.first*ptr.size_cell + 10 + d_x, a.second*ptr.size_cell + 90 + d_y, ptr.size_cell, ptr.size_cell);
                b.change_color();
                if (b.state()) {paint->fillRect(b.rect(), b.color());}
                if (a.first == size_t(x_current) && a.second == size_t(y_current)) {in = true;}
            }
        }
        if (in)
        {
            paint->setPen(Qt::black);
            QRect rect(x_current*ptr.size_cell + 10 + d_x, y_current*ptr.size_cell + 90 + d_y, ptr.size_cell, ptr.size_cell);
            rect.setWidth(rect.width() - 1);
            rect.setHeight(rect.height() - 1);
            paint->drawRect(rect);
        }
        else if (x_current >= 0 && y_current >= 0)
        {
            paint->setPen(Qt::white);
            QRect rect(x_current*ptr.size_cell + 10 + d_x, y_current*ptr.size_cell + 90 + d_y, ptr.size_cell, ptr.size_cell);
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
            if (min_x >= 0 && size_t(max_x) < nb_lines && min_y >= 0 && size_t(max_y) < nb_col)
            {
                for (size_t a(min_x); a<=size_t(max_x) && max_x >=0; a++)
                {
                    for (size_t b(min_y); b<=size_t(max_y) && max_y >= 0; b++)
                    {
                        QRect rect(a*ptr.size_cell + 10, b*ptr.size_cell + 90, ptr.size_cell, ptr.size_cell);
                        paint->fillRect(rect, QColor(0,0,100,180));
                    }
                }
            }
        }
        else if (calque.on_off && x_current >= 0 && y_current >= 0)
        {
            for (auto const& a : calque.alive)
            {
                if (int(a.first + x_current) - int(calque.translate.first) >=0    &&
                    int(a.second + y_current) - int(calque.translate.second) >= 0 &&
                    a.first + x_current - calque.translate.first < nb_lines       &&
                    a.second + y_current - calque.translate.second < nb_col)
                {
                    QRect rect((a.first + x_current - calque.translate.first)*ptr.size_cell + 10, (a.second + y_current - calque.translate.second)*ptr.size_cell + 90, ptr.size_cell, ptr.size_cell);
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

bool MainWindow::mouse_in(QMouseEvent* event)
{
    return ((event->x() >= 10 + d_x) && (event->x() <= int(ptr.px_x) + 10 - d_x))&&
           ((event->y() >= 90 + d_y) && (event->y() <= int(ptr.px_y) + 90 - d_y));
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (!simul_on && !ctrl_on)
        {
            if (mouse_in(event))
            {
                QPoint pos(pos_souris_rel(event));
                if (calque.on_off)
                {
                    for (auto const& a : calque.alive)
                    {
                        if (int(pos.x() + a.first) - int(calque.translate.first) >= 0 &&
                            int(pos.y() + a.second) - int(calque.translate.second) >= 0 &&
                            pos.x() + a.first  - calque.translate.first < nb_lines    &&
                            pos.y() + a.second - calque.translate.second < nb_col)
                        {
                            ptr.vue->add_cell({pos.x() + a.first  - calque.translate.first,pos.y() + a.second - calque.translate.second});
                        }
                    }
                }
                else { ptr.vue->inv_cell({pos.x(), pos.y()});}
                x_prec = x_current;
                y_prec = y_current;
                x_current = pos.x();
                y_current = pos.y();
                this->update();
            }
        }
    }
    else if (event->button() == Qt::RightButton && state == 1)
    {
        if (mouse_in(event))
        {
            QPoint pos(pos_souris_rel(event));
            x_prec = x_current;
            y_prec = y_current;
            x_current = pos.x();
            y_current = pos.y();
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (state == 1 && event->button() == Qt::RightButton)
    {
        ptr.vue->translate(x_prec - x_current, y_prec - y_current);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (nb_lines >= 1 && event->buttons() != Qt::RightButton)
    {
        if (!(event->modifiers() == Qt::ControlModifier)) {ctrl_on = false;}
        if (mouse_in(event))
        {
            x_prec = x_current;
            y_prec = y_current;
            QPoint pos(pos_souris_rel(event));
            x_current = pos.x();
            y_current = pos.y();
            if ((event->buttons() == Qt::LeftButton && (x_prec != x_current || y_prec != y_current) && !simul_on) && !ctrl_on)
            {
                if (x_prec != -1 && y_prec != -1)
                {
                    liste a_ajouter(segment({x_prec, y_prec}, {x_current, y_current}));
                    for (auto const& a : a_ajouter)
                    {
                        ptr.vue->add_cell(a);
                    }
                }
                //ptr.vue->inv_cell({x_current, y_current});
            }
        }
        else {x_current = -1; y_current = -1;}
        this->update();
    }
    if (state != 0) { pos_souris->setText(QString::number(x_current) + " " + QString::number(y_current));}
    if (state == 1 && event->buttons() == Qt::RightButton)
    {
        if (mouse_in(event))
        {
            ptr.vue->translate(x_prec - x_current, y_prec - y_current);
            ptr.lmin = ptr.vue->get_Lmin();
            ptr.cmin = ptr.vue->get_Cmin();
            ptr.lmax = ptr.vue->get_Lmax();
            ptr.cmax = ptr.vue->get_Cmax();
            //std::cout << "(" << ptr.lmin << ", " << ptr.lmax << ", " << ptr.cmin << ", " << ptr.cmax << ")" << std::endl;
            QPoint pos(pos_souris_rel(event));
            x_prec = x_current;
            y_prec = y_current;
            x_current = pos.x();
            y_current = pos.y();
            this->update();
        }
    }
}

void MainWindow::wheelEvent(QWheelEvent* event)
{
    //int nbr_degres = event->angleDelta().y() / 12;
    // 10% par cran = 10% par 15° = 10% par 120 huitième de degré = 1% par 12 huitièmes de degrés

    //double taux(1- (nbr_pourcent/100.0));

    //double taux(0.5);

    //std::cerr << "1: Lmin = " << ptr.vue->get_Lmin() << " Lmax = " << ptr.vue->get_Lmax() << " Cmin = " << ptr.vue->get_Cmin() << " Cmax = " << ptr.vue->get_Cmax() << std::endl;

    //std::cout << "pos : " << x_current << " | " << y_current << std::endl;

    QPoint delta(event->angleDelta());
    QPoint delta_pix(event->pixelDelta());
    //std::cout << "(" << delta.x() << ", " << delta.y();
    //std::cout << ") | (" << delta_pix.x() << ", " << delta_pix.y() << ")" << std::endl;
    int mouv(0);
    /*
    if (!delta_pix.isNull())
    {
        std::cout << " pixel : " << delta_pix.y() << std::endl;
        mouv = delta_pix.y()/100;
    }
    */
    // angleDelta en 8eme de degré
    // 120 = 15°  (24 crans sur une souris standard)
    
    if (delta.y()%120 == 0)
    {
        //std::cout << "Not null ";
        mouv = delta.y()/12;
        if (mouv >= 0 && mouv <= 10) {
            mouv = 10;
        } else if (mouv < 0 && mouv >= -10) {
            mouv = -10;
        }
    
    }
    else 
    {
        //std::cout << "null";
        int delta_pix_current(delta_pix.y());
        mouv = delta_pix_current - delta_pix_prec;
        mouv *= 10;
        delta_pix_prec = delta_pix_current;
    }
    //std::cout << mouv << std::endl;
    //std::cerr << delta.y() << " " << mouv << std::endl;

    mouv = (mouv/10)*10;//cheat code

    //std::cout << mouv << std::endl;

    double taux = 1.0 + (double(mouv/10)/10.0);
    int taux_2 = mouv/10;
    std::cout << taux_2 << std::endl;

    //std::cout << taux << std::endl;
    /*
    if (!(taux < 1.0 && (nb_lines <= 2 || nb_col <= 2)))
    {
        ptr.lmax = ptr.lmin + ptr.nb_l_prec;
        ptr.cmax = ptr.cmin + ptr.nb_c_prec;
        ptr.vue->resize(ptr.lmin, ptr.lmax, ptr.cmin, ptr.cmax);        
        ptr.vue->zoom({x_current, y_current}, taux);//en coord relatives parce que Tudor

        nb_lines = ptr.vue->nbr_lignes();
        nb_col = ptr.vue->nbr_colonnes();
        //std::cout << taux << " : " << ptr.px_x << " -> " << nb_lines << " | " << ptr.size_cell << std::endl;
        ptr.size_cell = ptr.px_x/nb_lines;
        ptr.nb_l_prec = nb_lines;
        ptr.nb_c_prec = nb_col;

        //std::cout << (ptr.px_x - ptr.size_cell*nb_lines)/ptr.size_cell << std::endl;
        nb_lines += (ptr.px_x - ptr.size_cell*nb_lines)/ptr.size_cell;
        nb_col += (ptr.px_y - ptr.size_cell*nb_col)/ptr.size_cell;
        //ptr.lmin -= nb_lines/2;
        ptr.lmax = ptr.lmin + nb_lines;
        //ptr.cmin -= nb_col/2;
        ptr.cmax = ptr.cmin + nb_col;
        if (ptr.lmax >= MAX_LIGNES)
        {
            ptr.lmin += MAX_LIGNES - ptr.lmax - 1;
            ptr.lmax = MAX_LIGNES - 1;
        }
        if (ptr.cmax >= MAX_COLONNES)
        {
            ptr.cmin += MAX_COLONNES - ptr.cmax - 1;
            ptr.cmax = MAX_COLONNES - 1;
        } 
        if (ptr.lmin < 0) {ptr.lmin = 0;}
        if (ptr.cmin < 0) {ptr.cmin = 0;}

        ptr.vue->resize(ptr.lmin, ptr.lmax, ptr.cmin, ptr.cmax);
        std::cout << ptr.lmin << " " << ptr.lmax << " " << ptr.cmin << " " << ptr.cmax << " ";
        std::cout << taux << " : " << ptr.nb_l_prec << " -> " << nb_lines << " | " << ptr.size_cell << std::endl;
    }
    */
    //std::cerr << "2: Lmin = " << ptr.vue->get_Lmin() << " Lmax = " << ptr.vue->get_Lmax() << " Cmin = " << ptr.vue->get_Cmin() << " Cmax = " << ptr.vue->get_Cmax() << std::endl;
    
    //std::cout << taux_2 << std::endl;

    double new_size(ptr.size_cell);
    if (taux_2 < 0) {new_size /= (1.5*(-taux_2));}
    else if (taux_2 > 0) {new_size *= (1.5*taux_2);}

    switch (ptr.px_x < ptr.px_y)
    {
        case true:
            if (new_size > ptr.px_x) {new_size = ptr.px_x;}
            break;
        case false:
            if (new_size > ptr.px_y) {new_size = ptr.px_y;}
            break;
        default: break;
    }
    if (new_size == 1.5) {new_size = 2.0;}


    if (new_size >= 1 && new_size <= ptr.px_x && new_size <= ptr.px_y)
    {
        ptr.size_cell = new_size;
        nb_lines = ptr.px_x/ptr.size_cell;
        nb_col = ptr.px_y/ptr.size_cell;
        //std::cout << " >> " << nb_lines << " | " << nb_col << std::endl;        
        if (MAX_LIGNES*ptr.size_cell < ptr.px_x) 
        {
            nb_lines = MAX_LIGNES;
            ptr.lmin = 0;
            ptr.lmax = MAX_LIGNES;
        } else {
            ptr.lmin += x_current - nb_lines/2;
            ptr.lmax = ptr.lmin + nb_lines;             
        }
        if (MAX_COLONNES*ptr.size_cell < ptr.px_y) 
        {
            nb_col = MAX_COLONNES;
            ptr.cmin = 0;
            ptr.cmax = MAX_COLONNES;
        } else {
            ptr.cmin += y_current - nb_col/2;
            ptr.cmax = ptr.cmin + nb_col;              
        }




        //std::cout << ptr.lmax << " , " << ptr.lmin << " , " << ptr.cmax << " , " << ptr.cmin << std::endl;                                
        if (ptr.lmin < 0) {ptr.lmax -= ptr.lmin; ptr.lmin = 0;}

        if (ptr.lmax > MAX_LIGNES) 
        {
            ptr.lmin -= (ptr.lmax - MAX_LIGNES);
            if (ptr.lmin < 0) {ptr.lmin = 0;}
            ptr.lmax = MAX_LIGNES;
        }

        if (ptr.cmin < 0) {ptr.cmax -= ptr.cmin; ptr.cmin = 0;}

        if (ptr.cmax > MAX_COLONNES) 
        {
            ptr.cmin -= ptr.cmax - MAX_COLONNES;
            if (ptr.cmin < 0) {ptr.cmin = 0;}
            ptr.cmax = MAX_COLONNES;
        }

        //std::cout << ptr.lmax << " | " << ptr.lmin << " | " << ptr.cmax << " | " << ptr.cmin << std::endl; 
        //std::cout << ptr.lmax - ptr.lmin << " | " << ptr.cmax -ptr.cmin << std::endl;         
        ptr.vue->resize(ptr.lmin, ptr.lmax, ptr.cmin, ptr.cmax);
        nb_lines = ptr.vue->nbr_lignes();
        nb_col = ptr.vue->nbr_colonnes(); 
        //std::cout << nb_lines << " | " << nb_col << std::endl;
    }

    this->update();
    /*QPoint delta(event->angleDelta());
    // angleDelta en 8eme de degré
    // 120 = 15°  (24 crans sur une souris standard)
    int mouv(delta.y()/12);
    //std::cerr << delta.y() << " " << mouv << std::endl;
    if (mouv >= 0 && mouv <= 10) {
        mouv = 10;
    } else if (mouv < 0 && mouv >= -10) {
        mouv = -10;
    }
    mouv = (mouv/10)*10;
    if ((0 < mouv + nb_lines && mouv + nb_lines <= 400 && 0 < mouv + nb_col && mouv + nb_col <= 400) &&
        (x_current != -1 && y_current != -1))
    {
        ptr.lmin += -(((int(nb_lines)/2)-x_current)) - mouv/2;
        if (ptr.lmin < 0) {ptr.lmin = 0;}
        ptr.cmin += -(((int(nb_col)/2)-y_current)) - mouv/2;
        if (ptr.cmin < 0) {ptr.cmin = 0;}
        nb_lines += mouv;
        nb_col += mouv;
        ptr.lmax = ptr.lmin + nb_lines;
        if (ptr.lmax > MAX_LIGNES) {ptr.lmax = MAX_LIGNES;}
        ptr.cmax = ptr.cmin + nb_col;
        if (ptr.cmax > MAX_COLONNES) {ptr.cmax = MAX_COLONNES;}
        //std::cout << ptr.lmin << " | " << ptr.cmin << std::endl;
        ptr.vue->resize(ptr.lmin, ptr.lmax, ptr.cmin, ptr.cmax);
        //std::cerr << nb_lines << " " << nb_col << std::endl;
        //ptr.vue->resize(nb_lines, nb_col); A réimplémenter pour l'instant flemme.
        this->update();
    }*/
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (timer != 0)
    {
        Q_UNUSED(event);
        clock_t T1(clock());
        ptr.vue->evolve();
        double ms((double(clock() - T1)/CLOCKS_PER_SEC)*1000);
        if (ms < 50) {ms = 50;}
        labels["eps"]->setText(QString::number(1000/ms));
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
    if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) && state == 0)
    {
        buttons["new_sim"]->click();
    }
    if (event->key() == Qt::Key_I)
    {
        info_on = 1 - info_on;
        if (info_on)
        {
            if (pos_souris != nullptr) {pos_souris->show();}
            if (state != 0) {labels["eps"]->show();}
        }
        else
        {
            if (pos_souris != nullptr) {pos_souris->hide();}
            if (state != 0) {labels["eps"]->hide();}            
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    if (buttons["lance_new"] != nullptr && !buttons["lance_new"]->isHidden())
    {
        buttons["lance_new"]->move(this->width()-110, 10);
    }
    if (pause != nullptr && save_game != nullptr && buttons["lance_new"] != nullptr)
    {
        pause->move(this->width()-110, 10);
        save_game->move(this->width()-110, 50);
    }
    if (state != 0)
    {
        ptr.px_x = (this->width() - 20);
        ptr.px_y = (this->height() - 100);
        nb_lines = ptr.px_x/ptr.size_cell;
        nb_col = ptr.px_y/ptr.size_cell;
        //std::cout << " >> " << nb_lines << " | " << nb_col << std::endl;        
        if (MAX_LIGNES*ptr.size_cell < ptr.px_x) 
        {
            nb_lines = MAX_LIGNES;
            ptr.lmin = 0;
            ptr.lmax = MAX_LIGNES;
        } else {
            //ptr.lmin += x_current - nb_lines/2;
            ptr.lmax = ptr.lmin + nb_lines;             
        }
        if (MAX_COLONNES*ptr.size_cell < ptr.px_y) 
        {
            nb_col = MAX_COLONNES;
            ptr.cmin = 0;
            ptr.cmax = MAX_COLONNES;
        } else {
            //ptr.cmin += y_current - nb_col/2;
            ptr.cmax = ptr.cmin + nb_col;              
        }




        //std::cout << ptr.lmax << " , " << ptr.lmin << " , " << ptr.cmax << " , " << ptr.cmin << std::endl;                                
        if (ptr.lmin < 0) {ptr.lmax -= ptr.lmin; ptr.lmin = 0;}

        if (ptr.lmax > MAX_LIGNES) 
        {
            ptr.lmin -= (ptr.lmax - MAX_LIGNES);
            if (ptr.lmin < 0) {ptr.lmin = 0;}
            ptr.lmax = MAX_LIGNES;
        }

        if (ptr.cmin < 0) {ptr.cmax -= ptr.cmin; ptr.cmin = 0;}

        if (ptr.cmax > MAX_COLONNES) 
        {
            ptr.cmin -= ptr.cmax - MAX_COLONNES;
            if (ptr.cmin < 0) {ptr.cmin = 0;}
            ptr.cmax = MAX_COLONNES;
        }

        //std::cout << ptr.lmax << " | " << ptr.lmin << " | " << ptr.cmax << " | " << ptr.cmin << std::endl; 
        //std::cout << ptr.lmax - ptr.lmin << " | " << ptr.cmax -ptr.cmin << std::endl;         
        ptr.vue->resize(ptr.lmin, ptr.lmax, ptr.cmin, ptr.cmax);
        nb_lines = ptr.vue->nbr_lignes();
        nb_col = ptr.vue->nbr_colonnes();    
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
        if (min_x >= 0 && max_x < int(nb_lines) && min_y >= 0 && max_y < int(nb_col))
        {
            Calque temp;
            for (auto a : ptr.vue->get_viv())
            {
                if (((long long int)(a.first) >= min_x && (long long int)(a.first) <= max_x) &&
                    ((long long int)(a.second) >= min_y && (long long int)(a.second) <= max_y))
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

void MainWindow::charger_calques()
{
    calques->clear();
    calques->addItem("--select--");
    std::vector<std::string> motifs_locaux(existing_local_motifs());
    for (std::string a : motifs_locaux) {calques->addItem(QString::fromStdString(a));}
    nb_motifs_locaux = calques->count();
    calques->insertSeparator(nb_motifs_locaux);
    std::vector<std::string> motifs_presaved(existing_presaved_motifs());
    for (std::string a : motifs_presaved) {calques->addItem(QString::fromStdString(a));}
}

void MainWindow::lancer_s()
{
    timer = startTimer(50);
    buttons["lance_new"]->hide();
    pause = new QPushButton("pause", this);
    connect(pause, SIGNAL (clicked()), this, SLOT (pause_s()));
    pause->move(this->width()-110, 10);
    pause->show();
    save_game = new QPushButton("save", this);
    connect(save_game, SIGNAL (clicked()), this, SLOT (save_game_s()));
    save_game->move(this->width()-110, 50);
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
        buttons["calque_mod"]->setDown(false);
    }
    else {buttons["calque_mod"]->setDown(true);}
    calque.on_off = 1 - calque.on_off;
}

void MainWindow::combo_time(int i)
{
    Q_UNUSED(i);
    if (frame_on &&
        QCursor::pos().x() >= calques->mapToGlobal(calques->view()->pos()).x()                            &&
        QCursor::pos().y() >= calques->mapToGlobal(calques->view()->pos()).y() + calques->height()        &&
        QCursor::pos().x() <= calques->mapToGlobal(calques->view()->pos()).x() + calques->view()->width() &&
        QCursor::pos().y() <= calques->mapToGlobal(calques->view()->pos()).y() + calques->view()->height())
    {
        if (calques->itemText(calques->view()->currentIndex().row()) != "--select--")
        {
            detail_selectionne->load(calques->itemText(calques->view()->currentIndex().row()).toStdString(), (calques->view()->currentIndex().row() <= nb_motifs_locaux));
            detail_selectionne->move(QCursor::pos());
            detail_selectionne->show();
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
    if (calque.on_off) {buttons["calque_mod"]->setDown(true);}
}

void MainWindow::item_changed_s(QString const& entree)
{
    if (entree != "" && entree != "--select--")
    {
        FILE_CATEGORY emplacement(presaved);
        if ((calques->view()->currentIndex().row() <= nb_motifs_locaux)) {emplacement = local;}
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

void MainWindow::reload_calques_s()
{
    charger_calques();
}

MainWindow::~MainWindow()
{
    delete sim_loc;
    delete sim_presaved;
    delete new_taille;
    delete new_entree;
    delete paint;
    delete pause;
    delete pos_souris;
    delete calques;
    delete detail_selectionne;
    delete ptr.vue;
    delete save_game;
    for (auto& a : labels) {delete a.second;}
    for (auto& a : buttons) {delete a.second;}
    delete reload_calques;
}
