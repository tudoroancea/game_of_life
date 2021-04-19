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
	class QMouseEvent;
	class QGraphicsRectItem;
	class QPolygonF;
	class QRectF;
QT_END_NAMESPACE


class Viewport : public QWidget {
Q_OBJECT
protected:
	/**
	 * @brief Parent window which contains the Viewport
	 */
	MainWindow* mainWindow;
	/**
	 * @brief pointer on GOL which contains the simulation behind the curtains
	 */
	GameOfLifeView* game;
	/**
	 * @brief Contains the Motif we copied and we are going to paste (one or more times)
	 */
	Motif copiedMotif;

public:
	explicit Viewport(MainWindow* parent = nullptr, GameOfLifeView* game = nullptr);
	~Viewport() override;

	/**
	 * @return the widget itself in the case of an OptimizedVieport, and the GraphicsView attribute in the case of a NormalVieport
	 */
	virtual QWidget* getWidget();

//	Cells modifications ========================================
	/**
	 * @brief Adds a cell to the GOL and if the cell has indeed been added will also add it to the Viewport
	 * @param i,j Coordinates of the cell (in [0,MAX_LIGNES)x[0,MAX_COLONNES) )
	 * @return true if the cell was added, false if not (eg because the cell already existed before)
	 */
	virtual bool addCell(size_t const& i, size_t const& j) = 0;
	/**
	 * @brief Deletes a cell in the GOL and if the cell has indeed been deleted will also delete it in the Viewport
	 * @param i,j Coordinates of the cell (in [0,MAX_LIGNES)x[0,MAX_COLONNES) )
	 * @return true if the cell was deleted, false if not (eg because the cell was already dead)
	 */
	virtual bool deleteCell(size_t const& i, size_t const& j) = 0;
	/**
	 * @brief Adds a Motif to the GOL and then adds the cells to the Viewport. (uses addCell() )
	 * @param motif Motif to add
	 * @return List of bools indicating which cells have indeed been added.
	 */
	std::vector<bool> addMotif(Motif const& motif);
	/**
	 * @brief Deletes a Motif in the GOL and then deletes the cells in the Viewport. (uses deleteCell() )
	 * @param motif Motif to delete
	 * @return List of bools indicating which cells have indeed been deleted.
	 */
	std::vector<bool> deleteMotif(Motif const& motif);
	/**
	 * @brief Change the Cells in the Viewport without changing the GOL
	 * @param toChange
	 */
	virtual void modifyCells(golChange const& toChange) = 0;
	/**
	 * @brief Deletes all the cells in the GOL and in the Viewport
	 */
	virtual void wipe() = 0;

//	Selection zone modifications ================================
	virtual void setNewSelectionZone(QRectF const& rect) = 0;
	virtual void setCurrentSelectedZone(QPolygonF const& polygon) = 0;

	//	Movables groups modifications ========================================
	virtual void createMovableGroup(Motif const&) = 0;
	virtual void moveMovableGroup(int const& dx, int const& dy) const = 0;
	[[nodiscard]] virtual MovableGroup* getMovableGroup() const = 0;
	[[nodiscard]] virtual golChange insertMovableGroup() = 0;

//	Copy/paste mmodifications ========================================
	virtual void copy() = 0;
	virtual void cut() = 0;
	virtual void paste() = 0;
	[[nodiscard]] Motif const& getCopiedMotif() const;

//	GOL evolution ========================================
	/**
	 * @brief Makes the GOL evolve and the Viewport update
	 */
	virtual void evolve() = 0;

//	View modifications : zoom, translations ========================================
	/**
	 * @brief Zoom the Viewport by the factor zoomFactor.
	 * If zoomFactor > 1 it zooms in, otherwise it zooms out.
	 * @param zoomFactor
	 */
	virtual void zoom(qreal const& zoomFactor) = 0;
	/**
	 * @brief Sets the zoom level to the original (1.0)
	 */
	virtual void resetZoom() = 0;

signals:
	void viewportMouseMoveEvent(QMouseEvent* event);
	void viewportMousePressEvent(QMouseEvent* event);
	void viewportMouseReleaseEvent(QMouseEvent* event);
	void viewportMouseDoubleClickEvent(QMouseEvent* event);
};

#endif //GAME_OF_LIFE_VIEWPORT_HPP
