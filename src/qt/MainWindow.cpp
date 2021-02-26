//
// Created by Tudor Oancea on 26/02/2021.
//


#include "MainWindow.hpp"
#include "GraphicsView.hpp"
#include "Cell.hpp"
#include "termcolor.hpp"

#include <QApplication>
#include <QGraphicsObject>
#include <QPainter>
#include <QPointF>
#include <QTransform>
#include <QtWidgets>
#include <QEvent>

#include <thread>
#include <chrono>
#include <iostream>

using namespace std::literals::chrono_literals;

MainWindow::MainWindow()
		: selectedArea(QRect(100,100,100,100)),
		  newSelectedArea(QRect(100,100,100,100)),
		  gridLabel(new QLabel("Future Grid", this)),
		  scene(new QGraphicsScene(0.0, 0.0, MAX_LIGNES, MAX_COLONNES)),
		  view(new GraphicsView(scene, this)),
		  game(new GameOfLifeView)
{
	createActions();
	createMenus();
	createToolBars();
	gridLabel->resize(200,100);
	gridLabel->move(this->geometry().center() - gridLabel->geometry().center());
	gridLabel->hide();
	
	this->setWindowTitle("golbis");
	this->resize(500,500);
	this->move(QGuiApplication::screens().at(0)->geometry().center() - frameGeometry().center());
	this->setUnifiedTitleAndToolBarOnMac(true);
	this->setCentralWidget(view);
	this->createFrame();
	this->addToolBar(mainToolBar);
	
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	this->refreshScene();
	
	view->setScene(scene);
	view->setBackgroundBrush(QBrush(Qt::white));
	view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	view->setRenderHint(QPainter::Antialiasing);
	view->setCacheMode(QGraphicsView::CacheBackground);
	view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	view->show();
	connect(view, &GraphicsView::modifyCellIntention, this, &MainWindow::modifyCell);
	
	this->setFocus();

//	timerIdcreateAct = this->startTimer(period);
}


void MainWindow::createActions() {
//	File Menu ========================================================================================
	newSimAct = new QAction("New Simulation", this);
	newSimAct->setShortcut(QKeySequence::New);
	connect(newSimAct, SIGNAL(triggered()), this, SLOT(newSim()));
	
	openAct = new QAction("Open");
	openAct->setShortcut(QKeySequence::Open);
	connect(openAct, &QAction::triggered, this, &MainWindow::open);
	
	saveMotifAct = new QAction("Save Motif", this);
	connect(saveMotifAct, &QAction::triggered, this, &MainWindow::saveMotif);
	
	saveSimAct = new QAction("Save Simulation", this);
	connect(saveSimAct, &QAction::triggered, this, &MainWindow::saveSim);
	
	aboutAct = new QAction("About", this);
	connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

//	Edit Menu ========================================================================================
	undoAct = new QAction("Undo", this);
	undoAct->setShortcut(QKeySequence::Undo);
	connect(undoAct, &QAction::triggered, this, &MainWindow::undo);
	
	redoAct = new QAction("Redo", this);
	redoAct->setShortcut(QKeySequence::Redo);
	connect(redoAct, &QAction::triggered, this, &MainWindow::redo);
	
	copyAct = new QAction("Copy", this);
	copyAct->setShortcut(QKeySequence::Copy);
	connect(copyAct, &QAction::triggered, this, &MainWindow::copy);
	
	pasteAct = new QAction("Paste", this);
	pasteAct->setShortcut(QKeySequence::Paste);
	connect(pasteAct, &QAction::triggered, this, &MainWindow::paste);
	
	cutAct = new QAction("Cut", this);
	cutAct->setShortcut(QKeySequence::Cut);
	connect(cutAct, &QAction::triggered, this, &MainWindow::cut);
	
	clearAct = new QAction("Clear", this);
	clearAct->setShortcut(QKeySequence(Qt::Key_K));
	connect(clearAct, &QAction::triggered, this, &MainWindow::clear);
	
	new QAction("Suppr", this);

//	View Menu ========================================================================================
	zoomInAct = new QAction("Zoom In", this);
	zoomInAct->setShortcut(QKeySequence::ZoomIn);
	connect(zoomInAct, &QAction::triggered, this, &MainWindow::zoomIn);
	
	zoomOutAct = new QAction("Zoom Out", this);
	zoomOutAct->setShortcut(QKeySequence::ZoomOut);
	connect(zoomOutAct, &QAction::triggered, this, &MainWindow::zoomOut);
	
	pauseResumeAct = new QAction("Pause/Resume", this);
	pauseResumeAct->setShortcut(QKeySequence(Qt::Key_Space));
	connect(pauseResumeAct, &QAction::triggered, this, &MainWindow::pauseResume);
	
}

