#include "setting_actor_tachie_listchild.h"
#include "ui_setting_actor_tachie_listchild.h"

#include <QSettings>
#include <QStandardPaths>


setting_actor_tachie_listchild::setting_actor_tachie_listchild(const QString &name,const QPixmap img,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::setting_actor_tachie_listchild)
{

    ui->setupUi(this);
    ui->label_name->setText(name);
    ui->label_image->setPixmap(img.scaled(
        ui->label_image->size(),             // 缩放到 QLabel 的大小
        Qt::KeepAspectRatio,                 // 保持比例
        Qt::SmoothTransformation));          // 平滑缩放

    QSettings * settings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/Setting.ini", QSettings::IniFormat);
    // 构造 anim.ini 的完整路径
    QString iniPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                      + "/ZcChat/characters/"
                      + settings->value("actor/name").toString()
                      + "/anim.ini";

    // 创建 QSettings 对象读取 INI 文件（注意 format 参数是 IniFormat）
    QSettings animSettings(iniPath, QSettings::IniFormat);
    // 读取 "name/animation" 项
    ui->comboBox_anime->setCurrentIndex(animSettings.value(name.split(".")[0]+"/animation").toInt());
}

setting_actor_tachie_listchild::~setting_actor_tachie_listchild()
{
    delete ui;
}

void setting_actor_tachie_listchild::on_comboBox_anime_currentIndexChanged(int index)
{

    QSettings * settings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/Setting.ini", QSettings::IniFormat);

    // 构造 anim.ini 路径（假设你有成员变量存放角色路径，或者通过参数传入）
    QString animIniPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                          + "/ZcChat/characters/"
                          + settings->value("actor/name").toString()   // 需要有 actorName 成员变量保存当前角色名
                          + "/anim.ini";

    // 写入 anim.ini
    QSettings animSettings(animIniPath, QSettings::IniFormat);
    qDebug()<<ui->label_name->text();
    animSettings.setValue(ui->label_name->text().split(".")[0]+"/animation", index);
    animSettings.endGroup();

    animSettings.sync(); // 确保立即写入磁盘

}


