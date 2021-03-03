//
// Created by Tudor Oancea on 26/02/2021.
//


#include "MainWindow.hpp"
#include "GraphicsView.hpp"
#include "Cell.hpp"
#include "termcolor.hpp"

#include <QApplication>
#include <QObject>
#include <QGraphicsObject>
#include <QPainter>
#include <QPointF>
#include <QTransform>
#include <QtWidgets>
#include <QEvent>

#include <iterator>
#include <list>
#include <deque>
#include <thread>
#include <chrono>
#include <iostream>

using namespace std::literals::chrono_literals;

MainWindow::MainWindow()
		: lastModif(historic.begin()),
		  scene(new QGraphicsScene(0.0, 0.0, MAX_LIGNES, MAX_COLONNES)),
		  view(new GraphicsView(scene, this)),
		  game(new GameOfLifeView)
{
	labels.fill(nullptr);
	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
	
	this->setWindowTitle("golbis");
	this->resize(500,500);
	this->move(QGuiApplication::screens()[0]->geometry().center() - frameGeometry().center());
	this->setUnifiedTitleAndToolBarOnMac(true);
	this->setCentralWidget(view);
	
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	this->refreshScene();
	
	view->setBackgroundBrush(QBrush(Qt::white));
	view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	view->setRenderHint(QPainter::Antialiasing);
	view->setCacheMode(QGraphicsView::CacheBackground);
	view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	view->show();
	connect(view, SIGNAL(modifyCellIntention(size_t const&, size_t const&, bool)), this, SLOT(modifyCell(size_t const&, size_t const&, bool)));
	
	this->setFocus();
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
	saveMotifAct->setIcon(QIcon(":/images/icons8-save.png"));
	connect(saveMotifAct, &QAction::triggered, this, &MainWindow::saveMotif);
	
	saveSimAct = new QAction("Save Simulation", this);
	connect(saveSimAct, &QAction::triggered, this, &MainWindow::saveSim);
	

//	Edit Menu ========================================================================================
	undoAct = new QAction(QIcon(":/images/icons8-undo.png"),"Undo", this);
	undoAct->setShortcut(QKeySequence::Undo);
	connect(undoAct, &QAction::triggered, this, &MainWindow::undo);
	
	redoAct = new QAction(QIcon(":/images/icons8-redo.png"), "Redo", this);
	redoAct->setShortcut(QKeySequence::Redo);
	connect(redoAct, &QAction::triggered, this, &MainWindow::redo);
	
	copyAct = new QAction(QIcon(":/images/icons8-copy.png"), "Copy", this);
	copyAct->setShortcut(QKeySequence::Copy);
	connect(copyAct, &QAction::triggered, this, &MainWindow::copy);
	
	pasteAct = new QAction(QIcon(":/images/icons8-paste.png"), "Paste", this);
	pasteAct->setShortcut(QKeySequence::Paste);
	connect(pasteAct, &QAction::triggered, this, &MainWindow::paste);
	
	cutAct = new QAction(QIcon(":/images/icons8-cut.png"), "Cut", this);
	cutAct->setShortcut(QKeySequence::Cut);
	connect(cutAct, &QAction::triggered, this, &MainWindow::cut);
	
	clearAct = new QAction(QIcon(":/images/icons8-delete.png"), "Clear", this);
	clearAct->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_K));
	connect(clearAct, &QAction::triggered, this, &MainWindow::clear);
	
