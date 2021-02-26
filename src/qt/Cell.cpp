//
// Created by Tudor Oancea on 26/02/2021.
//

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QtCore>
#include <QPainter>
#include <QBrush>

#include <iostream>

#include "Cell.hpp"
#include "termcolor.hpp"


Cell::Cell(qreal const& x, qreal const& y, qreal const& size) : QGraphicsObject(nullptr), rect_(0.0, 0.0, size, size){
	this->setPos(x,y);
}

[[maybe_unused]] void Cell::setSize(const qreal& size) {
	rect_.setHeight(size);
	rect_.setWidth(size);
}

void Cell::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
	Q_UNUSED(option)
	Q_UNUSED(widget)
	painter->fillRect(this->boundingRect(), QBrush(Qt::black));
}

Cell::~Cell() = default;

QRectF Cell::boundingRect() const {
	return rect_;
}

