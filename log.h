#ifndef LOG_H
#define LOG_H

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <iostream>
#include <QApplication>

namespace QT_LOG
{ //默认文件名为当前时间命名的log文件
    QMutex m_LogMutex;
    void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg /*char *msg*/)
    {
        QString log_info;
        switch (type) {
        case QtDebugMsg:
            log_info = QString("%1 [Debug] %2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), msg);
            break;
        case QtInfoMsg: // 添加对 QtInfoMsg 的处理
            log_info = QString("%1 [Info] %2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), msg);
            break;
        case QtWarningMsg:
            log_info = QString("%1 [Warning] %2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), msg);
            break;
        case QtCriticalMsg:
            log_info = QString("%1 [Critical] %2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), msg);
            break;
        case QtFatalMsg:
            log_info = QString("%1 [Fatal] %2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), msg);
            abort();
        }
        m_LogMutex.lock(); //为了线程安全
        QFile outFile(qApp->applicationDirPath()+"/log.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
        QTextStream ts(&outFile);
        ts << log_info << Qt::endl;
        std::cout << log_info.toLocal8Bit().constData() << std::endl;
        outFile.close();
        m_LogMutex.unlock();
    }
    //默认调试级别为warning及以上才会写入日志文件，默认log文件名为程序启动时间命名的log文件
    void logInit(QString logFile = "")
    {
        QFile file(qApp->applicationDirPath()+"/log.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            file.close();
        } else {
            // 处理错误
            qWarning() << "无法清空文件:" << file.errorString();
        }
        qInstallMessageHandler(customMessageHandler);
    }
};
#endif // LOG_H
