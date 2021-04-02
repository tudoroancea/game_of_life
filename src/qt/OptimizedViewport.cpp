#include "OptimizedViewport.hpp"

OptimizedViewport::OptimizedViewport(QWidget* parent, const Liste* t) : QWidget(parent), to_draw(t)
{
}

void OptimizedViewport::setTransform(QTransform t)
{
	transform.translate(this->width()/2, this->height()/2);
	transform.scale(scaleFactor, scaleFactor);
} 

void OptimizedViewport::mousePressEvent(QMouseEvent* event)
{
	emit ViewportMousePressEvent(event);
	this->update();
}

void OptimizedViewport::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	if (this->isVisible())
    {
        QPainter paint(this);
    	QRect background(0, 0, this->width(), this->height());
	    paint.fillRect(background, Qt::black);  
	    paint.setTransform(transform);      
        if (to_draw != nullptr)
        {
	        for (auto const& a : *to_draw)
	        {
	            QRect rect(a.first*1, a.second*1, 1, 1);
	            paint.fillRect(rect, Qt::white);
	        }
    	}
    }
}

void OptimizedViewport::wheelEvent(QWheelEvent* event)
{
	bool vert_hori(abs(event->angleDelta().y()) > abs(event->angleDelta().x()));
	int change(vert_hori*event->angleDelta().y() + (1-vert_hori)*event->angleDelta().x());
	bool shift_state(event->modifiers() & Qt::SHIFT);
	transform.translate((vert_hori && !shift_state) ? 0 : change/5*(1.0/scaleFactor), (vert_hori && !shift_state) ? change/5*(1.0/scaleFactor) : 0);
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

OptimizedViewport::~OptimizedViewport()
{
}