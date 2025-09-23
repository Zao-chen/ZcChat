#include "setting_actor_tachie.h"
#include "ui_setting_actor_tachie.h"

#include "../mainwindow.h"
#include <QDir>

setting_actor_tachie::setting_actor_tachie(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::setting_actor_tachie)
    , settings(new QSettings(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/Setting.ini", QSettings::IniFormat, this))
    , settings_actor(new QSettings(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/characters_config/" + settings->value("actor/name").toString() + "/config.ini", QSettings::IniFormat))
{
    ui->setupUi(this);
    mainWin = qobject_cast<MainWindow *>(this->parent());
    ui->progressBar->setVisible(false);
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


void setting_actor_tachie::on_pushButton_loadtachie_clicked()
{
    ui->progressBar->setVisible(true);
    QWidget* content = ui->scrollArea_tachieList->widget();
    if (content) {
        // 删除子控件
        QList<QWidget*> children = content->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
        for (QWidget* child : children)
        {
            delete child;
        }
        // 如果你还用了 layout，也清空 layout
        QLayout* layout = content->layout();
        if (layout) {
            QLayoutItem* item;
            while ((item = layout->takeAt(0)) != nullptr) {
                if (QWidget* w = item->widget()) {
                    delete w;
                }
                delete item;
            }
        }
    }

    QSettings * settings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/Setting.ini", QSettings::IniFormat);

    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
             + "/ZcChat/characters/"
             + settings->value("actor/name").toString());

    QStringList nameFilters;
    nameFilters << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp" << "*.gif";
    QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::NoDotAndDotDot);

    QWidget *container = ui->scrollArea_tachieList->widget(); // 获取内部容器
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(container->layout());
    if (!layout) return;

    ui->progressBar->setMaximum(files.size());
    int i=0;
    for (const QString& file : files)
    {
        ui->progressBar->setValue(i);
        qInfo() << "找到立绘:" << file;

        QString filePath = dir.absoluteFilePath(file); // 获取完整路径
        QPixmap img(filePath); // 读取为图片
        if (img.isNull()) {
            qWarning() << "图片加载失败:" << filePath;
            continue;
        }
        auto *setting_actor_tachie_listchild_new = new setting_actor_tachie_listchild(file, img, container);
        layout->addWidget(setting_actor_tachie_listchild_new);
        i++;
    }
    ui->progressBar->setVisible(false);
}

void setting_actor_tachie::on_lineEdit_Default_textChanged(const QString &arg1)
{
    settings_actor->setValue("/tachie/default",arg1);
}

