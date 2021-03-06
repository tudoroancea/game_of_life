//
// Created by Tudor Oancea on 26/02/2021.
//

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QtCore>
#include <QPainter>
#include <QBrush>

#include <iostream>

#include "Cell.hpp"


Cell::Cell(qreal const& x, qreal const& y, qreal const& size) : QGraphicsRectItem(x,y, size, size) {}

[[maybe_unused]] void Cell::setSize(const qreal& size) {
	this->setRect(this->x(), this->y(), size, size);
}

void Cell::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
	Q_UNUSED(option)
	Q_UNUSED(widget)
	painter->fillRect(this->rect(), QBrush(Qt::black));
}

Cell::~Cell() = default;