void MainWindow::createMenus() {
	fileMenu = menuBar()->addMenu("File");
	fileMenu->addAction(newSimAct);
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveMotifAct);
	fileMenu->addAction(saveSimAct);
	fileMenu->addAction(aboutAct);
	
	editMenu = menuBar()->addMenu("Edit");
	editMenu->addAction(undoAct);
	editMenu->addAction(redoAct);
	editMenu->addAction(copyAct);
	editMenu->addAction(cutAct);
	editMenu->addAction(pasteAct);
	editMenu->addAction(clearAct);
	
	viewMenu = menuBar()->addMenu("View");
	viewMenu->addAction(pauseResumeAct);
	viewMenu->addAction(zoomInAct);
	viewMenu->addAction(zoomOutAct);
}

MainWindow::~MainWindow() {
	delete gridLabel;
	delete scene;
	delete game;
	delete view;
	
	delete newSimAct;
	delete openAct;
	delete saveMotifAct;
	delete saveSimAct;
	delete aboutAct;
	
	delete undoAct;
	delete redoAct;
	delete copyAct;
	delete pasteAct;
	delete cutAct;
	delete clearAct;
	
	delete zoomInAct;
	delete zoomOutAct;
	delete pauseResumeAct;
	
	delete fileMenu;
	delete editMenu;
	delete viewMenu;
}

void MainWindow::placeholder(const char* str) {
	std::cerr << termcolor::cyan << "\n Action " << str << termcolor::reset << std::endl;
}

void MainWindow::about() {
	placeholder("About");
}

void MainWindow::saveMotif() {
	placeholder("saveMotif");
}

void MainWindow::saveSim() {
	placeholder("saveSim");
}

void MainWindow::undo() {
	placeholder("undo");
}

void MainWindow::redo() {
	placeholder("redo");
}

void MainWindow::copy() {
	placeholder("copy");
}

void MainWindow::paste() {
	placeholder("paste");
}

void MainWindow::cut() {
	placeholder("cut");
}

void MainWindow::zoomOut() {
	view->scaleFactor() /= 2;
	view->setTransform(QTransform::fromScale(view->scaleFactor(), view->scaleFactor()));
	view->update();
}

void MainWindow::zoomIn() {
	view->scaleFactor() *= 2;
	view->setTransform(QTransform::fromScale(view->scaleFactor(), view->scaleFactor()));
	view->update();
}

void MainWindow::newSim() {
	placeholder("newSim");
}

void MainWindow::open() {
	placeholder("open");
}


void MainWindow::timerEvent(QTimerEvent* event) {
	Q_UNUSED(event)
	game->evolve();
	this->refreshScene();
}

void MainWindow::pauseResume() {
	if (timerId != 0) {
		this->killTimer(timerId);
		timerId = 0;
	} else {
		timerId = this->startTimer(period);
	}
}

void MainWindow::clear() {
	game->wipe();
	this->refreshScene();
}

void MainWindow::createFrame() {
	QGraphicsLineItem* xaxis(new QGraphicsLineItem(0.0, MAX_LIGNES/2, MAX_COLONNES, MAX_LIGNES/2));
	QGraphicsLineItem* yaxis(new QGraphicsLineItem(MAX_COLONNES/2, 0.0, MAX_COLONNES/2, MAX_LIGNES));
	xaxis->setPen(QPen(Qt::red));
	yaxis->setPen(QPen(Qt::green));
	scene->addItem(xaxis);
	scene->addItem(yaxis);
	
	QGraphicsLineItem* top(new QGraphicsLineItem(0.0, 0.0, MAX_COLONNES, 0.0));
	top->setPen(QPen(Qt::blue));
	scene->addItem(top);
	QGraphicsLineItem* left(new QGraphicsLineItem(0.0, 0.0, 0.0, MAX_LIGNES));
	left->setPen(QPen(Qt::blue));
	scene->addItem(left);
	QGraphicsLineItem* right(new QGraphicsLineItem(MAX_COLONNES, 0.0, MAX_COLONNES, MAX_LIGNES));
	right->setPen(QPen(Qt::blue));
	scene->addItem(right);
	QGraphicsLineItem* bottom(new QGraphicsLineItem(0.0, MAX_LIGNES, MAX_COLONNES, MAX_LIGNES));
	bottom->setPen(QPen(Qt::blue));
	scene->addItem(bottom);
}

