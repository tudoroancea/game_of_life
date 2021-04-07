#include "MainWindow.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    static MainWindow w;
    w.show();
    return QApplication::exec();
}
