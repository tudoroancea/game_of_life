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
#include <deque>

#include "GraphicsView.hpp"
#include "Cell.hpp"
#include "GameOfLife.hpp"
#include "Motif.hpp"
#include "EquivalenceTable.hpp"

QT_BEGIN_NAMESPACE
class QLabel;
class QGraphicsScene;
class QMenu;
class QAction;
class QEvent;
class QToolBar;
class QComboBox;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT
private:
//	Timer ====================================================================================
	int timerId = 0;
	std::chrono::milliseconds period = 50ms;
	
//	Event/Keys state ====================================================================================
	bool hasTouchEvent = false;
	bool leftButtonPressed = false;
	bool doubleLeftButtonPressed = false;
	bool ctrlPressed = false;
	qreal lastI = -1.0;
	qreal lastJ = -1.0;

//	Mode de modification ====================================================================================
	enum CellModifier {Selecting, Adding, Deleting};
	CellModifier modifyState_ = Selecting;
	
//	Historic for undo/redo modifications ====================================================================================
	std::deque<std::pair<bool,Motif>> historic;
	std::deque<std::pair<bool,Motif>>::iterator lastModif;
	
//	Simulation data ====================================================================================
	QGraphicsScene* scene;
	GraphicsView* view;
	GameOfLifeView* game;
	std::array<QLabel*,10> labels;
	
//	Zone de sélection et copy/paste ====================================================================================
	QRectF newSelectedZoneRect;
	QGraphicsRectItem* newSelectedZone;
	QPolygonF currentSelectedZonePolygon;
	QGraphicsPolygonItem* currentSelectedZone;
	Motif copiedMotif;

//  Menus ====================================================================================
	std::unordered_map<std::string, QMenu*> menus;
	QComboBox* stateBox;
	QToolBar* mainToolBar;

//	Actions ====================================================================================
	std::unordered_map<std::string, QAction*> actions;
	
//	Utility methods ====================================================================================
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	static void placeholder(const char* str);
	// Create the frame and the axis on the simulation window
	void createFrame();
	void refreshScene();
	void setModifyState(int const& modifyState);
	void updateStatusBar();
	void setSelectionZoneColors();

private slots:
//	For actions ==========================================================================================
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
//	Utility ===========================================================================================
	void showStatusBarMessage(const string& message, int const& timer);

public:
//	Constructor & Destructors =========================================================================
	MainWindow();
	~MainWindow() override;
	
//	Events handlers =========================================================================
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
	void viewportMousePressEvent(QMouseEvent *event);
	void viewportMouseDoubleClickEvent(QMouseEvent *event);
	void viewportMouseMoveEvent(QMouseEvent *event);
	void viewportMouseReleaseEvent(QMouseEvent *event);
};


#endif //GAME_OF_LIFE_MAINWINDOW_HPP
