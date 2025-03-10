#include "history.h"
#include "ui_history.h"
#include "historychild.h"
#include <QScrollBar>  // 需要显式包含此头文件
#include <QTimer>

history::history(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::history)
{
    ui->setupUi(this);
    qInfo() << "history 窗口创建...";
    // 设置窗口属性
    /*无边框设置*/
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setWindowOpacity(0.9);
    setAttribute(Qt::WA_TranslucentBackground);
    // 获取 scrollAreaWidgetContents 的布局，如果没有，创建一个布局
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->scrollAreaWidgetContents->layout());
    ui->scrollArea->setWidgetResizable(true);
    connect(ui->scrollArea->verticalScrollBar(), &QScrollBar::rangeChanged, this, [=]() {
        ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->maximum());
    });
}

history::~history()
{
    qInfo() << "history 窗口销毁...";
    delete ui;
}
void history::addChildWindow(QString name,QString msg) {
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->scrollAreaWidgetContents->layout());
    if (!layout) return;

    historychild *newChild = new historychild(name,msg);
    layout->addWidget(newChild);
    // 添加子窗口后，滑动到最底部
    // 等待布局更新后滑动到最底部
    qInfo() << "添加日志：" << msg;

}
