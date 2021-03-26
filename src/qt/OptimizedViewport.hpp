#ifndef OPTIMIZED_VIEWPORT_H
#define OPTIMIZED_VIEWPORT_H
#include <QWidget>
#include <QPainter>
#include <iostream>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QEvent>
#include <QTransform>

typedef std::vector<std::pair<size_t,size_t>> Liste;

class OptimizedViewport : public QWidget
{
Q_OBJECT
public:
	OptimizedViewport(QWidget* parent = nullptr, Liste const* t = nullptr);
    ~OptimizedViewport();

    void set_draw(Liste const* td) {to_draw = td;}
    qreal& rscaleFactor() {return scaleFactor;}
	void setTransform(QTransform const& t) {transform = t;} 

    void paintEvent(QPaintEvent * event);	
    void mouseMoveEvent(QMouseEvent* event) {emit ViewportMouseMoveEvent(event);}
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event) {emit ViewportMouseReleaseEvent(event);}
	void mouseDoubleClickEvent(QMouseEvent* event) {emit ViewportMouseDoubleClickEvent(event);}
	void touchEvent(QTouchEvent* event) {emit ViewportTouchEvent(event);}   
	void wheelEvent(QWheelEvent* event);
	//void keyPressEvent(QKeyEvent* event);
	//void keyReleaseEvent(QKeyEvent* event);

signals:
	void ViewportMouseMoveEvent(QMouseEvent* event);
	void ViewportMousePressEvent(QMouseEvent* event);
	void ViewportMouseReleaseEvent(QMouseEvent* event);
	void ViewportMouseDoubleClickEvent(QMouseEvent* event);
	void ViewportTouchEvent(QTouchEvent* event);


private:

	Liste const* to_draw;
	QTransform transform;
	qreal scaleFactor = 1.0;
};

#endif