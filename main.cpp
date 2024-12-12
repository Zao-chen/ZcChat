#include "mainwindow.h"

#include <QApplication>
#include "ElaApplication.h"
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    ElaApplication::getInstance()->init();
    QApplication::setQuitOnLastWindowClosed(false);
    QDir::setCurrent(QCoreApplication::applicationDirPath()); //防止开机自启无法读取配置
    return a.exec();
}
