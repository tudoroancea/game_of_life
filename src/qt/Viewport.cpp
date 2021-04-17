//
// Created by Tudor Oancea on 17/04/2021.
//

#include "Viewport.hpp"
#include "Motif.hpp"
#include "GameOfLife.hpp"
#include "MainWindow.hpp"

#include <QWidget>
#include <QMouseEvent>

Viewport::Viewport(MainWindow* parent, GameOfLifeView* game) : mainWindow(parent), game(g) {

}

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

void Viewport::modifyCells(golChange const& toChange) {
	for (auto it(toChange.toAdd.cbegin()); it != toChange.toAdd.cend(); ++it) {
		this->addCell(it->first, it->second);
	}
	for (auto it(toChange.toDelete.cbegin()); it != toChange.toDelete.cend(); ++it) {
		this->deleteCell(it->first, it->second);
	}
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
