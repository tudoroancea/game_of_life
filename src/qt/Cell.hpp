//
// Created by Tudor Oancea on 26/02/2021.
//

#ifndef GAME_OF_LIFE_CELL_HPP
#define GAME_OF_LIFE_CELL_HPP

#include <QGraphicsRectItem>

QT_BEGIN_NAMESPACE
class QStyleOptionGraphicsItem;
class QWidget;
class QPainter;
class QGraphicsRectItem;
QT_END_NAMESPACE

class [[maybe_unused]] Cell : public QGraphicsRectItem {
public:
	explicit Cell(qreal const& x, qreal const& y, qreal const& size = 1.0);
	~Cell() override;
	
	[[maybe_unused]] void setSize(qreal const& size);
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};


#endif //GAME_OF_LIFE_CELL_HPP