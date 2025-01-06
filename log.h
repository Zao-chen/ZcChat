#ifndef LOG_H
#define LOG_H

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <iostream>

//默认调试级别为warning，即小于warning级别的都不会写入日志文件
//只有release版本的时候，才会输出到日志，debug版本正常输出到终端。
namespace QT_LOG
{//默认文件名为当前时间命名的log文件
static int m_LogLevel = 1;
static QString m_LogFile = QString("%1.log").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
QMutex m_LogMutex;

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg /*char *msg*/)
{
    if (type < m_LogLevel)//设置输出日志级别，小于该级别，将不会写入日志文件，默认是warning级别，即debug信息不会写入日志文件
    {
        return;
    }

    QString log_info;
    switch (type)
    {
    case QtDebugMsg:
        log_info = QString("%1:%2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),msg);
        break;

    case QtWarningMsg:
        log_info = QString("%1[Warning]:%2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),msg);
        break;

    case QtCriticalMsg:
        log_info = QString("%1[Critical]:%2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),msg);
        break;

    case QtFatalMsg:
        log_info = QString("%1[Fatal]:%2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),msg);
        abort();
    }
    //为了线程安全
    m_LogMutex.lock();

    QFile outFile(m_LogFile);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream ts(&outFile);
    ts << log_info << Qt::endl;
    std::cout << log_info.toLocal8Bit().constData() << std::endl;
    outFile.close();

    m_LogMutex.unlock();
}
//默认调试级别为warning及以上才会写入日志文件，默认log文件名为程序启动时间命名的log文件
void logInit(QString logFile = "",int logLevel = 0)
{
    //#ifndef _DEBUG  //实现debug版本的时候，输出到终端；release版本的时候输出到日志文件
    if ((logLevel < 0) || (logLevel > 3))
    {
        m_LogLevel = 1;
    }
    else
    {
        m_LogLevel = logLevel;
    }

    if (!logFile.isEmpty())
    {
        m_LogFile = logFile;
    }

    qInstallMessageHandler(customMessageHandler);
    //#endif
}
};


#endif // LOG_H
