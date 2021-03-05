//
// Created by Tudor Oancea on 26/02/2021.
//

#include <QEvent>
#include <QTouchEvent>
#include <QGraphicsView>
#include <QWidget>
#include <QLineF>
#include <QTransform>
#include <QGraphicsSceneMouseEvent>

#include <iostream>

#include "GraphicsView.hpp"
#include "termcolor.hpp"
#include "Cell.hpp"

GraphicsView::GraphicsView(QGraphicsScene* scene, QWidget* parent) : QGraphicsView(scene, parent) {
	this->setMouseTracking(true);
}

bool GraphicsView::viewportEvent(QEvent *event) {
	switch (event->type()) {
		case QEvent::TouchEnd:
		case QEvent::TouchBegin:
		case QEvent::TouchUpdate:
		{
//			auto touchEvent(dynamic_cast<QTouchEvent*>(event));
//			QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
//			if (touchPoints.count() == 2) {
//				// determine scale factor
//				const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
//				const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
//				qreal newScaleFactor = QLineF(touchPoint0.pos(), touchPoint1.pos()).length() / QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();
//				if (touchEvent->touchPointStates() & Qt::TouchPointReleased) {
//					// if one of the fingers is released, remember the current scale
//					// factor so that adding another finger later will continue zooming
//					// by adding new scale factor to the existing remembered value.
//					currentScaleFactor *= newScaleFactor;
//					newScaleFactor = 1;
//				}
//				this->setTransform(QTransform::fromScale(currentScaleFactor * newScaleFactor, currentScaleFactor * newScaleFactor));
//				this->update();
//			}
//			return true;
		}
		default:
			break;
	}
	return QGraphicsView::viewportEvent(event);
}

const qreal& GraphicsView::scaleFactor() const {
	return currentScaleFactor;
}
qreal& GraphicsView::rscaleFactor() {
	return currentScaleFactor;
}

void GraphicsView::mousePressEvent(QMouseEvent* event) {
	QGraphicsView::mousePressEvent(event);
	if (event->button() == Qt::LeftButton) {
		leftButtonPressed = true;
		auto res(this->mapToScene(event->pos()));
		lastPos = res;
		emit singleLeftClick(size_t(res.x()), size_t(res.y()), size_t(lastPos.x()), size_t(lastPos.y()), true);
	}
//	} else if (event->button() == Qt::RightButton) {
//		auto res(this->mapToScene(event->pos()));
//		emit singleRightClick(res.x(), res.y());
//	}
}

void GraphicsView::mouseMoveEvent(QMouseEvent* event) {
	QGraphicsView::mouseMoveEvent(event);
	if (leftButtonPressed) {
		auto res(this->mapToScene(event->pos()));
		emit singleLeftClick(size_t(res.x()), size_t(res.y()), size_t(lastPos.x()), size_t(lastPos.y()), false);
		lastPos = res;
	}
	if (doubleLeftButtonPressed) {
		auto res(this->mapToScene(event->pos()));
		emit changeSelectionIntention(res.x(), res.y());
		
	}
}

void GraphicsView::mouseReleaseEvent(QMouseEvent* event) {
	QGraphicsView::mouseReleaseEvent(event);
	if (leftButtonPressed && !(event->buttons() & Qt::LeftButton)) {
		leftButtonPressed = false;
		auto res(this->mapToScene(event->pos()));
		emit singleLeftClick(size_t(res.x()), size_t(res.y()), size_t(lastPos.x()), size_t(lastPos.y()), false);
		lastPos = res;
	}
	if (doubleLeftButtonPressed && !(event->buttons() & Qt::LeftButton)) {
		doubleLeftButtonPressed = false;
		emit addSelectionIntention();
	}
}

void GraphicsView::paintEvent(QPaintEvent* event) {
	QGraphicsView::paintEvent(event);
}

void GraphicsView::mouseDoubleClickEvent(QMouseEvent* event) {
	QGraphicsView::mouseDoubleClickEvent(event);
	if (event->button() == Qt::LeftButton) {
		doubleLeftButtonPressed = true;
		auto res(this->mapToScene(event->pos()));
		emit doubleLeftClick(res.x(), res.y());
	}
}

