//
// Created by Tudor Oancea on 26/02/2021.
//


#include "MainWindow.hpp"
#include "GraphicsView.hpp"
#include "CellItem.hpp"
#include "termcolor.hpp"
#include "NormalViewport.hpp"
#include "OptimizedViewport.hpp"

#include <QApplication>
#include <QPointF>
#include <QTransform>
#include <QtWidgets>
#include <QEvent>

#include <iterator>
#include <deque>
#include <chrono>
#include <iostream>

using namespace std::literals::chrono_literals;

MainWindow::MainWindow() // NOLINT(cppcoreguidelines-pro-type-member-init)
		: historic(std::deque<std::pair<bool,Motif>>()),
		  lastModif(historic.begin()),
		  game(new GameOfLifeView),
		  viewport(new NormalViewport(this, game))
{
	this->setWindowTitle("Conway's Game of Life Emulator");
	this->resize(600,600);
	this->move(QGuiApplication::screens()[0]->geometry().center() - frameGeometry().center());
	this->setUnifiedTitleAndToolBarOnMac(true);

	labels.fill(nullptr);

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();


	this->setCentralWidget(viewport->getWidget());

	viewport->refreshSelectedZone(newSelectedZoneRect, currentSelectedZonePolygon);
//	this->setSelectionZoneColors();
//	this->createFrame();

	connect(viewport, SIGNAL(viewportMousePressEvent(QMouseEvent*)), this, SLOT(viewportMousePressEvent(QMouseEvent*)));
	connect(viewport, SIGNAL(viewportMouseDoubleClickEvent(QMouseEvent*)), this, SLOT(viewportMouseDoubleClickEvent(QMouseEvent*)));
	connect(viewport, SIGNAL(viewportMouseMoveEvent(QMouseEvent*)), this, SLOT(viewportMouseMoveEvent(QMouseEvent*)));
	connect(viewport, SIGNAL(viewportMouseReleaseEvent(QMouseEvent*)), this, SLOT(viewportMouseReleaseEvent(QMouseEvent*)));

	this->setFocus();
}
MainWindow::~MainWindow() {
	for (const auto & label : labels) {
		delete label;
	}
	for (auto const& menu : menus) {
		delete menu.second;
	}
	delete stateBox;
	delete mainToolBar;
	for (auto const& action : actions) {
		delete action.second;
	}
	delete game;
	delete viewport;
}

//	Utility methods ====================================================================================

void MainWindow::createActions() {
//	File Menu ========================================================================================
//	actions["newSimAct"] = new QAction("New Simulation", this);
//	actions["newSimAct"]->setShortcut(QKeySequence::New);
//	connect(actions["newSimAct"], SIGNAL(triggered()), this, SLOT(newSim()));
//
//	actions["openAct"] = new QAction("Open");
//	actions["openAct"]->setShortcut(QKeySequence::Open);
//	connect(actions["openAct"], &QAction::triggered, this, &MainWindow::open);
//
//	actions["saveMotifAct"] = new QAction("Save Motif", this);
//	actions["saveMotifAct"]->setIcon(QIcon(":/images/icons8-save.png"));
//	connect(actions["saveMotifAct"], &QAction::triggered, this, &MainWindow::saveMotif);
//
//	actions["saveSimAct"] = new QAction("Save Simulation", this);
//	connect(actions["saveSimAct"], &QAction::triggered, this, &MainWindow::saveSim);
	

//	Edit Menu ========================================================================================
	actions["undoAct"] = new QAction(QIcon(":/images/icons8-undo.png"),"Undo", this);
	actions["undoAct"]->setShortcut(QKeySequence::Undo);
	connect(actions["undoAct"], &QAction::triggered, this, &MainWindow::undo);
	
	actions["redoAct"] = new QAction(QIcon(":/images/icons8-redo.png"), "Redo", this);
	actions["redoAct"]->setShortcut(QKeySequence::Redo);
	connect(actions["redoAct"], &QAction::triggered, this, &MainWindow::redo);
	
	actions["copyAct"] = new QAction(QIcon(":/images/icons8-copy.png"), "Copy", this);
	actions["copyAct"]->setShortcut(QKeySequence::Copy);
	actions["copyAct"]->setToolTip("Copy the selected cells");
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
	actions["zoomInAct"]->setAutoRepeat(true);
	connect(actions["zoomInAct"], &QAction::triggered, this, &MainWindow::zoomIn);
	
	actions["zoomOutAct"] = new QAction(QIcon(":/images/icons8-zoom_out.png"),"Zoom Out", this);
	actions["zoomOutAct"]->setShortcut(QKeySequence::ZoomOut);
	actions["zoomOutAct"]->setAutoRepeat(true);
	connect(actions["zoomOutAct"], &QAction::triggered, this, &MainWindow::zoomOut);
	
	actions["resetZoomAct"] = new QAction(QIcon(":/images/icons8-zoom_to_actual_size.png"), "Reset Zoom", this);
	actions["resetZoomAct"]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_0));
	connect(actions["resetZoomAct"], &QAction::triggered, this, &MainWindow::resetZoom);
	
	actions["pauseResumeAct"] = new QAction(QIcon(":/images/icons8-play.png"),"Play", this);
	actions["pauseResumeAct"]->setShortcut(QKeySequence(Qt::Key_Space));
	connect(actions["pauseResumeAct"], &QAction::triggered, this, &MainWindow::pauseResume);
	
