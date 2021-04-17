//
// Created by Tudor Oancea on 17/04/2021.
//

#ifndef GAME_OF_LIFE_NORMALVIEWPORT_HPP
#define GAME_OF_LIFE_NORMALVIEWPORT_HPP

//#include "MainWindow.hpp"
#include "Viewport.hpp"
#include "GraphicsView.hpp"

QT_BEGIN_NAMESPACE
	class QGraphicsScene;
QT_END_NAMESPACE


class NormalViewport : public Viewport, public GraphicsView {
private :
	QGraphicsScene *scene;
//	GraphicsView* view;
	std::vector<std::vector<CellItem*>> cells = std::vector<std::vector<CellItem*>>(MAX_LIGNES, std::vector<CellItem*>(MAX_COLONNES, nullptr));

	QGraphicsRectItem* newSelectedZone;
	QGraphicsPolygonItem* currentSelectedZone;

	MovableGroup* movableGroup = nullptr;

public:
	explicit NormalViewport(MainWindow* parent = nullptr, GameOfLifeView* game = nullptr);
	~NormalViewport() override;

//	Cells modifications ========================================
	bool addCell(size_t const& i, size_t const& j) override;
	bool deleteCell(size_t const& i, size_t const& j) override;
	void wipe() override;

//	Selection zone modifications ================================
	void refreshSelectedZone(QRectF const& newSelectedZoneRect, QPolygonF const& currentSelectedZonePolygon) override;
	[[nodiscard]] QGraphicsRectItem* getNewSelectedZone() const override;

//	Movables groups modifications ========================================
	void createMovableGroup(Motif const& motif) override;
	void moveMovableGroup(int const& dx, int const& dy) const override;
	[[nodiscard]] MovableGroup* getMovableGroup() const override;
	golChange insertMovableGroup() override;

//	Copy/paste mmodifications ========================================
	void copy() override;
	void cut() override;
	void paste() override;

//	GOL evolution ========================================
	void evolve() override;

//	View modifications : zoom, translations ========================================
	void zoom(qreal const& zoomFactor) override;
	void resetZoom() override;

//	QGraphicsView event handlers ==============================
	void mousePressEvent(QMouseEvent * event) override;
	void mouseMoveEvent(QMouseEvent * event) override;
	void mouseReleaseEvent(QMouseEvent * event) override;
	void mouseDoubleClickEvent(QMouseEvent * event) override;
};


#endif //GAME_OF_LIFE_NORMALVIEWPORT_HPP
