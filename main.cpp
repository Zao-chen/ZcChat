#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include "log.h"
#include "ElaApplication.h"

int main(int argc, char *argv[])
{  
    QApplication a(argc, argv);
    QT_LOG::logInit(); //日志初始化
    MainWindow w;
    ElaApplication::getInstance()->init();
    QApplication::setQuitOnLastWindowClosed(false);
    QDir::setCurrent(QCoreApplication::applicationDirPath()); //防止开机自启无法读取配置
    return a.exec();
}
