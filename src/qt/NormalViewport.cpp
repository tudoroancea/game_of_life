//
// Created by Tudor Oancea on 17/04/2021.
//

#include "NormalViewport.hpp"
#include "Viewport.hpp"
#include "MainWindow.hpp"
#include "CellItem.hpp"
#include "GraphicsView.hpp"

#include <type_traits>
#include <QStatusBar>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsPolygonItem>
#include <QPen>
#include <QMouseEvent>

QWidget* NormalViewport::getWidget() {
	return view;
}

NormalViewport::NormalViewport(MainWindow* parent, GameOfLifeView* game)
: Viewport(parent, game),
  scene(new QGraphicsScene),
  view(new GraphicsView(scene, parent)),
  newSelectedZone(new QGraphicsRectItem(0.0,0.0,0.0,0.0)),
  currentSelectedZone(new QGraphicsPolygonItem(QPolygonF()))
{
	for (const auto & cell : game->vivantes()) {
	    cells[cell.first][cell.second] = new CellItem(cell.first, cell.second);
	    scene->addItem(cells[cell.first][cell.second]);
	}

	view->setBackgroundBrush(QBrush(Qt::white));
	view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	view->setRenderHint(QPainter::Antialiasing);
	view->setCacheMode(QGraphicsView::CacheBackground);
	view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	connect(view, SIGNAL(sendMousePressEvent(QMouseEvent*)), this, SLOT(graphicsViewPressEvent(QMouseEvent*)));
	connect(view, SIGNAL(sendMouseMoveEvent(QMouseEvent*)), this, SLOT(graphicsViewMoveEvent(QMouseEvent*)));
	connect(view, SIGNAL(sendMouseReleaseEvent(QMouseEvent*)), this, SLOT(graphicsViewReleaseEvent(QMouseEvent*)));
	connect(view, SIGNAL(sendMouseDoubleClickEvent(QMouseEvent*)), this, SLOT(graphicsViewDoubleClickEvent(QMouseEvent*)));

//	Ajout axes dans la scene ==========================
	auto xaxis(new QGraphicsLineItem(0.0, MAX_LIGNES/2, MAX_COLONNES, MAX_LIGNES/2)); // NOLINT(bugprone-integer-division)
	auto yaxis(new QGraphicsLineItem(MAX_COLONNES/2, 0.0, MAX_COLONNES/2, MAX_LIGNES)); // NOLINT(bugprone-integer-division)
	xaxis->setPen(QPen(Qt::red));
	yaxis->setPen(QPen(Qt::green));
	scene->addItem(xaxis);
	scene->addItem(yaxis);
//	Ajout cadre dans la scene ==========================
	auto top(new QGraphicsLineItem(0.0, 0.0, MAX_COLONNES, 0.0));
	top->setPen(QPen(Qt::blue));
	scene->addItem(top);
	auto left(new QGraphicsLineItem(0.0, 0.0, 0.0, MAX_LIGNES));
	left->setPen(QPen(Qt::blue));
	scene->addItem(left);
	auto right(new QGraphicsLineItem(MAX_COLONNES, 0.0, MAX_COLONNES, MAX_LIGNES));
	right->setPen(QPen(Qt::blue));
	scene->addItem(right);
	auto bottom(new QGraphicsLineItem(0.0, MAX_LIGNES, MAX_COLONNES, MAX_LIGNES));
	bottom->setPen(QPen(Qt::blue));
	scene->addItem(bottom);
//	Couleurs zones de séléction ======================
	newSelectedZone->setBrush(QColor(0, 102, 255, 133));
	newSelectedZone->setPen(QColor(0,102,255,255));
	currentSelectedZone->setBrush(QColor(0, 0, 255, 133));
	currentSelectedZone->setPen(QColor(0,0,255,255));
}

NormalViewport::~NormalViewport() {
	delete movableGroup;
	delete scene;
}
void NormalViewport::forceAddCell(size_t const& i, size_t const& j) {
	if (i < MAX_LIGNES && j < MAX_COLONNES && cells[i][j] == nullptr) {
		cells[i][j] = new CellItem((qreal) i, (qreal) j);
		scene->addItem(cells[i][j]);
		scene->update(cells[i][j]->rect());
	}
}
void NormalViewport::forceDeleteCell(const size_t& i, const size_t& j) {
	if (i < MAX_LIGNES && j < MAX_COLONNES && cells[i][j] != nullptr) {
		delete cells[i][j];
		cells[i][j] = nullptr;
	}
}
bool NormalViewport::addCell(const size_t& i, const size_t& j) {
		if (game->addCell(i, j)) {
			this->forceAddCell(i,j);
			return true;
		}
	view->update();
	return false;
}

bool NormalViewport::deleteCell(const size_t& i, const size_t& j) {
		if (game->deleteCell(i, j)) {
			this->forceDeleteCell(i,j);
			return true;
		}
	view->update();
	return false;
}

void NormalViewport::wipe() {
	this->deleteMotif(Motif(game->vivantes()));
}

