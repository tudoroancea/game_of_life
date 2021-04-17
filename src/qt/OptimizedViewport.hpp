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
	explicit OptimizedViewport(QWidget* parent = nullptr, Liste const* t = nullptr);
    ~OptimizedViewport() override;

    void set_draw(Liste const* td);
    qreal& rscaleFactor();
	void setTransform(QTransform t);

    void paintEvent(QPaintEvent * event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void touchEvent(QTouchEvent* event);
	void wheelEvent(QWheelEvent* event) override;
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