#include "OptimizedViewport.hpp"

OptimizedViewport::OptimizedViewport(QWidget* parent, const Liste* t) : QWidget(parent), to_draw(t)
{
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
    	QRect background(0, 0, this->height(), this->width());
	    paint.fillRect(background, Qt::black);  
	    paint.setTransform(transform);      
        if (to_draw != nullptr)
        {
	        for (auto a : *to_draw)
	        {
	            QRect rect(a.first*1, a.second*1, 1, 1);
	            paint.fillRect(rect, Qt::white);
	        }
    	}
    }
}

OptimizedViewport::~OptimizedViewport()
{
}