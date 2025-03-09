#include "setting_vits.h"
#include "ui_setting_vits.h"

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
