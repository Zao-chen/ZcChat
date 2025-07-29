#include "setting_actor_tachie.h"
#include "ui_setting_actor_tachie.h"

#include "../mainwindow.h"

setting_actor_tachie::setting_actor_tachie(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::setting_actor_tachie)
{
    ui->setupUi(this);
    mainWin = qobject_cast<MainWindow *>(this->parent());
}

setting_actor_tachie::~setting_actor_tachie()
{
    delete ui;
}

void setting_actor_tachie::on_spinBox_tachie_size_valueChanged(int arg1)
{
    mainWin->ChangeSetting_tachieSize(arg1);
}

void setting_actor_tachie::on_pushButton_resettachie_clicked()
{
    mainWin->resetTachie();
}