//	View Menu ========================================================================================
	zoomInAct = new QAction(QIcon(":/images/icons8-zoom_in.png"), "Zoom In", this);
	zoomInAct->setShortcut(QKeySequence::ZoomIn);
	connect(zoomInAct, &QAction::triggered, this, &MainWindow::zoomIn);
	
	zoomOutAct = new QAction(QIcon(":/images/icons8-zoom_out.png"),"Zoom Out", this);
	zoomOutAct->setShortcut(QKeySequence::ZoomOut);
	connect(zoomOutAct, &QAction::triggered, this, &MainWindow::zoomOut);
	
	resetZoomAct = new QAction(QIcon(":/images/icons8-zoom_to_actual_size.png"), "Reset Zoom", this);
	resetZoomAct->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_0));
	connect(resetZoomAct, &QAction::triggered, this, &MainWindow::resetZoom);
	
	pauseResumeAct = new QAction(QIcon(":/images/icons8-play.png"),"Pause/Resume", this);
	pauseResumeAct->setShortcut(QKeySequence(Qt::Key_Space));
	connect(pauseResumeAct, &QAction::triggered, this, &MainWindow::pauseResume);
	
//	Help Menu ========================================================================================
	aboutAct = new QAction(QIcon(":/images/icons8-about.png"), "About", this);
	connect(aboutAct, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::createMenus() {
	fileMenu = menuBar()->addMenu("File");
	fileMenu->addAction(newSimAct);
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveMotifAct);
	fileMenu->addAction(saveSimAct);
	
	editMenu = menuBar()->addMenu("Edit");
	editMenu->addAction(undoAct);
	editMenu->addAction(redoAct);
	editMenu->addAction(copyAct);
	editMenu->addAction(cutAct);
	editMenu->addAction(pasteAct);
	editMenu->addAction(clearAct);
	
	viewMenu = menuBar()->addMenu("View");
	editMenu->addAction(pauseResumeAct);
	viewMenu->addAction(zoomInAct);
	viewMenu->addAction(zoomOutAct);
	viewMenu->addAction(resetZoomAct);

	helpMenu = menuBar()->addMenu("Help");
	helpMenu->addAction(aboutAct);
}

MainWindow::~MainWindow() {
	delete scene;
	delete game;
	delete view;
	for (const auto & label : labels) {
		delete label;
	}
	
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
	delete helpMenu;
	delete stateBox;
	delete mainToolBar;
//	delete statusBar;
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
//	placeholder("undo Action");
//	if (std::distance(lastModif, historic.end()) > 1) {
//		std::cerr << termcolor::blue << "Undo :";
//		if (lastModif->first) {
//			game->deleteMotif(lastModif->second);
//			++lastModif;
//			for (auto& jt : lastModif->second) {
//				std::cerr << termcolor::green << jt << " | " << termcolor::reset;
//			}
//		} else {
//			game->addMotif(lastModif->second);
//			++lastModif;
//			for (auto& jt : lastModif->second) {
//				std::cerr << termcolor::red << jt << " | " << termcolor::reset;
//			}
//		}
//		std::cout << termcolor::reset << std::endl;
//		this->updateStatusBar();
//		this->refreshScene();
	if (lastModif != historic.end()) {
		if (lastModif->first) {
			game->deleteMotif(lastModif->second);
		} else {
			game->addMotif(lastModif->second);
		}
		++lastModif;
		this->updateStatusBar();
		this->refreshScene();
	} else {
		std::cerr << termcolor::red << "Il n'y a pas de modification plus ancienne enregistrée." << termcolor::reset << std::endl;
	}
}

