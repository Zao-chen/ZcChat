#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QTranslator>
#include "log.h"
#include "ElaApplication.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //语言切换
    QTranslator translator;
    QString locale = QLocale::system().name(); // 获取系统语言环境
    qInfo()<<"用户使用语言"<<locale;
    // 尝试加载系统语言的翻译文件
    if (!translator.load(":/translations/" + locale))
    {
        // 如果没有找到对应语言的翻译文件，则加载英语（en_US）作为默认翻译
        (void)translator.load(":/translations/en_US.qm");
    }
    a.installTranslator(&translator); // 安装翻译器
    //其他初始化
    QT_LOG::logInit(); //日志初始化
    MainWindow w;
    ElaApplication::getInstance()->init();
    QApplication::setQuitOnLastWindowClosed(false);
    QDir::setCurrent(QCoreApplication::applicationDirPath()); //防止开机自启无法读取配置
    return a.exec();
}
