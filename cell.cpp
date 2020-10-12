#include "cell.h"
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

cell::cell(int x_, int y_, QWidget* parent, bool c)
    : QWidget(parent), color(c), pos_x(x_), pos_y(y_)
{
    this->setStyleSheet("background-color:black;");
    setMouseTracking(true);
}

void cell::change_color()
{
    if (color) {this->setStyleSheet("background-color:black;"); color = false;}
    else {this->setStyleSheet("background-color:white;"); color = true;}
}

void cell::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    change_color();
}
