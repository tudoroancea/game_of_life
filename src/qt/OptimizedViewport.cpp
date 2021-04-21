#include "OptimizedViewport.hpp"
#include <QApplication>
#include <QStyle>

OptimizedViewport::OptimizedViewport(MainWindow* parent, GameOfLifeView* game) : Viewport(parent, game)
{
	//this->resize(parent->width(), this->height());
}

OptimizedViewport::~OptimizedViewport() {}

/*void OptimizedViewport::set_draw(Liste const* td) {to_draw = td;}
qreal& OptimizedViewport::rscaleFactor() {return scaleFactor;}

void OptimizedViewport::setTransform(QTransform t) {
	Q_UNUSED(t)
	transform.translate(this->width() / 2, this->height() / 2);
	transform.scale(scaleFactor, scaleFactor);
}

*/
void OptimizedViewport::paintEvent(QPaintEvent* event) {
	Q_UNUSED(event);
	if (this->isVisible()) {
		QPainter paint(this);
		QRect background(0, 0, this->width(), this->height());
		paint.fillRect(background, Qt::white);
		paint.setPen(Qt::blue);
		paint.setTransform(transform);
		x_ = (this->width() - MAX_LIGNES)/2;
		y_ = (this->height() - MAX_COLONNES)/2;		
		paint.drawRect(QRect(x_,y_,MAX_LIGNES,MAX_COLONNES));
		if (game != nullptr) {
			for (auto const& a : game->vivantes()) {
				QRect rect((int) a.first, (int) a.second, 1, 1);
				paint.fillRect(rect, Qt::black);
			}
		}
	}
}

void OptimizedViewport::mousePressEvent(QMouseEvent* event) {
	emit viewportMousePressEvent(event);
}
void OptimizedViewport::mouseMoveEvent(QMouseEvent* event) {
	emit viewportMouseMoveEvent(event);
}
void OptimizedViewport::mouseReleaseEvent(QMouseEvent* event) {
	emit viewportMouseReleaseEvent(event);
}
void OptimizedViewport::mouseDoubleClickEvent(QMouseEvent* event) {
	emit viewportMouseDoubleClickEvent(event);
}
//void OptimizedViewport::touchEvent(QTouchEvent* event) {emit ViewportTouchEvent(event);}

void OptimizedViewport::wheelEvent(QWheelEvent* event) {
	bool vert_hori(abs(event->angleDelta().y()) > abs(event->angleDelta().x()));
	int change(vert_hori * event->angleDelta().y() + (1 - vert_hori) * event->angleDelta().x());
	bool shift_state(event->modifiers() & Qt::SHIFT);
	transform.translate((vert_hori && !shift_state) ? 0 : change / 5 * (1.0 / scaleFactor),
	                    (vert_hori && !shift_state) ? change / 5 * (1.0 / scaleFactor) : 0);
	this->update();
}


/*
void OptimizedViewport::keyPressEvent(QKeyEvent* event)
{
	if (event->modifiers() & Qt::SHIFT)
	{
		std::cout << "here" << std::endl;
		shift_state = true;
	}
}

void OptimizedViewport::keyReleaseEvent(QKeyEvent* event)
{
	std::cout << "called " << std::endl;
	if (event->modifiers() & Qt::SHIFT)
	{
		shift_state = false;
	}
}
*/

// Yours to implement =======================================

bool OptimizedViewport::addCell(const size_t& i, const size_t& j) {
	bool retour(game->addCell(i,j));
	this->update();
	return retour;
}

bool OptimizedViewport::deleteCell(const size_t& i, const size_t& j) {
	Q_UNUSED(i)
	Q_UNUSED(j)
	return false;
}

void OptimizedViewport::modifyCells(const golChange& toChange) {
	Q_UNUSED(toChange)
}

void OptimizedViewport::wipe() {

}

// Garbage ====================================

void OptimizedViewport::setNewSelectionZone(const QRectF& rect) {Q_UNUSED(rect)}

void OptimizedViewport::setCurrentSelectedZone(const QPolygonF& polygon) {Q_UNUSED(polygon)}

void OptimizedViewport::createMovableGroup(const Motif&) {}

void OptimizedViewport::moveMovableGroup(const int& dx, const int& dy) const {Q_UNUSED(dx)Q_UNUSED(dy)}

MovableGroup* OptimizedViewport::getMovableGroup() const {return nullptr;}

golChange OptimizedViewport::insertMovableGroup() {return golChange();}

void OptimizedViewport::copy() {}

void OptimizedViewport::cut() {}

void OptimizedViewport::paste() {}

void OptimizedViewport::evolve() 
{
	game->evolve();
	this->update();
}

// Yours to implement =======================================

void OptimizedViewport::zoom(const qreal& zoomFactor) {
	QPoint pos = this->mapFromGlobal(QCursor::pos());
	//std::cout << pos.x() << " " << pos.y() << " " << zoomFactor << std::endl;
	QRect bounds = transform.mapRect(QRect(x_, y_, MAX_LIGNES, MAX_COLONNES));
	QRect bounds_inter = bounds.intersected(this->geometry());
	if (bounds_inter.contains(pos))
	{
		transform.translate(pos.x(), pos.y());	
		transform.scale(zoomFactor, zoomFactor);			
		transform.translate(-pos.x(), -pos.y());		
	}
	else
	{
		QPoint center(bounds.center());
		transform.translate(center.x(), center.y());
		transform.scale(zoomFactor, zoomFactor);
		transform.translate(-center.x(), -center.y());
	}
	this->update();
}

void OptimizedViewport::resetZoom() {

}
