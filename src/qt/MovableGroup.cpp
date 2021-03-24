//
// Created by Tudor Oancea on 22/03/2021.
//

#include "MovableGroup.hpp"

#include <QGraphicsRectItem>
#include <QPen>
#include <iostream>

MovableGroup::MovableGroup() : rect(nullptr) {

}

MovableGroup::MovableGroup(const QList<CellItem*>& other) : cells(other), rect(nullptr) {
	if (!other.empty()) {
		qreal iMin(other.front()->x()), iMax(iMin), jMin(other.front()->y()), jMax(jMin);
		for (const auto & cell : other) {
		    if (cell->x() < iMin) iMin = cell->x();
		    if (cell->x() > iMax) iMax = cell->x();
		    if (cell->y() < jMin) jMin = cell->y();
		    if (cell->y() > jMax) jMax = cell->y();
		}
		rect = new QGraphicsRectItem(iMin-1.0, jMin-1.0, iMax-iMin+2.0, jMax-jMin+2.0);
		rect->setPen(QPen(Qt::blue));
	}
}
MovableGroup::MovableGroup(Motif const& motif) {
	if (motif.size() != 0) {
		qreal iMin(motif.cbegin()->first), iMax(iMin), jMin(motif.cbegin()->second), jMax(jMin);
		for (auto it = motif.cbegin(); it != motif.cend(); ++it) {
			if (it->first < iMin) iMin = it->first;
			if (it->first > iMax) iMax = it->first;
			if (it->second < jMin) jMin = it->second;
			if (it->second > jMax) jMax = it->second;
			cells.append(new CellItem(it->first, it->second));
		}
		rect = new QGraphicsRectItem(iMin-1.0, jMin-1.0, iMax-iMin+2.0, jMax-jMin+2.0);
		rect->setPen(QPen(Qt::blue));
	}
}

MovableGroup::~MovableGroup() {
	delete rect;
}

void MovableGroup::moveBy(const int& dx, const int& dy) {
//	for (const auto & cell : cells) {
//	    cell->moveBy(dx, dy);
//	}
//	rect->moveBy(dx, dy);
	QRectF rect2(rect->rect());
	int x(rect2.x()), y(rect2.y()), newX(x+dx), newY(y+dy);
	bool xModified(false), yModified(false);
	if (newX >= 0) {
		xModified = true;
	}
	if (newY >= 0) {
		yModified = true;
	}
	if (xModified && !yModified) {
		rect2.moveLeft(newX);
		for (const auto & cell : cells) {
		    cell->moveByX(dx);
		}
	} else if (yModified && !xModified) {
		rect2.moveTop(newY);
		for (const auto & cell : cells) {
		    cell->moveByY(dy);
		}
	} else if (xModified) { // yModified is always true at this point so we can ignore it
		rect2.moveTo(newX, newY);
		for (const auto & cell : cells) {
		    cell->moveBy(dx,dy);
		}
	}
	rect->setRect(rect2);
//	rect->setRect(size_t((int)rect->rect().x()+dx), size_t((int)rect->rect().y()+dy), rect->rect().width(), rect->rect().height());
}

void MovableGroup::append(CellItem* const& cell) {
	if (rect == nullptr) {
		rect = new QGraphicsRectItem(cell->rect());
	} else {
		QRectF rect2(rect->rect());
		qreal x(cell->rect().x()), y(cell->rect().y());
		bool modified(false);
		if (x < rect2.left()) {
			rect2.setLeft(x);
			modified = true;
		}
		if (y < rect2.top()) {
			rect2.setTop(y);
			modified = true;
		}
		if (x+1.0 > rect2.right()) {
			rect2.setRight(x+1.0);
			modified = true;
		}
		if (y+1.0 > rect2.bottom()) {
			rect2.setBottom(y+1.0);
			modified = true;
		}
		if (modified) {
			rect->setRect(rect2);
		}
	}
	cells.append(cell);
}

QList<CellItem*>& MovableGroup::refCells() {
	return cells;
}

QList<CellItem*>::iterator MovableGroup::begin() {
	return cells.begin();
}
QList<CellItem*>::iterator MovableGroup::end() {
	return cells.end();
}
QList<CellItem*>::const_iterator MovableGroup::cbegin() const {
	return cells.cbegin();
}
QList<CellItem*>::const_iterator MovableGroup::cend() const {
	return cells.cend();
}

QGraphicsRectItem* MovableGroup::rectItem() const {
	return rect;
}
