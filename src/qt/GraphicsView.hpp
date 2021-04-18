//
// Created by Tudor Oancea on 26/02/2021.
//

#ifndef GOLBIS_GRAPHICSVIEW_HPP
#define GOLBIS_GRAPHICSVIEW_HPP

#include <QGraphicsView>
#include <QObject>

class GraphicsView : public QGraphicsView {
Q_OBJECT
private:
	qreal currentScaleFactor = 1.0;
protected:
//	void drawForeground(QPainter* painter, QRectF const& rect) override;
public:
//	Constructors & Destructors ========================================================================================
	explicit GraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);
	
//	Getters & Setters ========================================================================================
	[[nodiscard]] qreal& rscaleFactor();
	[[nodiscard]] const qreal& scaleFactor() const;
	
//	QGraphicsView Event handlers ========================================================================================
	bool viewportEvent(QEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

signals:
	void sendMousePressEvent(QMouseEvent* event);
	void sendMouseDoubleClickEvent(QMouseEvent* event);
	void sendMouseMoveEvent(QMouseEvent* event);
	void sendMouseReleaseEvent(QMouseEvent* event);
};


#endif //GOLBIS_GRAPHICSVIEW_HPP
