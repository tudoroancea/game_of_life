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
#include <QGraphicsPathItem>

#include <iterator>
#include <deque>
#include <chrono>
#include <iostream>

using namespace std::literals::chrono_literals;

MainWindow::MainWindow()
		: lastModif(historic.begin()),
		  scene(new QGraphicsScene(0.0, 0.0, MAX_LIGNES, MAX_COLONNES)),
		  view(new GraphicsView(scene, this)),
		  game(new GameOfLifeView),
		  newSelectedZone(new QGraphicsRectItem()),
		  currentSelectedZone(new QGraphicsPolygonItem())
{
	labels.fill(nullptr);
	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
	
	this->setWindowTitle("Conway's Game of Life Emulator");
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
	connect(view, SIGNAL(singleLeftClick(size_t const&, size_t const&, size_t const&, size_t const&, bool)), this, SLOT(singleViewportClick(size_t const&, size_t const&, size_t const&, size_t const&, bool)));
//	connect(view, SIGNAL(singleRightClick(qreal const& i, qreal const& j)), this, SLOT(singleRightClick(qreal const& i, qreal const& j)));
	
//	Selection zone
	connect(view, SIGNAL(addSelectionIntention()), this, SLOT(addSelection()));
	connect(view, SIGNAL(changeSelectionIntention(qreal const&, qreal const&)), this, SLOT(changeSelection(qreal const&, qreal const&)));
	connect(view, SIGNAL(doubleLeftClick(qreal const&, qreal const&)), this, SLOT(beginSelection(qreal const&, qreal const&)));
	this->setSelectionZoneColors();
	
	this->setFocus();
}


void MainWindow::createActions() {
//	File Menu ========================================================================================
	actions["newSimAct"] = new QAction("New Simulation", this);
	actions["newSimAct"]->setShortcut(QKeySequence::New);
	connect(actions["newSimAct"], SIGNAL(triggered()), this, SLOT(newSim()));
	
	actions["openAct"] = new QAction("Open");
	actions["openAct"]->setShortcut(QKeySequence::Open);
	connect(actions["openAct"], &QAction::triggered, this, &MainWindow::open);
	
	actions["saveMotifAct"] = new QAction("Save Motif", this);
	actions["saveMotifAct"]->setIcon(QIcon(":/images/icons8-save.png"));
	connect(actions["saveMotifAct"], &QAction::triggered, this, &MainWindow::saveMotif);
	
	actions["saveSimAct"] = new QAction("Save Simulation", this);
	connect(actions["saveSimAct"], &QAction::triggered, this, &MainWindow::saveSim);
	

//	Edit Menu ========================================================================================
	actions["undoAct"] = new QAction(QIcon(":/images/icons8-undo.png"),"Undo", this);
	actions["undoAct"]->setShortcut(QKeySequence::Undo);
	connect(actions["undoAct"], &QAction::triggered, this, &MainWindow::undo);
	
	actions["redoAct"] = new QAction(QIcon(":/images/icons8-redo.png"), "Redo", this);
	actions["redoAct"]->setShortcut(QKeySequence::Redo);
	connect(actions["redoAct"], &QAction::triggered, this, &MainWindow::redo);
	
	actions["copyAct"] = new QAction(QIcon(":/images/icons8-copy.png"), "Copy", this);
	actions["copyAct"]->setShortcut(QKeySequence::Copy);
	connect(actions["copyAct"], &QAction::triggered, this, &MainWindow::copy);
	
	actions["pasteAct"] = new QAction(QIcon(":/images/icons8-paste.png"), "Paste", this);
	actions["pasteAct"]->setShortcut(QKeySequence::Paste);
	connect(actions["pasteAct"], &QAction::triggered, this, &MainWindow::paste);
	
	actions["cutAct"] = new QAction(QIcon(":/images/icons8-cut.png"), "Cut", this);
	actions["cutAct"]->setShortcut(QKeySequence::Cut);
	connect(actions["cutAct"], &QAction::triggered, this, &MainWindow::cut);
	
	actions["clearAct"] = new QAction(QIcon(":/images/icons8-delete.png"), "Clear", this);
	actions["clearAct"]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_K));
	connect(actions["clearAct"], &QAction::triggered, this, &MainWindow::clear);
	
