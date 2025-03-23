#include "history.h"
#include "ui_history.h"
#include "historychild.h"
#include <QScrollBar>  // 需要显式包含此头文件
#include <QTimer>
#include <QPainterPath>
#include <QPainter>

history::history(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::history)
{
    ui->setupUi(this);
    qInfo() << "history 窗口创建...";
    //设置窗口属性
    /*无边框设置*/
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setWindowOpacity(0.9);
    setAttribute(Qt::WA_TranslucentBackground);
    // 获取 scrollAreaWidgetContents 的布局，如果没有，创建一个布局
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->scrollAreaWidgetContents->layout());
    ui->scrollArea->setWidgetResizable(true);
    connect(ui->scrollArea->verticalScrollBar(), &QScrollBar::rangeChanged, this, [=]()
    {
        ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->maximum());
    });
}

history::~history()
{
    delete ui;
}
void history::addChildWindow(QString name,QString msg) {
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->scrollAreaWidgetContents->layout());
    if (!layout) return;
    historychild *newChild = new historychild(name,msg,this);
    layout->addWidget(newChild);
    // 添加子窗口后，滑动到最底部
    // 等待布局更新后滑动到最底部
    qInfo() << "添加日志：" << msg;
}
/*圆角边框*/
void history::paintEvent(QPaintEvent *event)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    QRectF rect(5, 5, this->width() - 10, this->height() - 10);
    path.addRoundedRect(rect, 15, 15);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));
    QColor color(0, 0, 0, 50);
    for (int i = 0; i < 5; i++)
    {
        QPainterPath shadowPath;
        shadowPath.setFillRule(Qt::WindingFill);
        // 使用圆角矩形而不是普通矩形绘制阴影
        QRectF shadowRect((5 - i), (5 - i) , this->width() - (5 - i) * 2, this->height() - (5 - i) * 2);
        shadowPath.addRoundedRect(shadowRect, 15, 15); //添加圆角矩形路径
        // 增加透明度效果，模拟阴影逐渐变淡
        color.setAlpha(50 - qSqrt(i) * 22);
        painter.setPen(color);
        painter.drawPath(shadowPath); //绘制阴影路径
    }
}
