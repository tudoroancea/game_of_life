//
// Created by Tudor Oancea on 26/02/2021.
//

#ifndef GAME_OF_LIFE_CELL_HPP
#define GAME_OF_LIFE_CELL_HPP

#include <QGraphicsObject>

QT_BEGIN_NAMESPACE
class QRectF;
class QStyleOptionGraphicsItem;
class QWidget;
class QPainter;
QT_END_NAMESPACE

class [[maybe_unused]] Cell : public QGraphicsObject {
//	Q_OBJECT

private:
	QRectF rect_;

public:
	explicit Cell(qreal const& x, qreal const& y, qreal const& size = 1.0);
	~Cell() override;
	
	[[maybe_unused]] void setSize(qreal const& size);
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
	[[nodiscard]] QRectF boundingRect() const override;
};


#endif //GAME_OF_LIFE_CELL_HPP