//	Help Menu ========================================================================================
//	actions["aboutAct"] = new QAction(QIcon(":/images/icons8-about.png"), "About", this);
//	connect(actions["aboutAct"], &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::createMenus() {
//	menus["fileMenu"] = menuBar()->addMenu("File");
//	menus["fileMenu"]->addAction(actions["newSimAct"]);
//	menus["fileMenu"]->addAction(actions["openAct"]);
//	menus["fileMenu"]->addAction(actions["saveMotifAct"]);
//	menus["fileMenu"]->addAction(actions["saveSimAct"]);
	
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
//	menus["helpMenu"]->addAction(actions["aboutAct"]);
}

void MainWindow::createToolBars() {
	stateBox = new QComboBox;
	stateBox->addItem("Select");
	stateBox->addItem("Add");
	stateBox->addItem("Delete");
	stateBox->setToolTip("How you can modify the grid");
	connect(stateBox, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::setModifyState);
	mainToolBar = new QToolBar;
	mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	mainToolBar->addWidget(stateBox);
	mainToolBar->addSeparator();
	mainToolBar->addAction(actions["pauseResumeAct"]);
	mainToolBar->addAction(actions["clearAct"]);
	mainToolBar->addAction(actions["undoAct"]);
	mainToolBar->addAction(actions["redoAct"]);
	mainToolBar->addSeparator();
	mainToolBar->addAction(actions["zoomInAct"]);
	mainToolBar->addAction(actions["zoomOutAct"]);
	mainToolBar->addAction(actions["resetZoomAct"]);
	this->addToolBar(mainToolBar);
	
}

void MainWindow::createStatusBar() {
#ifdef DEBUG_MODE
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
#endif

#ifdef RELEASE_MODE
	labels[0] = new QLabel("Version");
	statusBar()->addWidget(labels[0]);
	labels[1] = new QLabel(PROJECT_VERSION);
	statusBar()->addWidget(labels[1]);
#endif
	
}

void MainWindow::updateStatusBar() {
#ifdef DEBUG_MODE
	labels[1]->setNum((int)historic.size());
	labels[3]->setNum(int(std::distance<std::deque<std::pair<bool,Motif>>::iterator>(historic.begin(), lastModif)));
#endif
}

