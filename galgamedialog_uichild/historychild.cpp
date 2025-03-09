#include "historychild.h"
#include "ui_historychild.h"

historychild::historychild(const QString &name,const QString &msg,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::historychild)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_ShowWithoutActivating); // 显示不抢占焦点
    qInfo() << "子窗口创建：" << msg;
    ui->label_msg->setText(msg);
    ui->label_name->setText(name);
}

historychild::~historychild()
{
    delete ui;
}
