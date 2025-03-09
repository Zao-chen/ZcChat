#include "setting_ai.h"
#include "ui_setting_ai.h"

setting_ai::setting_ai(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::setting_ai)
{
    ui->setupUi(this);
}

setting_ai::~setting_ai()
{
    delete ui;
}
