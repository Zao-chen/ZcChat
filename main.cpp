#include "mainwindow.h"

#include <QApplication>
#include "ElaApplication.h"
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    ElaApplication::getInstance()->init();
    QApplication::setQuitOnLastWindowClosed(false);
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    return a.exec();
}
