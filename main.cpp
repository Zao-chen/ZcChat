#include "mainwindow.h"

#include <QApplication>
#include "ElaApplication.h"
#include <QDir>
#include "log.h"

int main(int argc, char *argv[])
{
    if (argc > 1)
    {//设置log文件名为可执行文件名，如果程序启动时有参数，则认为参数为调试级别，否则，按默认级别
        QT_LOG::logInit("log.txt");
    }
    else
    {

        QT_LOG::logInit("log.txt");
    }
    QApplication a(argc, argv);
    MainWindow w;
    ElaApplication::getInstance()->init();
    QApplication::setQuitOnLastWindowClosed(false);
    QDir::setCurrent(QCoreApplication::applicationDirPath()); //防止开机自启无法读取配置
    return a.exec();
}
