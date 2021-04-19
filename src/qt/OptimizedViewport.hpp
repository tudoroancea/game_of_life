#ifndef OPTIMIZED_VIEWPORT_H
#define OPTIMIZED_VIEWPORT_H

#include "Viewport.hpp"
#include "GameOfLife.hpp"

#include <QPainter>
#include <iostream>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QEvent>
#include <QTransform>

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

class OptimizedViewport : public Viewport {
Q_OBJECT
private:
//	Liste const* to_draw;
	QTransform transform = QTransform();
	qreal scaleFactor = 1.0;

public:
	explicit OptimizedViewport(MainWindow* parent = nullptr, GameOfLifeView* game = nullptr);
    ~OptimizedViewport() override;

//	Cells modifications ========================================

//  Yours to implement
	bool addCell(size_t const& i, size_t const& j) override;
	bool deleteCell(size_t const& i, size_t const& j) override;
	void modifyCells(golChange const& toChange) override;
	void wipe() override;

//	Selection zone modifications ================================
// Both methods should do absolutely nothing
	void setNewSelectionZone(QRectF const& rect) override;
	void setCurrentSelectedZone(QPolygonF const& polygon) override;

//	Movables groups modifications ========================================
// The following 4 methods should do absolutely nothing
	void createMovableGroup(Motif const&) override;
	void moveMovableGroup(int const& dx, int const& dy) const override;
	[[nodiscard]] MovableGroup* getMovableGroup() const override; // should return a nullptr
	[[nodiscard]] golChange insertMovableGroup() override; //should return a default-initialized golChange

//	Copy/paste mmodifications ========================================
//  The following 3 methods should do absolutely nothing
	void copy() override;
	void cut() override;
	void paste() override;

//	GOL evolution ========================================
	void evolve() override;

//	View modifications : zoom, translations ========================================
//  Yours to implement
	void zoom(qreal const& zoomFactor) override;
	void resetZoom() override;

//	From below is your old stuff ============================================================

//    void set_draw(Liste const* td);
//    qreal& rscaleFactor();
//	void setTransform(QTransform t);
//
//    void paintEvent(QPaintEvent * event) override;
//    void mouseMoveEvent(QMouseEvent* event) override;
//    void mousePressEvent(QMouseEvent* event) override;
//    void mouseReleaseEvent(QMouseEvent* event) override;
//	void mouseDoubleClickEvent(QMouseEvent* event) override;
//	void touchEvent(QTouchEvent* event);
//	void wheelEvent(QWheelEvent* event) override;
	//void keyPressEvent(QKeyEvent* event);
	//void keyReleaseEvent(QKeyEvent* event);

//signals:
//	void ViewportMouseMoveEvent(QMouseEvent* event);
//	void ViewportMousePressEvent(QMouseEvent* event);
//	void ViewportMouseReleaseEvent(QMouseEvent* event);
//	void ViewportMouseDoubleClickEvent(QMouseEvent* event);
//	void ViewportTouchEvent(QTouchEvent* event);
};

#endif