#include "setting_vits.h"
#include "ui_setting_vits.h"
#include "QSettings"

static QScopedPointer<QSettings> settings(new QSettings("Setting.ini", QSettings::IniFormat)); //使用 QScopedPointer来自动管理资源。

setting_vits::setting_vits(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::setting_vits)
{
    ui->setupUi(this);
}

setting_vits::~setting_vits()
{
    delete ui;
}

//开启语音合成？
void setting_vits::on_checkBox_enable_clicked(bool checked)
{
    settings->setValue("/vits/enable",checked);
}
void setting_vits::on_lineEdit_url_textChanged(const QString &arg1)
{
    settings->setValue("/vits/url",arg1);
}
//重载窗口
void setting_vits::refreshUI()
{
    ui->retranslateUi(this);
}
