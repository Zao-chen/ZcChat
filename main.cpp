#include "mainwindow.h"

#include <QApplication>
#include "ElaApplication.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    ElaApplication::getInstance()->init();
    QApplication::setQuitOnLastWindowClosed(false);
    return a.exec();
}
