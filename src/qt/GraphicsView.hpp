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
	bool leftButtonPressed = false;
	bool doubleLeftButtonPressed = false;
	QPointF lastPos = QPointF();
public:
	explicit GraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);
	bool viewportEvent(QEvent *event) override;
	
	[[nodiscard]] qreal& rscaleFactor();
	[[nodiscard]] const qreal& scaleFactor() const;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent *event) override;
	
signals:
	void singleLeftClick(size_t const& i, size_t const& j, size_t const& lastI, size_t const& lastJ, bool mousePressed);
	void doubleLeftClick(qreal const& i, qreal const& j);
	void singleRightClick(qreal const& i, qreal const& j);
	void changeSelectionIntention(qreal const& i, qreal const& j);
	void addSelectionIntention();
//	virtual void drawBackground(QPainter *painter, const QRectF &rect = this->rect()) override;
};


#endif //GOLBIS_GRAPHICSVIEW_HPP