//	View Menu ========================================================================================
	actions["zoomInAct"] = new QAction(QIcon(":/images/icons8-zoom_in.png"), "Zoom In", this);
	actions["zoomInAct"]->setShortcut(QKeySequence::ZoomIn);
	connect(actions["zoomInAct"], &QAction::triggered, this, &MainWindow::zoomIn);
	
	actions["zoomOutAct"] = new QAction(QIcon(":/images/icons8-zoom_out.png"),"Zoom Out", this);
	actions["zoomOutAct"]->setShortcut(QKeySequence::ZoomOut);
	connect(actions["zoomOutAct"], &QAction::triggered, this, &MainWindow::zoomOut);
	
	actions["resetZoomAct"] = new QAction(QIcon(":/images/icons8-zoom_to_actual_size.png"), "Reset Zoom", this);
	actions["resetZoomAct"]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_0));
	connect(actions["resetZoomAct"], &QAction::triggered, this, &MainWindow::resetZoom);
	
	actions["pauseResumeAct"] = new QAction(QIcon(":/images/icons8-play.png"),"Pause/Resume", this);
	actions["pauseResumeAct"]->setShortcut(QKeySequence(Qt::Key_Space));
	connect(actions["pauseResumeAct"], &QAction::triggered, this, &MainWindow::pauseResume);
	
//	Help Menu ========================================================================================
	actions["aboutAct"] = new QAction(QIcon(":/images/icons8-about.png"), "About", this);
	connect(actions["aboutAct"], &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::createMenus() {
	menus["fileMenu"] = menuBar()->addMenu("File");
	menus["fileMenu"]->addAction(actions["newSimAct"]);
	menus["fileMenu"]->addAction(actions["openAct"]);
	menus["fileMenu"]->addAction(actions["saveMotifAct"]);
	menus["fileMenu"]->addAction(actions["saveSimAct"]);
	
	menus["editMenu"] = menuBar()->addMenu("Edit");
	menus["editMenu"]->addAction(actions["undoAct"]);
	menus["editMenu"]->addAction(actions["redoAct"]);
	menus["editMenu"]->addAction(actions["copyAct"]);
	menus["editMenu"]->addAction(actions["cutAct"]);
	menus["editMenu"]->addAction(actions["pasteAct"]);
	menus["editMenu"]->addAction(actions["clearAct"]);
	
	menus["viewMenu"] = menuBar()->addMenu("View");
	menus["viewMenu"]->addAction(actions["pauseResumeAct"]);
	menus["viewMenu"]->addAction(actions["zoomInAct"]);
	menus["viewMenu"]->addAction(actions["zoomOutAct"]);
	menus["viewMenu"]->addAction(actions["resetZoomAct"]);

	menus["helpMenu"] = menuBar()->addMenu("Help");
	menus["helpMenu"]->addAction(actions["aboutAct"]);
}

MainWindow::~MainWindow() {
	delete scene;
	delete game;
	delete view;
	for (auto const& action : actions) {
		delete action.second;
	}
	for (const auto & label : labels) {
		delete label;
	}
	for (auto const& menu : menus) {
	    delete menu.second;
	}
	delete stateBox;
	delete mainToolBar;
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
		statusBar()->showMessage("Pas de modif plus ancienne", 2000);
	}
}

void MainWindow::redo() {
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
		statusBar()->showMessage("Pas de modif plus récente", 2000);
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
		actions["pauseResumeAct"]->setIcon(QIcon(":/images/icons8-play.png"));
	} else {
		timerId = this->startTimer(period);
		historic.erase(historic.begin(), historic.end());
		lastModif = historic.begin();
		this->updateStatusBar();
		actions["pauseResumeAct"]->setIcon(QIcon(":/images/icons8-pause.png"));
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
	auto xaxis(new QGraphicsLineItem(0.0, MAX_LIGNES/2, MAX_COLONNES, MAX_LIGNES/2)); // NOLINT(bugprone-integer-division)
	auto yaxis(new QGraphicsLineItem(MAX_COLONNES/2, 0.0, MAX_COLONNES/2, MAX_LIGNES)); // NOLINT(bugprone-integer-division)
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
	newSelectedZone = new QGraphicsRectItem(newSelectedZoneRect);
	currentSelectedZone = new QGraphicsPolygonItem(currentSelectedZonePolygon);
	this->setSelectionZoneColors();
	scene->addItem(newSelectedZone);
	scene->addItem(currentSelectedZone);
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
	mainToolBar->addAction(actions["pauseResumeAct"]);
	mainToolBar->addAction(actions["clearAct"]);
	mainToolBar->addAction(actions["undoAct"]);
	mainToolBar->addAction(actions["redoAct"]);
	mainToolBar->addAction(actions["zoomInAct"]);
	mainToolBar->addAction(actions["zoomOutAct"]);
	mainToolBar->addAction(actions["resetZoomAct"]);
	this->addToolBar(mainToolBar);
	
}

