#include "mainwindow.h"

#include <QApplication>
#include "ElaApplication.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    eApp->init(); //初始化
    QApplication::setQuitOnLastWindowClosed(false);
    return a.exec();
}
