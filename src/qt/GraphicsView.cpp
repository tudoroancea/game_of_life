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

//	Constructors & Destructors ========================================================================================

GraphicsView::GraphicsView(QGraphicsScene* scene, QWidget* parent) : QGraphicsView(scene, parent) {
	this->setMouseTracking(true);
//	this->setForegroundBrush(QColor(0, 102, 255, 133));
}

//	Getters & Setters ========================================================================================

const qreal& GraphicsView::scaleFactor() const {
	return currentScaleFactor;
}
qreal& GraphicsView::rscaleFactor() {
	return currentScaleFactor;
}

//	Event handlers ========================================================================================

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

void GraphicsView::mousePressEvent(QMouseEvent* event) {
	QGraphicsView::mousePressEvent(event);
	emit sendMousePressEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent* event) {
	QGraphicsView::mouseMoveEvent(event);
	emit sendMouseMoveEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent* event) {
	QGraphicsView::mouseReleaseEvent(event);
	emit sendMouseReleaseEvent(event);
}
void GraphicsView::mouseDoubleClickEvent(QMouseEvent* event) {
	QGraphicsView::mouseDoubleClickEvent(event);
	emit sendMouseDoubleClickEvent(event);
}

void GraphicsView::paintEvent(QPaintEvent* event) {
	QGraphicsView::paintEvent(event);
	auto painter(new QPainter(viewport()));
	painter->setBrush(QBrush(QColor(0, 102, 255, 133)));
	this->drawForeground(painter, QRectF(50.0, 50.0, 50.0, 50.0));
	delete painter;
}


