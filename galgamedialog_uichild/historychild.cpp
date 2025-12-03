#include "historychild.h"
#include "ui_historychild.h"
#include "../galgamedialog.h"

historychild::historychild(const QString &name,const QString &msg,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::historychild)
{
    ui->setupUi(this);
    msg_l=msg;
    galWin = qobject_cast<galgamedialog *>(this->parent()->parent());
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_ShowWithoutActivating); //显示不抢占焦点
    qInfo() << "子窗口创建：" << msg;
    /*获取信息*/
    if(msg.split("|").size()==3)
    {
        ui->label_msg->setText(msg.split("|")[1]);
    }
    else
    {
        ui->label_msg->setText(msg);
        ui->pushButton_reSpawnVoice->hide();
    }
    ui->label_name->setText(name);
    //如果开了语音，就允许重放
    QSettings * settings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat" + "/Setting.ini", QSettings::IniFormat);
    if (!settings->value("vits/enable").toBool())
    {
        ui->pushButton_reSpawnVoice->hide();
    }
}
historychild::~historychild()
{
    delete ui;
}
/*重播消息*/
void historychild::on_pushButton_reSpawnVoice_clicked(bool checked)
{
    qInfo()<<"重播消息"<<msg_l;
    galWin->spawnVoice(msg_l,true);
}

