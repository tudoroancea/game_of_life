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


CellItem::CellItem(qreal const& x, qreal const& y, qreal const& size) : QGraphicsRectItem(x, y, size, size), i_(x), j_(y) {}

CellItem::CellItem(const size_t& i, const size_t& j) : QGraphicsRectItem((qreal) i, (qreal) j, 1.0, 1.0), i_(i), j_(j) {}

[[maybe_unused]] void CellItem::setSize(const qreal& size) {
	this->setRect(i_, j_, size, size);
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



void CellItem::moveBy(qreal dx, qreal dy) {
//	QGraphicsItem::moveBy(dx, dy);
	if (i_+dx < 0) {
		i_ = 0;
	} else {
		i_ = size_t(i_+dx);
	}
	if (j_+dy < 0) {
		j_ = 0;
	} else {
		j_ = size_t(j_+dy);
	}
	this->setRect(i_, j_, 1.0, 1.0);
}
size_t const& CellItem::i() const {
	return i_;
}
size_t const& CellItem::j() const {
	return j_;
}


CellItem::~CellItem() = default;
