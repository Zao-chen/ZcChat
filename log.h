#ifndef LOG_H
#define LOG_H

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <iostream>
#include <QApplication>
#include <QWidget>
#include <QStandardPaths>
#include "utils/createwin.h"

namespace QT_LOG
{
QMutex m_LogMutex;

// 获取当前活动窗口的类名
QString getActiveWindowName() {
    QWidget *activeWindow = QApplication::activeWindow();
    if (activeWindow) {
        return QString("[%1]").arg(activeWindow->metaObject()->className());
    }
    return "[UnknownWindow]";
}

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString log_info;
    QString windowName = getActiveWindowName(); // 获取窗口类名

    switch (type)
    {
        case QtDebugMsg:
            log_info = QString("%1 [Debug] %2 %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), windowName, msg);
            break;
        case QtInfoMsg:
            log_info = QString("%1 [Info] %2 %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), windowName, msg);
            break;
        case QtWarningMsg:
            log_info = QString("%1 [Warning] %2 %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), windowName, msg);
            break;
        case QtCriticalMsg:
            log_info = QString("%1 [Critical] %2 %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), windowName, msg);
            createwin(QString("%1 [Error] %2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), msg));
            break;
        case QtFatalMsg:
            log_info = QString("%1 [Fatal] %2 %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), windowName, msg);
            abort();
    }
    m_LogMutex.lock(); // 线程安全
    QFile outFile(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/log.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream ts(&outFile);
    ts << log_info << Qt::endl;
    std::cout << log_info.toLocal8Bit().constData() << std::endl;
    outFile.close();
    m_LogMutex.unlock();
}

void logInit(QString logFile = "")
{
    QFile file(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/log.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.close();
    } else {
        qWarning() << "无法清空文件:" << file.errorString();
    }
    qInstallMessageHandler(customMessageHandler);
}
};

#endif // LOG_H