void MainWindow::refreshScene(golChange const& toChange) {
	for (auto it(toChange.toAdd.cbegin()); it != toChange.toAdd.cend(); ++it) {
		this->addCell(it->first, it->second);
	}
	for (auto it(toChange.toDelete.cbegin()); it != toChange.toDelete.cend(); ++it) {
		this->deleteCell(it->first, it->second);
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

void MainWindow::refreshSelectionZone() {
//	if (newSelectedZone != nullptr) {
//		scene->removeItem(newSelectedZone);
//		delete newSelectedZone;
//	}
//	if (currentSelectedZone != nullptr) {
//		scene->removeItem(currentSelectedZone);
//		delete currentSelectedZone;
//	}
//	newSelectedZone = new QGraphicsRectItem(newSelectedZoneRect);
//	currentSelectedZone = new QGraphicsPolygonItem(currentSelectedZonePolygon);
//	scene->addItem(newSelectedZone);
//	scene->addItem(currentSelectedZone);
}
void MainWindow::setSelectionZoneColors() {
//	newSelectedZone->setBrush(QColor(0, 102, 255, 133));
//	newSelectedZone->setPen(QColor(0,102,255,255));
//	currentSelectedZone->setBrush(QColor(0, 0, 255, 133));
//	currentSelectedZone->setPen(QColor(0,0,255,255));
}

void MainWindow::addCell(size_t const& i, size_t const& j) {
	Q_UNUSED(i)
	Q_UNUSED(j)
//	if (i < MAX_LIGNES && j < MAX_COLONNES && cells[i][j] == nullptr) {
//		if (!optimizedMode) {
//			cells[i][j] = new CellItem((qreal) i, (qreal) j);
//			scene->addItem(cells[i][j]);
//			scene->update(cells[i][j]->rect());
//		}
//	}
}
void MainWindow::deleteCell(size_t const& i, size_t const& j) {
	Q_UNUSED(i)
	Q_UNUSED(j)
//	if (i < MAX_LIGNES && j < MAX_COLONNES && cells[i][j] != nullptr) {
//		if (!optimizedMode) {
//			delete cells[i][j];
//			cells[i][j] = nullptr;
//		}
//	}
}
//	For actions ==========================================================================================
void MainWindow::placeholder(const char* str) {
	std::cerr << termcolor::cyan << "Action " << str << termcolor::reset << std::endl;
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
//			game->deleteMotif(lastModif->second);
//			for (auto it(lastModif->second.cbegin()); it != lastModif->second.cend(); ++it) {
//				this->deleteCell(it->first, it->second);
//			}
			viewport->deleteMotif(lastModif->second);
		} else {
//			game->addMotif(lastModif->second);
//			for (auto it(lastModif->second.cbegin()); it != lastModif->second.cend(); ++it) {
//				this->addCell(it->first, it->second);
//			}
			viewport->addMotif(lastModif->second);
		}
		++lastModif;
		this->updateStatusBar();
	} else {
		statusBar()->showMessage("Pas de modif plus ancienne", 2000);
	}
}

void MainWindow::redo() {
	if (lastModif != historic.begin()) {
		--lastModif;
		if (lastModif->first) {
//			game->addMotif(lastModif->second);
//			for (auto it(lastModif->second.cbegin()); it != lastModif->second.cend(); ++it) {
//				this->addCell(it->first, it->second);
//			}
			viewport->addMotif(lastModif->second);
		} else {
//			game->deleteMotif(lastModif->second);
//			for (auto it(lastModif->second.cbegin()); it != lastModif->second.cend(); ++it) {
//				this->deleteCell(it->first, it->second);
//			}
			viewport->deleteMotif(lastModif->second);
		}
		this->updateStatusBar();
	} else {
		statusBar()->showMessage("Pas de modif plus récente", 2000);
	}
}

void MainWindow::copy() {
	if (modifyState_ == Selecting) {
		if (!currentSelectedZonePolygon.empty()) {
//			copiedMotif.clear();
//			for (size_t i(0); i < MAX_LIGNES; ++i) {
//				for (size_t j(0); j < MAX_COLONNES; ++j) {
//					if (cells[i][j] != nullptr && currentSelectedZonePolygon.intersects(cells[i][j]->boundingRect())) {
//						copiedMotif.push_back({i,j});
//					}
//				}
//			}
			currentSelectedZonePolygon = QPolygonF();
			newSelectedZoneRect = QRectF();
			viewport->copy();
//			newSelectedZone->setRect(newSelectedZoneRect);
//			currentSelectedZone->setPolygon(currentSelectedZonePolygon);
//			if (copiedMotif.size() == 0) {
//				this->showStatusBarMessage("Sélection trop petite, rien copié", 1500);
//			}
		}
	} else {
		this->showStatusBarMessage("Copy ne marche qu'en mode Selecting", 1500);
	}
}

void MainWindow::cut() {
	if (modifyState_ == Selecting) {
//		viewport->copy();
//		game->deleteMotif(copiedMotif);
//		for (auto it(copiedMotif.cbegin()); it != copiedMotif.cend(); ++it) {
//			this->deleteCell(it->first, it->second);
//		}
		viewport->cut();
		if (lastModif != historic.begin()) {
			historic.erase(historic.begin(), lastModif);
		}
		historic.push_front({false, viewport->getCopiedMotif()});
		lastModif = historic.begin();
		if (historic.size() >= 11) {
			historic.pop_back();
		}
	} else {
		this->showStatusBarMessage("Cut ne marche qu'en mode Selecting", 1500);
	}
}

void MainWindow::paste() {
	if (modifyState_ == Selecting) {
		viewport->paste();
	} else {
		this->showStatusBarMessage("Paste ne marche qu'en mode Selecting", 1500);
	}
}

