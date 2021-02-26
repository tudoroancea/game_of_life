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
	qreal currentScaleFactor;
	bool pressed = false;
public:
	explicit GraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);
	bool viewportEvent(QEvent *event) override;
	
	[[nodiscard]] qreal& scaleFactor();
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

signals:
	void modifyCellIntention(size_t i, size_t j);
//	virtual void drawBackground(QPainter *painter, const QRectF &rect = this->rect()) override;
};


#endif //GOLBIS_GRAPHICSVIEW_HPP
