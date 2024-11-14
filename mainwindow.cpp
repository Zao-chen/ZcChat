#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent)
    : ElaWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //构造Model
    QStandardItemModel* model = new QStandardItemModel(ui->treeView_up);
    model->appendRow(new QStandardItem(QStringLiteral("立绘设置")));
    model->appendRow(new QStandardItem(QStringLiteral("对话框设置")));
    model->appendRow(new QStandardItem(QStringLiteral("模型设置")));
    model->appendRow(new QStandardItem(QStringLiteral("语音设置")));
    ui->treeView_up->setModel(model);
    QModelIndex modelindex = ui->treeView_up->model()->index(0, 0);
    ui->treeView_up->setCurrentIndex(modelindex);
    //窗口设置
    dialog_win = new galgamedialog;
    tachie_win = new tachie;
    tachie_win->show();

    connect(tachie_win, SIGNAL(show_dialogwin_to_main()), this, SLOT(show_dialogwin_from_tachie()));
    connect(dialog_win, SIGNAL(change_tachie_to_tachie(QString)), tachie_win, SLOT(changetachie_from_galdialog(QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkBox_2_clicked(bool checked)
{
    if(checked) tachie_win->show();
    else tachie_win->hide();
}

void MainWindow::show_dialogwin_from_tachie()
{
    qDebug()<<"【接收】立绘 --- 打开对话框 ---> 主窗口";
    if(ui->checkBox_3->isChecked())
    {
        dialog_win->hide();
        ui->checkBox_3->setChecked(false);
    }
    else
    {
        dialog_win->show();
        ui->checkBox_3->setChecked(true);
    }
}

void MainWindow::on_checkBox_3_clicked(bool checked)
{
    if(checked) dialog_win->show();
    else dialog_win->hide();
}


void MainWindow::on_treeView_up_clicked(const QModelIndex &index)
{
    qDebug()<<"主窗口 翻页到"+QString::number(index.row());
    ui->stackedWidget->setCurrentIndex(index.row());
}

