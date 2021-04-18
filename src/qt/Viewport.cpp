//
// Created by Tudor Oancea on 17/04/2021.
//

#include "Viewport.hpp"
#include "Motif.hpp"
#include "GameOfLife.hpp"
#include "MainWindow.hpp"

#include <QWidget>
#include <QMouseEvent>

QWidget* Viewport::getWidget() {
	return static_cast<QWidget*>(this);
}

Viewport::Viewport(MainWindow* parent, GameOfLifeView* game) : mainWindow(parent), game(game) {}

Viewport::~Viewport() {
	delete game;
	delete mainWindow;
}

std::vector<bool> Viewport::addMotif(Motif const& motif) {
	std::vector<bool> result(motif.size(), false);
	for (size_t i(0); i < motif.size(); ++i) {
		if (this->addCell(X(motif[i]), Y(motif[i]))) result[i] = true;
	}
	return result;
}

std::vector<bool> Viewport::deleteMotif(Motif const& motif) {
	std::vector<bool> result(motif.size(), false);
	for (size_t i(0); i < motif.size(); ++i) {
		if (this->deleteCell(X(motif[i]), Y(motif[i]))) result[i] = true;
	}
	return result;
}

void Viewport::mousePressEvent(QMouseEvent* event) {
	QWidget::mousePressEvent(event);
	emit viewportMousePressEvent(event);
}

void Viewport::mouseMoveEvent(QMouseEvent* event) {
	QWidget::mouseMoveEvent(event);
	emit viewportMouseMoveEvent(event);
}

void Viewport::mouseReleaseEvent(QMouseEvent* event) {
	QWidget::mouseReleaseEvent(event);
	emit viewportMouseReleaseEvent(event);
}

void Viewport::mouseDoubleClickEvent(QMouseEvent* event) {
	QWidget::mouseDoubleClickEvent(event);
	emit viewportMouseDoubleClickEvent(event);
}

Motif const& Viewport::getCopiedMotif() const {
	return copiedMotif;
}
