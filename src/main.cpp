#include "qt/mainwindow.h"
#include "qt/mainwindow_.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //MainWindow_ w;
    w.show();
    return a.exec();
}
