#include "setting_ai.h"
#include "ui_setting_ai.h"
#include "../mainwindow.h"
#include <QStandardPaths>

static QScopedPointer<QSettings> settings(new QSettings(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/Setting.ini", QSettings::IniFormat)); // 使用 QScopedPointer 来自动管理资源。

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
//重载窗口
void setting_ai::refreshUI()
{
    ui->retranslateUi(this);
}

void setting_ai::on_lineEdit_openai_url_textChanged(const QString &arg1)
{
    settings->setValue("/llm/openai_url",arg1);
}


void setting_ai::on_lineEdit_openai_key_textChanged(const QString &arg1)
{
    settings->setValue("/llm/openai_key",arg1);
}