void MainWindow::redo() {
//	placeholder("redo Action");
//	if (lastModif != historic.begin()) {
//		std::cerr << termcolor::blue << "Redo : ";
//		if (lastModif->first) {
//			game->addMotif(lastModif->second);
//			for (auto & jt : lastModif->second) {
//				std::cerr << termcolor::green << jt << " | " << termcolor::reset;
//			}
//			--lastModif;
//		} else {
//			game->deleteMotif(lastModif->second);
//			for (auto & jt : lastModif->second) {
//				std::cerr << termcolor::red << jt << " | " << termcolor::reset;
//			}
//			--lastModif;
//		}
//		if (lastModif == historic.begin()) {
//			if (lastModif->first) {
//				game->addMotif(lastModif->second);
//				for (auto & jt : lastModif->second) {
//					std::cerr << termcolor::green << jt << " | " << termcolor::reset;
//				}
//			} else {
//				game->deleteMotif(lastModif->second);
//				for (auto & jt : lastModif->second) {
//					std::cerr << termcolor::red << jt << " | " << termcolor::reset;
//				}
//			}
//		}
//		std::cout << termcolor::reset << std::endl;
	if (lastModif != historic.begin()) {
		--lastModif;
		if (lastModif->first) {
			game->addMotif(lastModif->second);
		} else {
			game->deleteMotif(lastModif->second);
		}
		this->updateStatusBar();
		this->refreshScene();
	} else {
		std::cerr << termcolor::red << "Il n'y a pas de modification plus récente enregistrée." << termcolor::reset << std::endl;
	}
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
	view->rscaleFactor() /= 2;
	view->setTransform(QTransform::fromScale(view->scaleFactor(), view->scaleFactor()));
	view->update();
}

void MainWindow::zoomIn() {
	view->rscaleFactor() *= 2;
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
//	auto start(std::chrono::high_resolution_clock::now());
	game->evolve();
	this->refreshScene();
//	auto stop(std::chrono::high_resolution_clock::now());
//	std::cout << termcolor::green << std::chrono::duration_cast<std::chrono::microseconds>(stop-start).count() << termcolor::reset << " | ";
}

void MainWindow::pauseResume() {
	if (timerId != 0) {
		this->killTimer(timerId);
		timerId = 0;
		pauseResumeAct->setIcon(QIcon(":/images/icons8-play.png"));
	} else {
		timerId = this->startTimer(period);
		historic.erase(historic.begin(), historic.end());
		lastModif = historic.begin();
		this->updateStatusBar();
		pauseResumeAct->setIcon(QIcon(":/images/icons8-pause.png"));
	}
}

void MainWindow::clear() {
	if (lastModif != historic.begin()) {
		historic.erase(historic.begin(), lastModif);
	}
	historic.push_front({false, Motif(game->vivantes())});
	lastModif = historic.begin();
	if (historic.size() > 10) {
		historic.pop_back();
	}
	this->updateStatusBar();
	game->wipe();
	this->refreshScene();
}

void MainWindow::createFrame() {
	auto xaxis(new QGraphicsLineItem(0.0, MAX_LIGNES/2, MAX_COLONNES, MAX_LIGNES/2));
	auto yaxis(new QGraphicsLineItem(MAX_COLONNES/2, 0.0, MAX_COLONNES/2, MAX_LIGNES));
	xaxis->setPen(QPen(Qt::red));
	yaxis->setPen(QPen(Qt::green));
	scene->addItem(xaxis);
	scene->addItem(yaxis);
	
	auto top(new QGraphicsLineItem(0.0, 0.0, MAX_COLONNES, 0.0));
	top->setPen(QPen(Qt::blue));
	scene->addItem(top);
	auto left(new QGraphicsLineItem(0.0, 0.0, 0.0, MAX_LIGNES));
	left->setPen(QPen(Qt::blue));
	scene->addItem(left);
	auto right(new QGraphicsLineItem(MAX_COLONNES, 0.0, MAX_COLONNES, MAX_LIGNES));
	right->setPen(QPen(Qt::blue));
	scene->addItem(right);
	auto bottom(new QGraphicsLineItem(0.0, MAX_LIGNES, MAX_COLONNES, MAX_LIGNES));
	bottom->setPen(QPen(Qt::blue));
	scene->addItem(bottom);
}

