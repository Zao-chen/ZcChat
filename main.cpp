#include "mainwindow.h"

#include <QApplication>
#include "ElaApplication.h"
#include <QDir>
#include "log.h"

int main(int argc, char *argv[])
{  
    QApplication a(argc, argv);
    QT_LOG::logInit();
    MainWindow w;
    ElaApplication::getInstance()->init();
    QApplication::setQuitOnLastWindowClosed(false);
    QDir::setCurrent(QCoreApplication::applicationDirPath()); //防止开机自启无法读取配置
    return a.exec();
}