void NormalViewport::modifyCells(const golChange& toChange) {
	for (auto it(toChange.toAdd.cbegin()); it != toChange.toAdd.cend(); ++it) {
		this->forceAddCell(it->first, it->second);
	}
	for (auto it(toChange.toDelete.cbegin()); it != toChange.toDelete.cend(); ++it) {
		this->forceDeleteCell(it->first, it->second);
	}
}

void NormalViewport::setNewSelectionZone(const QRectF& rect) {
//	REMARK : Deleting the item pointer should automatically remove it from the scene
	delete newSelectedZone;
	newSelectedZone = new QGraphicsRectItem(rect);
	newSelectedZone->setBrush(QColor(0, 102, 255, 133));
	newSelectedZone->setPen(QColor(0,102,255,255));
	scene->addItem(newSelectedZone);
}
void NormalViewport::setCurrentSelectedZone(const QPolygonF& polygon) {
//	REMARK : Deleting the item pointer should automatically remove it from the scene
	delete currentSelectedZone;
	currentSelectedZone = new QGraphicsPolygonItem(polygon);
	currentSelectedZone->setBrush(QColor(0, 0, 255, 133));
	currentSelectedZone->setPen(QColor(0,0,255,255));
	scene->addItem(currentSelectedZone);
}

void NormalViewport::createMovableGroup(const Motif& motif) {
	movableGroup = new MovableGroup(motif);
	for (auto it(movableGroup->cbegin()); it != movableGroup->cend(); ++it) {
		scene->addItem(*it);
	}
	scene->addItem(movableGroup->rectItem());
}

void NormalViewport::moveMovableGroup(const int& dx, const int& dy) const {
//	if (movableGroup != nullptr) {
		movableGroup->moveBy(dx, dy);
//	}
	view->update();
}

MovableGroup* NormalViewport::getMovableGroup() const {
	return movableGroup;
}

golChange NormalViewport::insertMovableGroup() {
	Motif added;
	if (movableGroup != nullptr) {
		for (auto it(movableGroup->begin()); it != movableGroup->end(); ++it) {
			size_t i((*it)->rect().x()), j((*it)->rect().y()); // NOLINT(cppcoreguidelines-narrowing-conversions)
			if (i < MAX_LIGNES && j < MAX_COLONNES && cells[i][j] == nullptr) {
				cells[i][j] = *it;
				game->addCell(i, j);
				added.push_back({i,j});
			} else {
				delete *it;
			}
		}
		delete movableGroup;
		movableGroup = nullptr;
	}
	return {added, Motif()};
}

void NormalViewport::copy() {
	if (!currentSelectedZone->polygon().empty()) {
		copiedMotif.clear();
		for (size_t i(0); i < MAX_LIGNES; ++i) {
			for (size_t j(0); j < MAX_COLONNES; ++j) {
				if (cells[i][j] != nullptr && currentSelectedZone->polygon().intersects(cells[i][j]->boundingRect())) {
					copiedMotif.push_back({i,j});
				}
			}
		}
		currentSelectedZone->setPolygon(QPolygonF());
		newSelectedZone->setRect(QRectF());
		if (copiedMotif.empty()) {
			mainWindow->statusBar()->showMessage("Sélection trop petite, rien copié", 1500);
		}
	} else {
		mainWindow->statusBar()->showMessage("Copy ne marche qu'en mode Selecting", 1500);
	}
}

void NormalViewport::cut() {
	this->copy();
	this->deleteMotif(copiedMotif);
}

void NormalViewport::paste() {
	this->insertMovableGroup();
	if (!copiedMotif.empty()) {
		copiedMotif.recalibrate();
		copiedMotif.translate((size_t)mainWindow->getLastI()-(copiedMotif.max_ligne()-copiedMotif.min_ligne())/2, // NOLINT(cppcoreguidelines-narrowing-conversions)
							  (size_t)mainWindow->getLastJ()-(copiedMotif.max_colonne()-copiedMotif.min_colonne())/2); // NOLINT(cppcoreguidelines-narrowing-conversions)
		this->createMovableGroup(copiedMotif);
	} else {
		mainWindow->statusBar()->showMessage("Rien à coller", 1500);
	}
}

void NormalViewport::evolve() {
	this->modifyCells(game->evolve());
}

void NormalViewport::zoom(qreal const& zoomFactor) {
	view->rscaleFactor() *= zoomFactor;
	view->setTransform(QTransform::fromScale(view->scaleFactor(), view->scaleFactor()));
	view->GraphicsView::update();
}
void NormalViewport::resetZoom() {
	view->rscaleFactor() = 1;
	view->setTransform(QTransform());
	view->GraphicsView::update();
}

void NormalViewport::graphicsViewPressEvent(QMouseEvent* event) {
	emit viewportMousePressEvent(event);
}
void NormalViewport::graphicsViewMoveEvent(QMouseEvent* event) {
	emit viewportMouseMoveEvent(event);
}
void NormalViewport::graphicsViewReleaseEvent(QMouseEvent* event) {
	emit viewportMouseReleaseEvent(event);
}
void NormalViewport::graphicsViewDoubleClickEvent(QMouseEvent* event) {
	emit viewportMouseDoubleClickEvent(event);
}
