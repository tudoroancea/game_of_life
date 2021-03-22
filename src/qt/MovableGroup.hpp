//
// Created by Tudor Oancea on 22/03/2021.
//

#ifndef GAME_OF_LIFE_MOVABLEGROUP_HPP
#define GAME_OF_LIFE_MOVABLEGROUP_HPP

#include "CellItem.hpp"
#include "Motif.hpp"

#include <QGraphicsItem>
#include <QList>

QT_BEGIN_NAMESPACE
class QGraphicsRectItem;
QT_END_NAMESPACE

class MovableGroup {
private:
	QList<CellItem*> cells;
	QGraphicsRectItem* rect;
	
public:
	MovableGroup();
	MovableGroup(QList<CellItem*> const&);
	explicit MovableGroup(Motif const&);
	/**
	 * @brief Only deletes rect because among the cells we have do delete
	 * only some of them (those which lie above already living cells)
	 * and this information is only available in MainWindow
	 */
	~MovableGroup();
	
	void moveBy(qreal const& dx, qreal const& dy);
	void append(CellItem* const& cell);
	QList<CellItem*>& refCells();
	QList<CellItem*>::iterator begin();
	QList<CellItem*>::iterator end();
	[[nodiscard]] QList<CellItem*>::const_iterator cbegin() const;
	[[nodiscard]] QList<CellItem*>::const_iterator cend() const;
	QGraphicsRectItem* boundingRect() const;
};


#endif //GAME_OF_LIFE_MOVABLEGROUP_HPP
