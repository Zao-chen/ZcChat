#include "setting_general.h"
#include "ui_setting_general.h"
#include "../mainwindow.h"
#include <QDir>
#include <QSettings>

setting_general::setting_general(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::setting_general)
{
    ui->setupUi(this);
    mainWin = qobject_cast<MainWindow *>(this->parent());
}

setting_general::~setting_general()
{
    delete ui;
}
//立绘修改
void setting_general::on_comboBox_actor_currentTextChanged(const QString &arg1)
{
    mainWin->ChangeSetting_ActorChoose(arg1);
}
//开机自启修改
void setting_general::on_checkBox_autostart_clicked(bool checked)
{
    mainWin->ChangeSetting_AutoStart(checked);
}
//自动运行脚本设置
void setting_general::on_checkBox_autoopen_clicked(bool checked)
{
    mainWin->ChangeSetting_AutoOpen(checked);
}
//逐字显示设置
void setting_general::on_spinBox_dialogtime_valueChanged(int arg1)
{
    mainWin->ChangeSetting_DialogTime(arg1);
}

