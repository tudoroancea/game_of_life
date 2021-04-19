//
// Created by Tudor Oancea on 17/04/2021.
//

#ifndef GAME_OF_LIFE_NORMALVIEWPORT_HPP
#define GAME_OF_LIFE_NORMALVIEWPORT_HPP

#include "Viewport.hpp"
#include "GraphicsView.hpp"
#include "CellItem.hpp"
#include "GameOfLife.hpp"

QT_BEGIN_NAMESPACE
	class QGraphicsScene;
QT_END_NAMESPACE


class NormalViewport : public Viewport {
Q_OBJECT
private :
	QGraphicsScene* scene;
	GraphicsView* view;
	std::vector<std::vector<CellItem*>> cells = std::vector<std::vector<CellItem*>>(MAX_LIGNES, std::vector<CellItem*>(MAX_COLONNES, nullptr));

	QGraphicsRectItem* newSelectedZone;
	QGraphicsPolygonItem* currentSelectedZone;

	MovableGroup* movableGroup = nullptr;

//	Utility methods ================================
	/**
	 * @brief Adds a cell to the Viewport without adding it to the GOL and without verifying its state in the GOL
	 * @param i,j Coordinates of the cell to add
	 */
	void forceAddCell(size_t const& i, size_t const& j);
	/**
	 * @brief Deletes a cell in the Viewport without deleting it in the GOL and without verifying its state in the GOL
	 * @param i,j Coordinates of the cell to delete
	 */
	void forceDeleteCell(size_t const& i, size_t const& j);

public:
	explicit NormalViewport(MainWindow* parent = nullptr, GameOfLifeView* game = nullptr);
	~NormalViewport() override;

	[[nodiscard]] QWidget* getWidget() override;

//	Cells modifications ========================================

	bool addCell(size_t const& i, size_t const& j) override;
	bool deleteCell(size_t const& i, size_t const& j) override;
	void wipe() override;
	void modifyCells(golChange const& toChange) override;

//	Selection zone modifications ================================
	void setNewSelectionZone(QRectF const& rect) override;
	void setCurrentSelectedZone(QPolygonF const& polygon) override;

	//	Movables groups modifications ========================================
	void createMovableGroup(Motif const& motif) override;
	void moveMovableGroup(int const& dx, int const& dy) const override;
	[[nodiscard]] MovableGroup* getMovableGroup() const override;
	[[nodiscard]] golChange insertMovableGroup() override;

//	Copy/paste mmodifications ========================================
	void copy() override;
	void cut() override;
	void paste() override;

//	GOL evolution ========================================
	void evolve() override;

//	View modifications : zoom, translations ========================================
	void zoom(qreal const& zoomFactor) override;
	void resetZoom() override;

public slots:
//	QGraphicsView event handlers ==============================
	void graphicsViewPressEvent(QMouseEvent* event);
	void graphicsViewMoveEvent(QMouseEvent* event);
	void graphicsViewReleaseEvent(QMouseEvent* event);
	void graphicsViewDoubleClickEvent(QMouseEvent* event);
};


#endif //GAME_OF_LIFE_NORMALVIEWPORT_HPP