void MainWindow::singleViewportClick(size_t const& i, size_t const& j, size_t const& lastI, size_t const& lastJ, bool mousePressed) {
	Q_UNUSED(lastI)
	Q_UNUSED(lastJ)
	if (game != nullptr) {
		switch (modifyState_) {
			case Adding: {
				if (lastModif != historic.begin()) {
					historic.erase(historic.begin(), lastModif);
				}
//				Maintenant lastModif == historic.begin()
				if (dist(i,lastI) > 1 || dist(j,lastJ) > 1) {
//					On sera toujours dans la situation où on a bougé, donc on ne doit pas créer un nouveau motif dans historic
					Motif seg(segment(i,j,lastI,lastJ));
					std::vector<bool> ajouts(game->addMotif(seg));
					for (size_t k(0); k < ajouts.size(); ++k) {
						if (ajouts[k]) {
							historic.front().second.push_back(seg[k]);
							scene->addItem(new Cell((double) X(seg[k]), (double) Y(seg[k])));
						}
					}
				} else {
					bool modified(game->addCell(i,j));
					if (modified) {
						if (mousePressed) {
							historic.push_front({true,Motif({{i, j}} )} );
							scene->addItem(new Cell((double)i, (double)j));
							lastModif = historic.begin();
							if (historic.size() >= 11) {
								historic.pop_back();
							}
						} else {
							historic.front().second.push_back({i,j});
							scene->addItem(new Cell((double)i, (double)j));
						}
					}
				}
//				this->updateStatusBar();
				break;
			}
			case Deleting: {
				if (lastModif != historic.begin()) {
					historic.erase(historic.begin(), lastModif);
				}
//				Maintenant lastModif == historic.begin()
				if (dist(i,lastI) <= 1 && dist(j,lastJ) <= 1) {
					bool modified(game->deleteCell(i,j));
					if (modified) {
						if (mousePressed) {
							historic.push_front({false,Motif({{i, j}} )} );
							lastModif = historic.begin();
							if (historic.size() >= 11) {
								historic.pop_back();
							}
						} else {
							historic.front().second.push_back({i,j});
//							scene->itemAt((double)i, (double)j, QTransform())->setEnabled(false);
						}
					}
				} else {
//					On sera toujours dans la situation où on a bougé, donc on ne doit pas créer un nouveau motif dans historic
					Motif seg(segment(i,j,lastI,lastJ));
					std::vector<bool> ajouts(game->deleteMotif(seg));
					for (size_t k(0); k < ajouts.size(); ++k) {
						if (ajouts[k]) {
							historic.front().second.push_back(seg[k]);
						}
					}
				}
				this->updateStatusBar();
				break;
			}
			case Selecting: {
//				if (!currentSelectedZonePolygon.contains(QPointF(i,j))) {
//				    newSelectedZoneRect = QRectF();
//				    newSelectedZone->setRect(newSelectedZoneRect);
//				    currentSelectedZonePolygon = QPolygonF();
//				    currentSelectedZone->setPolygon(currentSelectedZonePolygon);
//				}
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
		case Qt::Key_L: {
			newSelectedZoneRect = QRectF();
			newSelectedZone->setRect(newSelectedZoneRect);
			currentSelectedZonePolygon = QPolygonF();
			currentSelectedZone->setPolygon(currentSelectedZonePolygon);
		    break;
		}
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
//	view->hide();
	QWidget::paintEvent(event);
//	QPainter painter(view);
//	painter.setBrush(Qt::blue);
//	painter.setPen(Qt::red);
//	painter.drawRect(QRect(0,0,200,200));
//	painter.end();
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

void MainWindow::showStatusBarMessage(const string& message, int const& timer) {
	statusBar()->showMessage(message.c_str(), timer);
}

void MainWindow::beginSelection(const qreal& i, const qreal& j) {
	if (modifyState_ == Selecting) {
		newSelectedZoneRect.moveTo(i,j);
		newSelectedZone->setRect(newSelectedZoneRect);
		this->refreshScene();
	}
}

void MainWindow::changeSelection(const qreal& i, const qreal& j) {
	if (modifyState_ == Selecting) {
		newSelectedZoneRect.setHeight(j-newSelectedZoneRect.y());
		newSelectedZoneRect.setWidth(i-newSelectedZoneRect.x());
		newSelectedZone->setRect(newSelectedZoneRect);
		this->refreshScene();
	}
}

void MainWindow::addSelection() {
	if (modifyState_ == Selecting) {
		if (currentSelectedZonePolygon.empty() || currentSelectedZonePolygon.intersects(newSelectedZoneRect)) {
			currentSelectedZonePolygon = currentSelectedZonePolygon.united(newSelectedZoneRect);
			newSelectedZoneRect = QRectF();
			this->refreshScene();
		} else {
			newSelectedZoneRect =QRectF();
			newSelectedZone->setRect(newSelectedZoneRect);
			this->refreshScene();
			this->showStatusBarMessage("La zone de sélection doit être connexe", 1000);
		}
	}
}

void MainWindow::setSelectionZoneColors() {
	newSelectedZone->setBrush(QColor(0, 102, 255, 133));
	currentSelectedZone->setBrush(QColor(0, 0, 255, 133));
}



