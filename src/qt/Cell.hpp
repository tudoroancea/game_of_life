#ifndef CELL__H
#define CELL__H

#include <Qt>
#include <QRect>

class Cell_
{
public:
    Cell_(unsigned int x_, unsigned int y_, unsigned int w, unsigned int h)
        :  state_(false), pos_x(x_), pos_y(y_), width(w), heigth(h) {}
    void change_color() {state_ = 1 - state_;}
    Qt::GlobalColor color() const;
    QRect rect() const {return QRect(pos_x, pos_y, width, heigth);}
    bool state() {return state_;}
private:
    bool state_;
    unsigned int pos_x;
    unsigned int pos_y;
    unsigned int width;
    unsigned int heigth;
};

#endif // CELL__H
