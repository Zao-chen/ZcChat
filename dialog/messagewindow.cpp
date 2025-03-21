#include "messagewindow.h"
#include "ui_messagewindow.h"

messagewindow::messagewindow(const QString &message,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::messagewindow)
{
    ui->setupUi(this);
    setWindowTitle("Error!");
    setWindowIcon(QIcon(":/img/img/logo.png"));
    ui->label->setText(message);
}

messagewindow::~messagewindow()
{
    delete ui;
}
