#include "setting_ai.h"
#include "ui_setting_ai.h"
#include "../mainwindow.h"

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
    mainWin->ChangeSetting_LLMUrl(arg1);
}
//开启报错返回？
void setting_ai::on_checkBox_feedback_clicked(bool checked)
{
    mainWin->ChangeSetting_LLMFeedback(checked);
}

