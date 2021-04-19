//
// Created by Tudor Oancea on 26/02/2021.
//

#ifndef GAME_OF_LIFE_MAINWINDOW_HPP
#define GAME_OF_LIFE_MAINWINDOW_HPP

#include <QMainWindow>
#include <unordered_map>
#include <list>
#include <deque>
#include <vector>

#include "GraphicsView.hpp"
#include "CellItem.hpp"
#include "GameOfLife.hpp"
#include "Motif.hpp"
#include "EquivalenceTable.hpp"
#include "MovableGroup.hpp"

QT_BEGIN_NAMESPACE
class QLabel;
class QGraphicsScene;
class QMenu;
class QAction;
class QEvent;
class QToolBar;
class QComboBox;
QT_END_NAMESPACE

class Viewport;

class MainWindow : public QMainWindow {
Q_OBJECT
private:
//	Timer ====================================================================================
	/**
	 * @brief To see if a timer is started or not.
	 */
	int timerId = 0;
	std::chrono::milliseconds period = 50ms;

	//	Event/Keys state ====================================================================================
	bool leftButtonPressed = false;
	bool doubleLeftButtonPressed = false;
	bool ctrlPressed = false;
	bool alreadyModified = false;
	qreal lastI = -1.0;
	qreal lastJ = -1.0;

//	Mode de modification ====================================================================================
	enum CellModifier {Selecting, Adding, Deleting};
	CellModifier modifyState_ = Adding;
	enum SubState {Nothing, Moving};
	SubState subState_ = Nothing;
	
//	Historic for undo/redo modifications ====================================================================================
	std::deque<std::pair<bool,Motif>> historic;
	std::deque<std::pair<bool,Motif>>::iterator lastModif;
	
//	Simulation data ====================================================================================
	GameOfLifeView* game;
	Viewport* viewport;

	//	QLabels for diverse purposes ====================================================================================
	std::array<QLabel*,10> labels = std::array<QLabel*,10>();

//	Zone de sélection et copy/paste ====================================================================================
	QRectF newSelectedZoneRect;
	QPolygonF currentSelectedZonePolygon;

//  Menus ====================================================================================
	std::unordered_map<std::string, QMenu*> menus;
	QComboBox* stateBox = nullptr;
	QToolBar* mainToolBar = nullptr;

//	Actions ====================================================================================
	std::unordered_map<std::string, QAction*> actions;
	
//	Utility methods ====================================================================================
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();

	void setModifyState(int const& modifyState);
	void updateStatusBar();

private slots:
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
//	Constructor & Destructors =========================================================================
	MainWindow();
	~MainWindow() override;
	
//	Events handlers =========================================================================
	void timerEvent(QTimerEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

	[[nodiscard]] qreal const& getLastI() const;
	[[nodiscard]] qreal const& getLastJ() const;

public slots:
	void viewportMousePressEvent(QMouseEvent *event);
	void viewportMouseDoubleClickEvent(QMouseEvent *event);
	void viewportMouseMoveEvent(QMouseEvent *event);
	void viewportMouseReleaseEvent(QMouseEvent *event);
};


#endif //GAME_OF_LIFE_MAINWINDOW_HPP
