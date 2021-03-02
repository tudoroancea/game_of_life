//
// Created by Tudor Oancea on 26/02/2021.
//

#ifndef GAME_OF_LIFE_MAINWINDOW_HPP
#define GAME_OF_LIFE_MAINWINDOW_HPP

#include <QMainWindow>
//#include <QObject>
//#include <QAction>
//#include <QMenu>
//#include <QToolBar>
//#include <QLabel>
//#include <QActiongroup>
//#include <QFrame>
//#include <QWidget>
#include <unordered_map>
#include <list>

#include "GraphicsView.hpp"
#include "GameOfLife.hpp"
#include "Motif.hpp"
#include "EquivalenceTable.hpp"
#include "Cell.hpp"

QT_BEGIN_NAMESPACE
class QLabel;
class QGraphicsScene;
class QMenu;
class QAction;
class QEvent;
class QToolBar;
class QComboBox;
class QPolygon;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT
private:
	int timerId = 0;
	std::chrono::milliseconds period = 50ms;
//	Etat des évenements
	bool hasTouchEvent = false;
	bool ctrlPressed = false;
	bool mousePressed = false;
//	Mode de modification
	enum CellModifier {Selecting, Adding, Deleting};
	CellModifier modifyState_ = Selecting;
	
//	Zone de séléction
	QPolygon selectedArea;
	QRect newSelectedArea;
	
//	Historiques
	Motif addedMotif = Motif();
	std::list<std::pair<bool,Motif>> historic;
	std::list<std::pair<bool,Motif>>::iterator lastModif;
//	Quand on fait undo on ajoute/supprime le motif désigné par lastModif, puis on l'incrémente. Si lastModif == historic.end()-1, on ne fait rien
//	Quand on fait redo on ajoute/supprime le motif désiné par lastModif, puis on le décremente. SI lastModif == historic.begin(), on ne fait rien
//  Quand on recommence à modifier, on supprime tous les motifs avant lastModif
	
	//	Stockage des infos
	QGraphicsScene* scene;
	GraphicsView* view;
	GameOfLifeView* game;
	QLabel* label = nullptr;

//  Menus
	QMenu* fileMenu;
	QMenu* editMenu;
	QMenu* viewMenu;
	QMenu* helpMenu;
	QComboBox* stateBox;
	QToolBar* mainToolBar;

//	Actions
	QAction* newSimAct;
	QAction* openAct;
	QAction* saveMotifAct;
	QAction* saveSimAct;
	QAction* aboutAct;
	
	QAction* undoAct;
	QAction* redoAct;
	QAction* copyAct;
	QAction* pasteAct;
	QAction* cutAct;
	QAction* clearAct;
	
	QAction* zoomInAct;
	QAction* zoomOutAct;
	QAction* resetZoomAct;
	QAction* pauseResumeAct;


//	utility methods
	void createActions();
	void createMenus();
	void createToolBars();
	static void placeholder(const char* str);
	void createFrame();
	void refreshScene();
	void setModifyState(int const& modifyState);
//	void setModifyState(CellModifier const& modifyState_);

private slots:
	void newSim();
	void open();
	void saveMotif();
	void saveSim();
	void about();
	
	void undo();
	void redo();
	void copy();
	void paste();
	void cut();
	void clear();
	
	void zoomIn();
	void zoomOut();
	void resetZoom();
	void pauseResume();


public:
//	Constructor & Destructors
	MainWindow();
	~MainWindow() override;
	
	//	Events handlers
	void timerEvent(QTimerEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	bool event(QEvent* event) override;
	void paintEvent(QPaintEvent* event) override;

public slots:
	void addCell(size_t const& i, size_t const& j);
	void deleteCell(size_t const& i, size_t const& j);
	void inverseCell(size_t const& i, size_t const& j);
	void setMousePressed();
	void unsetMousePressed(size_t const& i, size_t const& j);
	void modifyCell(size_t const& i, size_t const& j, bool mousePressed);
};


#endif //GAME_OF_LIFE_MAINWINDOW_HPP