void MainWindow::zoomOut() {
	viewport->zoom(1.0/1.2);
//	vue->rscaleFactor() /= 1.2;
//	vue->setTransform(QTransform::fromScale(view->scaleFactor(), view->scaleFactor()));
//	vue->update();
}

void MainWindow::zoomIn() {
	viewport->zoom(1.2);
}

void MainWindow::newSim() {
	placeholder("newSim");
}

void MainWindow::open() {
	placeholder("open");
}

void MainWindow::pauseResume() {
	if (timerId != 0) {
//		On fait PAUSE   
		this->killTimer(timerId);
		timerId = 0;
		actions["pauseResumeAct"]->setIcon(QIcon(":/images/icons8-play.png"));
		actions["pauseResumeAct"]->setText("Resume");
	} else {       
//		On fait PLAY
//      On ré-ajuste le timer
		timerId = this->startTimer(period);
		actions["pauseResumeAct"]->setIcon(QIcon(":/images/icons8-pause.png"));
		actions["pauseResumeAct"]->setText("Pause");
//		On supprime ce qui ne doit pas exister pendant que la simulation tourne : historique, groupe de cellules non insérees.
		historic.erase(historic.begin(), historic.end());       
		lastModif = historic.begin();          
		this->updateStatusBar();             
		viewport->insertMovableGroup();
	}
}

void MainWindow::resetZoom() {
	viewport->resetZoom();
}

void MainWindow::clear() {
	if (lastModif != historic.begin()) {
		historic.erase(historic.begin(), lastModif);
	}
	if (!game->vivantes().empty()) {
		historic.push_front({false, Motif(game->vivantes())});
		lastModif = historic.begin();
		if (historic.size() > 10) {
			historic.pop_back();
		}
		this->updateStatusBar();
		viewport->wipe(); // equivalent to viewport->modifyCells(game->wipe());
		this->refreshScene(game->wipe());
	}
}

void MainWindow::showStatusBarMessage(string const& message, int const& timer) {
	statusBar()->showMessage(message.c_str(), timer);
}

// Event handlers =========================================================================

void MainWindow::timerEvent(QTimerEvent* event) {
	Q_UNUSED(event)
//	auto start(std::chrono::high_resolution_clock::now());
	viewport->evolve();
//	auto stop(std::chrono::high_resolution_clock::now());
//	std::cout << termcolor::green << std::chrono::duration_cast<std::chrono::microseconds>(stop-start).count()/game->vivantes().size() << termcolor::reset << " | ";
}


