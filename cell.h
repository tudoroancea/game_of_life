#ifndef CELL_H
#define CELL_H

#include <QWidget>

class cell : public QWidget
{
public:
    cell(int x_, int y_, QWidget* parent = nullptr, bool c = false);
    void change_color();
    void mousePressEvent(QMouseEvent *event) override;
private:
    bool color;
    int pos_x;
    int pos_y;

};

#endif // CELL_H