void MainWindow::refreshScene() {
	scene->clear();
	this->createFrame();
//	auto start(std::chrono::high_resolution_clock::now());
	for (auto const& c : game->vivantes()) {
		scene->addItem(new Cell((double) c.first, (double) c.second));
	}
//	auto stop(std::chrono::high_resolution_clock::now());
}

void MainWindow::addCell(const size_t& i, const size_t& j) {
	if (game != nullptr) {
		game->addCell(i,j);
		this->refreshScene();
	}
}

void MainWindow::deleteCell(const size_t& i, const size_t& j) {
	if (game != nullptr) {
		game->deleteCell(i, j);
		this->refreshScene();
	}
}

void MainWindow::inverseCell(size_t const& i, size_t const& j) {
	if (game != nullptr) {
		game->inverseCell(i, j);
		this->refreshScene();
	}
}

void MainWindow::createToolBars() {
	stateBox = new QComboBox;
	stateBox->addItem("Select");
	stateBox->addItem("Add");
	stateBox->addItem("Delete");
	stateBox->setToolTip("How you can modify the grid");
	connect(stateBox, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::setModifyState);
	mainToolBar = new QToolBar;
	mainToolBar->addWidget(stateBox);
	mainToolBar->addAction(pauseResumeAct);
	mainToolBar->addAction(clearAct);
	mainToolBar->addAction(undoAct);
	mainToolBar->addAction(redoAct);
	mainToolBar->addAction(zoomInAct);
	mainToolBar->addAction(zoomOutAct);
	
}

void MainWindow::modifyCell(const size_t& i, const size_t& j) {
	if (game != nullptr) {
		switch (modifyState_) {
			case Adding:
				game->addCell(i,j);
				break;
			case Deleting:
				game->deleteCell(i,j);
				break;
			default:
				break;
		}
		this->refreshScene();
	}
}

void MainWindow::setModifyState(const int& modifyState) {
	switch (modifyState) {
		case 1:{
			this->modifyState_ = Adding;
			stateBox->setCurrentIndex(1);
			std::cout << "Adding" << std::endl;
			break;
		}
		case 2:{
			this->modifyState_ = Deleting;
			stateBox->setCurrentIndex(2);
			std::cout << "Deleting" << std::endl;
			break;
		}
		default:{
			this->modifyState_ = Selecting;
			stateBox->setCurrentIndex(0);
			std::cout << "Selecting" << std::endl;
			break;
		}
	}
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
	QWidget::keyPressEvent(event);
	switch (event->key()) {
		case Qt::Key_S:
			this->setModifyState(Selecting);
			break;
		case Qt::Key_A:
			this->setModifyState(Adding);
			break;
		case Qt::Key_D:
			this->setModifyState(Deleting);
			break;
	}
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
	QWidget::mousePressEvent(event);
}

void MainWindow::wheelEvent(QWheelEvent* event) {
	QWidget::wheelEvent(event);
	if (!hasTouchEvent) {
		int steps(event->angleDelta().y() / 120);
		view->scaleFactor() *= pow(2,steps);
		view->setTransform(QTransform::fromScale(view->scaleFactor(), view->scaleFactor()));
		view->update();
	}
}

bool MainWindow::event(QEvent* event) {
	switch (event->type()) {
		case QEvent::TouchBegin:
			hasTouchEvent = true;
			return true;
		case QEvent::TouchEnd:
			hasTouchEvent = false;
			return true;
		default:
			break;
	}
	return QMainWindow::event(event);
}

void MainWindow::paintEvent(QPaintEvent* event) {
	QWidget::paintEvent(event);
//	view->hide();
	gridLabel->hide();
	if (modifyState_ == Selecting) {
		QPainter painter(this);
		painter.begin(this);
		painter.fillRect(QRect(100,100,100,100), Qt::blue);
		painter.end();
	}
	
}