void MainWindow::keyPressEvent(QKeyEvent* event) {
	QWidget::keyPressEvent(event);
	switch (event->key()) {
		case Qt::Key_S:
			this->setModifyState(Selecting);
			break;
		case Qt::Key_A: {
			viewport->insertMovableGroup();
			this->setModifyState(Adding);
			break;
		}
		case Qt::Key_D: {
			viewport->insertMovableGroup();
			this->setModifyState(Deleting);
			break;
		}
#ifdef DEBUG_MODE
		case Qt::Key_L: {
			newSelectedZoneRect = QRectF();
			currentSelectedZonePolygon = QPolygonF();
			viewport->refreshSelectedZone(newSelectedZoneRect, currentSelectedZonePolygon);
			break;
		}
		case Qt::Key_I: {
			for (size_t k(0); k < 20000; ++k) {
				size_t i(QRandomGenerator::global()->bounded(0,MAX_LIGNES)), j(QRandomGenerator::global()->bounded(0,MAX_COLONNES));
				if (game->addCell(i, j)) {
					this->addCell(i,j);
				}
			}
			break;
		}
#endif
		case Qt::Key_Return:
		case Qt::Key_Enter: {
			viewport->insertMovableGroup();
			break;
		}
		case Qt::Key_O: {
//			if (optimizedMode) {
//				vue->hide();
//				view->show();
//				this->takeCentralWidget();
//				this->setCentralWidget(view);
////				vue->raise();
//				for (size_t i(0); i < MAX_LIGNES; ++i) {
//					for (size_t j(0); j < MAX_COLONNES; ++j) {
//						delete cells[i][j];
//						if (game->grille()[i+50][j+50].get_state()) {
//							cells[i][j] = new CellItem((qreal) i,(qreal) j);
//							scene->addItem(cells[i][j]);
//						}
//					}
//				}
//				scene->update(scene->itemsBoundingRect());
//				optimizedMode = false;
//			} else {
//				view->hide();
//				vue->show();
//				this->takeCentralWidget();
//				this->setCentralWidget(vue);
//				optimizedMode = true;
//			}
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
//	if (!hasTouchEvent && ctrlPressed) {
//		int steps(event->angleDelta().y() / 120);
//		if (steps != 0) {
//			std::cerr << termcolor::red << "wheelEvent " << pow(2, steps) << " " << steps << termcolor::reset << std::endl;
//			view->rscaleFactor() *= pow(2, steps);
//			view->setTransform(QTransform::fromScale(view->scaleFactor(), view->scaleFactor()));
//		}
//	}
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


// Public slots ===================================================================================================

void MainWindow::viewportMousePressEvent(QMouseEvent *event) {
	if (game != nullptr) {
		QPointF pos(event->localPos());
		size_t i(pos.x()), j(pos.y());
		switch (modifyState_) {
			case Adding: {
				if (event->button() == Qt::LeftButton) {
					leftButtonPressed = true;
					if (lastModif != historic.begin()) {
						historic.erase(historic.begin(), lastModif);
					}
//					Maintenant lastModif == historic.begin();
					if (viewport->addCell(i,j)) {
						historic.push_front({true, Motif({{i,j}} )} );
						lastModif = historic.begin();
					} else {
						alreadyModified = true;
					}
					this->updateStatusBar();
				}
				break;
			}
			case Deleting: {
				if (event->button() == Qt::LeftButton) {
					leftButtonPressed = true;
					if (lastModif != historic.begin()) {
						historic.erase(historic.begin(), lastModif);
					}
//			        Maintenant lastModif == historic.begin();
					if (viewport->deleteCell(i, j)) {
						historic.push_front({false, Motif({{i,j}} )}  );
						lastModif = historic.begin();
					} else {
						alreadyModified = true;
					}
					this->updateStatusBar();
				}
				break;
			}
			case Selecting: {
				if (event->button() == Qt::LeftButton) {
					if (subState_ == Moving) {
						if (viewport->getMovableGroup()->rectItem()->rect().contains(pos)) {
							leftButtonPressed = true;
						}
					}
				}
				break;
			}
			default:
				break;
		}
		if (historic.size() >= 11) {
			historic.pop_back();
		}
		lastI = pos.x();
		lastJ = pos.y();
		viewport->update();
	} else {
		std::cerr << termcolor::red << "[viewportMousePressEvent() n'a rien fait car il n'y a pas de jeu chargé]" << termcolor::reset;
	}
}

void MainWindow::viewportMouseDoubleClickEvent(QMouseEvent *event) {
	if (game != nullptr) {
		if (modifyState_ == Selecting && subState_ != Moving) {
			doubleLeftButtonPressed = true;
			QPointF pos(event->localPos());
			newSelectedZoneRect.moveTo(pos.x(), pos.y());
			viewport->getNewSelectedZone()->setRect(newSelectedZoneRect);
//			viewport->refreshSelectionZone(newSelectedZoneRect);
	//			viewport->getNewSelectedZone()
//			newSelectedZone->setRect(newSelectedZoneRect);
//			this->refreshSelectionZone();
//			this->setSelectionZoneColors();
		}
	}
}

void MainWindow::viewportMouseMoveEvent(QMouseEvent *event) {
	if (game != nullptr) {
		QPointF pos(event->localPos());
		size_t i(pos.x()), j(pos.y());
		switch (modifyState_) {
			case Adding: {
				if (leftButtonPressed) {
					if (dist(i,lastI) <= 1 && dist(j,lastJ) <= 1){
//			            The cell is next to the last added, we add a simple cell
						if (viewport->addCell(i,j)) {
							if (alreadyModified) {
								historic.push_front({true,Motif({{i,j}})});
								alreadyModified = false;
							} else {
								historic.front().second.push_back({i,j});
							}
						}
					} else if (lastI >=0 && lastJ >= 0) {
//		        		The cell is far from the last added, we add a segment from the latter to the first
						Motif seg(segment(i,j,lastI,lastJ));
						std::vector<bool> ajouts(viewport->addMotif(seg));
						for (size_t k(0); k < ajouts.size(); ++k) {
							if (ajouts[k]) {
								if (alreadyModified) {
									historic.push_front({true,Motif({seg[k]})});
									alreadyModified = false;
								} else {
									historic.front().second.push_back(seg[k]);
								}
							}
						}
					}
					lastModif = historic.begin();
					this->updateStatusBar();
				}
				break;
			}
			case Deleting: {
				if (leftButtonPressed) {
					if (dist(i,lastI) <= 1 && dist(j,lastJ) <= 1){
//			            The cell is next to the last deleted, we delete it
						if (viewport->deleteCell(i,j)) {
							if (alreadyModified) {
								historic.push_front({false,Motif({{i,j}})});
								alreadyModified = false;
							} else {
								historic.front().second.push_back({i,j});
							}
						}
					} else if (lastI >=0 && lastJ >= 0) {
//		        		The cell is far from the last deleted, we delete a segment from the latter to the first
						Motif seg(segment(i,j,lastI,lastJ));
						std::vector<bool> supprimes(viewport->deleteMotif(seg));
						for (size_t k(0); k < supprimes.size(); ++k) {
							if (supprimes[k]) {
								if (alreadyModified) {
									historic.push_front({false,Motif({seg[k]})});
									alreadyModified = false;
								} else {
									historic.front().second.push_back(seg[k]);
								}
							}
						}
					}
					lastModif = historic.begin();
					this->updateStatusBar();
				}
				break;
			}
			case Selecting: {
				if (doubleLeftButtonPressed) {
					newSelectedZoneRect.setWidth(pos.x()-newSelectedZoneRect.x());
					newSelectedZoneRect.setHeight(pos.y()-newSelectedZoneRect.y());
					viewport->getNewSelectedZone()->setRect(newSelectedZoneRect);
				} else if (subState_ == Moving && leftButtonPressed) {
					viewport->moveMovableGroup((int)i-(int)lastI, (int)j-(int)lastJ);
				}
			}
			default:
				break;
		}
		lastI = pos.x();
		lastJ = pos.y();
	}
}

void MainWindow::viewportMouseReleaseEvent(QMouseEvent *event) {
	Q_UNUSED(event)
	if (game != nullptr) {
		switch (modifyState_) {
			case Adding: {
				if (leftButtonPressed) {
					lastI = -1.0;
					lastJ = -1.0;
					leftButtonPressed = false;
				}
				
				break;
			}
			case Deleting: {
				if (leftButtonPressed) {
					lastI = -1.0;
					lastJ = -1.0;
					leftButtonPressed = false;
				}
			}
			case Selecting: {
				if (doubleLeftButtonPressed) {
					doubleLeftButtonPressed = false;
					if (currentSelectedZonePolygon.empty()) {
						currentSelectedZonePolygon = newSelectedZoneRect;
						newSelectedZoneRect = QRectF();
					} else {
						if (currentSelectedZonePolygon.intersects(newSelectedZoneRect)) {
							currentSelectedZonePolygon = currentSelectedZonePolygon.united(newSelectedZoneRect);
							newSelectedZoneRect = QRectF();
						} else {
							newSelectedZoneRect = QRectF();
							this->showStatusBarMessage("La zone de sélection doit être connexe", 1000);
						}
					}
					viewport->refreshSelectedZone(newSelectedZoneRect, currentSelectedZonePolygon);
//					this->refreshSelectionZone();
//					this->setSelectionZoneColors();
				} else if (subState_ == Moving && leftButtonPressed) {
					leftButtonPressed = false;
				}
				break;
			}
			default:
				break;
		}
	}
}

void MainWindow::insertMovableGroup() {
//	if (modifyState_ == Selecting && subState_ == Moving && movableGroup != nullptr) {
//		// On prépare l'historique pour ajouter l'ajout du motif collé
//		if (lastModif != historic.begin()) {
//			historic.erase(historic.begin(), lastModif);
//		}
//		historic.push_front({true, Motif()});
//		lastModif = historic.begin();
//		if (historic.size() >= 11) {
//			historic.pop_back();
//		}
////		On ajoute le motif
//		for (auto it = movableGroup->begin(); it != movableGroup->end(); ++it) {
//			size_t i((*it)->rect().x()), j((*it)->rect().y());
//			if (i < MAX_LIGNES && j < MAX_COLONNES && cells[i][j] == nullptr) {
//				cells[i][j] = *it;
//				game->addCell(i, j);
//				historic.front().second.push_back({i,j});
//			} else {
//				delete *it;
//			}
//		}
//		delete movableGroup;
//		movableGroup = nullptr;
//		subState_ = Nothing;
//	}
}

qreal const& MainWindow::getLastI() const {
	return lastI;
}

qreal const& MainWindow::getLastJ() const {
	return lastJ;
}

