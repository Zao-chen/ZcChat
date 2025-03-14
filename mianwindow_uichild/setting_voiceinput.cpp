#include "setting_voiceinput.h"
#include "ui_setting_voiceinput.h"
#include "../mainwindow.h"

static QScopedPointer<QSettings> settings(new QSettings("Setting.ini", QSettings::IniFormat)); //使用 QScopedPointer来自动管理资源。

setting_voiceinput::setting_voiceinput(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::setting_voiceinput)
{
    ui->setupUi(this);
    mainWin = qobject_cast<MainWindow *>(this->parent());
}

setting_voiceinput::~setting_voiceinput()
{
    delete ui;
}

//修改使用语音输入
void setting_voiceinput::on_comboBox_api_currentIndexChanged(int index)
{
    ui->stackedWidget_speechInput->setCurrentIndex(index);
    mainWin->ChangeSetting_speechInputAPI(index);
}
//修改输入url
void setting_voiceinput::on_lineEdit_url_textChanged(const QString &arg1)
{
    settings->setValue("/speechInput/url",arg1);
}
//是否开启长按输入
void setting_voiceinput::on_checkBox_enable_clicked(bool checked)
{
    settings->setValue("/speechInput/enable",checked);
    mainWin->init_to_dialog();
}
//api修改
void setting_voiceinput::on_lineEdit_baiduapikey_textChanged(const QString &arg1)
{
    settings->setValue("/speechInput/baidu_apikey",arg1);
}
void setting_voiceinput::on_lineEdit_baidusecretkey_textChanged(const QString &arg1)
{
    settings->setValue("/speechInput/baidu_secretkey",arg1);
}
//其他设置
void setting_voiceinput::on_checkBox_wake_clicked(bool checked)
{
    mainWin->ChangSetting_speechInputWake(checked);
    settings->setValue("/speechInput/wake_enable",checked);
}
void setting_voiceinput::on_checkBox_interrupt_clicked(bool checked)
{
    settings->setValue("/speechInput/interrupt",checked);
}
void setting_voiceinput::on_spinBox_energy_valueChanged(int arg1)
{
    settings->setValue("/speechInput/energy",arg1);
}
void setting_voiceinput::on_spinBox_size_valueChanged(int arg1)
{
    settings->setValue("/speechInput/size",arg1);
}

