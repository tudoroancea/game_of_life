//
// Created by Tudor Oancea on 26/02/2021.
//

#ifndef GAME_OF_LIFE_CELLITEM_HPP
#define GAME_OF_LIFE_CELLITEM_HPP

#include <QGraphicsRectItem>

QT_BEGIN_NAMESPACE
class QStyleOptionGraphicsItem;
class QWidget;
class QPainter;
class QGraphicsRectItem;
QT_END_NAMESPACE

class [[maybe_unused]] CellItem : public QGraphicsRectItem {
private:
	QColor color_ = Qt::black;
public:
	explicit CellItem(qreal const& x, qreal const& y, qreal const& size = 1.0);
	explicit CellItem(size_t const& i, size_t const& j);
	~CellItem() override;
	
	[[maybe_unused]] void setSize(qreal const& size);
	void setColor(QColor const& color);
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
	bool equalsTo(CellItem const& other) const;
};


#endif //GAME_OF_LIFE_CELLITEM_HPP