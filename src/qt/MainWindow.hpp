//
// Created by Tudor Oancea on 26/02/2021.
//

#ifndef GOLBIS_MAINWINDOW_HPP
#define GOLBIS_MAINWINDOW_HPP

#include <QMainWindow>
//#include <QObject>
//#include <QAction>
//#include <QMenu>
//#include <QToolBar>
//#include <QLabel>
//#include <QActiongroup>
//#include <QFrame>
//#include <QWidget>

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
	bool hasTouchEvent = false;
	enum CellModifier {Selecting, Adding, Deleting};
	CellModifier modifyState_ = Selecting;
	
	Motif motif;
	QPolygon selectedArea;
	QRect newSelectedArea;
	
	
	QLabel* gridLabel;
	QGraphicsScene* scene;
	GraphicsView* view;
	GameOfLifeView* game;

//  Menus
	QMenu* fileMenu;
	QMenu* editMenu;
	QMenu* viewMenu;
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
	void pauseResume();


public:
//	Constructor & Destructors
	MainWindow();
	~MainWindow() override;
	
	//	Events handlers
	void timerEvent(QTimerEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	bool event(QEvent* event) override;
	void paintEvent(QPaintEvent* event) override;

public slots:
	void addCell(size_t const& i, size_t const& j);
	void deleteCell(size_t const& i, size_t const& j);
	void inverseCell(size_t const& i, size_t const& j);
	void modifyCell(size_t const& i, size_t const& j);
};


#endif //GOLBIS_MAINWINDOW_HPP
