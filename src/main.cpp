#include "qt/MainWindow.hpp"
#include "qt/MainWindow_.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //MainWindow_ w;
    w.show();
    return a.exec();
}
