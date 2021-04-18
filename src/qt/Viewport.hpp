//
// Created by Tudor Oancea on 17/04/2021.
//

#ifndef GAME_OF_LIFE_VIEWPORT_HPP
#define GAME_OF_LIFE_VIEWPORT_HPP

#include "MainWindow.hpp"
#include "Motif.hpp"
#include "GameOfLife.hpp"
#include "MovableGroup.hpp"

#include <QWidget>
#include <vector>

QT_BEGIN_NAMESPACE
//	class QWidget;
	class QMouseEvent;
	class QGraphicsRectItem;
	class QPolygonF;
	class QRectF;
QT_END_NAMESPACE

//class MainWindow;
//class MovableGroup;

class Viewport : public QWidget {
Q_OBJECT
protected:
	MainWindow* mainWindow;
	GameOfLifeView* game;
	Motif copiedMotif;
public:
	explicit Viewport(MainWindow* parent = nullptr, GameOfLifeView* game = nullptr);
	~Viewport() override;

	virtual QWidget* getWidget();

//	Cells modifications ========================================
	virtual bool addCell(size_t const& i, size_t const& j) = 0;
	virtual bool deleteCell(size_t const& i, size_t const& j) = 0;
	std::vector<bool> addMotif(Motif const& motif);
	std::vector<bool> deleteMotif(Motif const& motif);
	virtual void modifyCells(golChange const& toChange);
	virtual void wipe() = 0;

//	Selection zone modifications ================================
	virtual void refreshSelectedZone(QRectF const&, QPolygonF const&) = 0;
	[[nodiscard]] virtual QGraphicsRectItem* getNewSelectedZone() const = 0;

//	Movables groups modifications ========================================
	virtual void createMovableGroup(Motif const&) = 0;
	virtual void moveMovableGroup(int const& dx, int const& dy) const = 0;
	[[nodiscard]] virtual MovableGroup* getMovableGroup() const = 0;
	virtual golChange insertMovableGroup() = 0;

//	Copy/paste mmodifications ========================================
	virtual void copy() = 0;
	virtual void cut() = 0;
	virtual void paste() = 0;
	[[nodiscard]] Motif const& getCopiedMotif() const;

//	GOL evolution ========================================
	virtual void evolve() = 0;

//	View modifications : zoom, translations ========================================
	virtual void zoom(qreal const& zoomFactor) = 0;
	virtual void resetZoom() = 0;

//	Events of QWidget ===================================
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;

signals:
	void viewportMouseMoveEvent(QMouseEvent* event);
	void viewportMousePressEvent(QMouseEvent* event);
	void viewportMouseReleaseEvent(QMouseEvent* event);
	void viewportMouseDoubleClickEvent(QMouseEvent* event);
//	virtual void viewportTouchEvent(QTouchEvent* event) = 0;
};

#endif //GAME_OF_LIFE_VIEWPORT_HPP
