//
// Created by Tudor Oancea on 26/02/2021.
//

#ifndef GAME_OF_LIFE_CELLITEM_HPP
#define GAME_OF_LIFE_CELLITEM_HPP

#include <QGraphicsRectItem>
#include <Motif.hpp>

QT_BEGIN_NAMESPACE
class QStyleOptionGraphicsItem;
class QWidget;
class QPainter;
class QGraphicsRectItem;
QT_END_NAMESPACE

class [[maybe_unused]] CellItem : public QGraphicsRectItem {
private:
	QColor color_ = Qt::black;
	/**
	 * @brief coordonnées de la cellule. Nécessaires pour s'assurer que l'item a bien toujours des coordonnées positives
	 */
	size_t i_, j_;
public:
	explicit CellItem(qreal const& x, qreal const& y, qreal const& size = 1.0);
	explicit CellItem(size_t const& i, size_t const& j);
	~CellItem() override;
	
	[[maybe_unused]] void setSize(qreal const& size);
	void setColor(QColor const& color);
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
	[[nodiscard]] bool equalsTo(CellItem const& other) const;
	[[nodiscard]] size_t const& i() const;
	[[nodiscard]] size_t const& j() const;
	void moveBy(const int& dx, const int& dy);
	void moveByX(int const& dx);
	void moveByY(int const& dy);
};


#endif //GAME_OF_LIFE_CELLITEM_HPP