void MainWindow::refreshScene() {
	scene->clear();
	this->createFrame();
	for (auto const& c : game->vivantes()) {
		scene->addItem(new Cell((double) c.first, (double) c.second));
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
	mainToolBar->addAction(resetZoomAct);
	this->addToolBar(mainToolBar);
	
}

void MainWindow::modifyCell(const size_t& i, const size_t& j, bool mousePressed) {
	if (game != nullptr) {
		switch (modifyState_) {
			case Adding: {
				bool modified(game->addCell(i,j));
				if (lastModif != historic.begin()) {
					historic.erase(historic.begin(), lastModif);
				}
//				Maintenant lastModif == historic.begin()
				if (modified) {
					if (mousePressed) {
						historic.push_front({true,Motif({{i, j}} )} );
						lastModif = historic.begin();
						if (historic.size() >= 11) {
							historic.pop_back();
						}
					} else {
						historic.front().second.push_back({i,j});
					}
				}
				this->updateStatusBar();
				break;
			}
			case Deleting: {
				bool modified(game->deleteCell(i,j));
				if (lastModif != historic.begin()) {
					historic.erase(historic.begin(), lastModif);
				}
//				Maintenant lastModif == historic.begin()
				if (modified) {
					if (mousePressed) {
						historic.push_front({false,Motif({{i, j}} )} );
						lastModif = historic.begin();
						if (historic.size() >= 11) {
							historic.pop_back();
						}
					} else {
						historic.front().second.push_back({i,j});
					}
				}
				this->updateStatusBar();
				break;
			}
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
			break;
		}
		case 2:{
			this->modifyState_ = Deleting;
			stateBox->setCurrentIndex(2);
			break;
		}
		default:{
			this->modifyState_ = Selecting;
			stateBox->setCurrentIndex(0);
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
		case Qt::Key_L:
		    break;
	}
	if (event->modifiers() & Qt::CTRL) {
		ctrlPressed = true;
	}
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
	QWidget::mousePressEvent(event);
}
void MainWindow::mouseMoveEvent(QMouseEvent* event) {
	QWidget::mouseMoveEvent(event);
}
void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
	QWidget::mouseReleaseEvent(event);
}

void MainWindow::wheelEvent(QWheelEvent* event) {
	QWidget::wheelEvent(event);
	if (!hasTouchEvent && ctrlPressed) {
		std::cerr << termcolor::red << "wheelEvent ";
		int steps(event->angleDelta().y() / 120);
		std::cerr << pow(2, steps) << " " << steps << termcolor::reset << std::endl;
		
		view->rscaleFactor() *= pow(2, steps);
		view->setTransform(QTransform::fromScale(view->scaleFactor(), view->scaleFactor()));
//		view->update();
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
//	auto start(std::chrono::high_resolution_clock::now());
	QWidget::paintEvent(event);
//	auto stop(std::chrono::high_resolution_clock::now());
//	std::cout << termcolor::blue << std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start).count() << termcolor::reset << " | ";
}

void MainWindow::keyReleaseEvent(QKeyEvent* event) {
	QWidget::keyReleaseEvent(event);
	if (ctrlPressed && !(event->modifiers() & Qt::CTRL)) {
		ctrlPressed = false;
	}
}

void MainWindow::resetZoom() {
	view->rscaleFactor() = 1;
	view->setTransform(QTransform());
	view->update();
}

void MainWindow::createStatusBar() {
	labels[0] = new QLabel("Historic size :");
	statusBar()->addWidget(labels[0]);
	labels[1] = new QLabel;
	labels[1]->setNum(0);
	statusBar()->addWidget(labels[1]);
	labels[2] = new QLabel("lastModif :");
	statusBar()->addWidget(labels[2]);
	labels[3] = new QLabel;
	labels[3]->setNum(int(std::distance<std::deque<std::pair<bool,Motif>>::iterator>(historic.begin(), lastModif)));
	statusBar()->addWidget(labels[3]);
	
}

void MainWindow::updateStatusBar() {
	labels[1]->setNum((int)historic.size());
	labels[3]->setNum(int(std::distance<std::deque<std::pair<bool,Motif>>::iterator>(historic.begin(), lastModif)));
}



