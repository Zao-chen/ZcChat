#include "setting_ai.h"
#include "ui_setting_ai.h"
#include "../mainwindow.h"

static QScopedPointer<QSettings> settings(new QSettings("Setting.ini", QSettings::IniFormat)); //使用 QScopedPointer来自动管理资源。

setting_ai::setting_ai(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::setting_ai)
{
    ui->setupUi(this);
    mainWin = qobject_cast<MainWindow *>(this->parent());
}

setting_ai::~setting_ai()
{
    delete ui;
}

//修改地址
void setting_ai::on_lineEdit_url_textChanged(const QString &arg1)
{
    settings->setValue("/llm/url",arg1);
}
//开启报错返回？
void setting_ai::on_checkBox_feedback_clicked(bool checked)
{
    settings->setValue("/llm/feedback",checked);
}

