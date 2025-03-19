#include "setting_general.h"
#include "ui_setting_general.h"

#include "../mainwindow.h"
#include <QDir>
#include <QTranslator>

static QScopedPointer<QSettings> settings(new QSettings("Setting.ini", QSettings::IniFormat)); //使用 QScopedPointer来自动管理资源。

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
    settings->setValue("/soft/auto_open",checked);
}
//逐字显示设置
void setting_general::on_spinBox_dialogtime_valueChanged(int arg1)
{
    settings->setValue("/dialog/time",arg1);
}
//修改对话框缩放
void setting_general::on_spinBox_dialogscale_valueChanged(int arg1)
{
    mainWin->ChangeSetting_dialogSize(arg1);
}


void setting_general::on_comboBox_lan_currentIndexChanged(int index)
{
    QTranslator translator;
    QString lan = "en_US";
    switch (index) {
    case 0:
        lan = "zh_CN";
        break;
    case 1:
        lan = "zh_TW";
        break;
    case 2:
        lan = "en_US";
        break;
    case 3:
        lan = "ja_JP";
        break;
    default:
        lan = "en_US";
        break;
    }
    qInfo()<<"切换语言到"<<lan;
    if (translator.load(":/translations/translations/" + lan + ".qm"))
    {
        qInfo() << "切换成功" << lan;
        qApp->installTranslator(&translator);
        ui->retranslateUi(this);
    }
    else
    {
        qWarning() << "加载翻译文件失败：" << lan;
    }
}


