#include "setting_actor.h"
#include "ui_setting_actor.h"

setting_actor::setting_actor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::setting_actor)
{
    ui->setupUi(this);
}

setting_actor::~setting_actor()
{
    delete ui;
}
