#include "setting_voiceinput.h"
#include "ui_setting_voiceinput.h"

setting_voiceinput::setting_voiceinput(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::setting_voiceinput)
{
    ui->setupUi(this);
}

setting_voiceinput::~setting_voiceinput()
{
    delete ui;
}
