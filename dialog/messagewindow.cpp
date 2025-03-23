#include "messagewindow.h"
#include "ui_messagewindow.h"

#include <Qclipboard>
#include <QStandardPaths>
#include <QFile>
#include <QDesktopServices>

messagewindow::messagewindow(const QString &message,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::messagewindow)
{
    ui->setupUi(this);
    setWindowTitle("Error!");
    setWindowIcon(QIcon(":/img/img/logo.png"));
    ui->label_msg->setText(message);
    setWindowFlags(Qt::WindowStaysOnTopHint);
}
messagewindow::~messagewindow()
{
    delete ui;
}
/*复制报错*/
void messagewindow::on_pushButton_copy_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->label_msg->text());
}
/*打开日志*/
void messagewindow::on_pushButton_open_clicked()
{
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString logFilePath = documentsPath + "/ZcChat/log.txt";
    QUrl fileUrl = QUrl::fromLocalFile(logFilePath);
    QDesktopServices::openUrl(fileUrl);
}

