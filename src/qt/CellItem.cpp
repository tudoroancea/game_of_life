//
// Created by Tudor Oancea on 26/02/2021.
//

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QtCore>
#include <QPainter>
#include <QBrush>

#include <iostream>

#include "CellItem.hpp"


CellItem::CellItem(qreal const& x, qreal const& y, qreal const& size) : QGraphicsRectItem(x, y, size, size) {}

CellItem::CellItem(const size_t& i, const size_t& j) : QGraphicsRectItem((qreal) i, (qreal) j, 1.0, 1.0) {}

[[maybe_unused]] void CellItem::setSize(const qreal& size) {
	this->setRect(this->x(), this->y(), size, size);
}

void CellItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
	Q_UNUSED(option)
	Q_UNUSED(widget)
	if (this->isSelected()) painter->fillRect(this->rect(), QBrush(Qt::blue));
	else painter->fillRect(this->rect(), QBrush(Qt::black));
}

void CellItem::setColor(const QColor& color) {
	color_ = color;
}

bool CellItem::equalsTo(CellItem const& other) const {
	return this->rect() == other.rect();
}


CellItem::~CellItem() = default;
