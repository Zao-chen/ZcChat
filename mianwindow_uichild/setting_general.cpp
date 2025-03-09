#include "setting_general.h"
#include "ui_setting_general.h"

setting_general::setting_general(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::setting_general)
{
    ui->setupUi(this);
}

setting_general::~setting_general()
{
    delete ui;
}
