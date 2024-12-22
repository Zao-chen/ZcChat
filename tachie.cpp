#include "tachie.h"
#include "ui_tachie.h"

#include <QMouseEvent>
#include <QSettings>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

tachie::tachie(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tachie)
{
    ui->setupUi(this);
    //无边框设置
    this->setWindowFlags(Qt::Tool); //当前窗口的构造函数下调用
    this->setWindowFlags(Qt::ToolTip); //同时隐藏任务栏图标和标题栏图标
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowFlags (windowFlags () | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    //初始化
    init_from_main();
    }
/*初始化*/
void tachie::init_from_main()
{
    QSettings *settings = new QSettings(qApp->applicationDirPath()+"/Setting.ini",QSettings::IniFormat);
    qDebug()<<qApp->applicationDirPath()+"/tachie/"+settings->value("tachie/name").toString()+"/正常.png";
    QPixmap pixmap(qApp->applicationDirPath()+"/tachie/"+settings->value("tachie/name").toString()+"/正常.png");
    ui->label->setPixmap(pixmap.scaled(pixmap.width()*(settings->value("tachie/size").toInt()/100.0),pixmap.height()*(settings->value("tachie/size").toInt()/100.0),Qt::KeepAspectRatio,Qt::SmoothTransformation));
}
void tachie::resetlocation_from_main()
{
    this->move(QCursor::pos());
}
/*立绘修改*/
void tachie::changetachie_from_galdialog(QString name)
{
    QSettings *settings = new QSettings(qApp->applicationDirPath()+"/Setting.ini",QSettings::IniFormat);
    qDebug()<<"【接收】对话框 --- 修改立绘"+name+" ---> 立绘";
    qDebug()<<qApp->applicationDirPath()+"/tachie/"+settings->value("tachie/name").toString()+"/"+name+".png";
    QPixmap pixmap;
    pixmap.load(qApp->applicationDirPath()+"/tachie/"+settings->value("tachie/name").toString()+"/"+name+".png");
    qDebug()<<pixmap.isNull();
    if(pixmap.isNull()) pixmap.load(qApp->applicationDirPath()+"/tachie/"+settings->value("tachie/name").toString()+"/正常.png");
    ui->label->setPixmap(pixmap.scaled(pixmap.width()*(settings->value("tachie/size").toInt()/100.0),pixmap.height()*(settings->value("tachie/size").toInt()/100.0),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    //动画
    QSettings *config = new QSettings(qApp->applicationDirPath()+"/tachie/"+settings->value("/tachie/name").toString()+"/config.ini",QSettings::IniFormat);
    // 创建动画组
    QSequentialAnimationGroup *animationGroup = new QSequentialAnimationGroup;
    switch (config->value(name+"/animation").toInt()) {
    case 1: //左右晃动
    {
        QPropertyAnimation *moveUp = new QPropertyAnimation(ui->label, "geometry");
        moveUp->setDuration(250); // 持续时间 1 秒
        moveUp->setStartValue(QRect(0, 0, pixmap.width()*(settings->value("tachie/size").toInt()/100.0), pixmap.height()*(settings->value("tachie/size").toInt()/100.0))); // 起始位置
        moveUp->setEndValue(QRect(0, -15, pixmap.width()*(settings->value("tachie/size").toInt()/100.0), pixmap.height()*(settings->value("tachie/size").toInt()/100.0))); // 结束位置
        QPropertyAnimation *moveDown = new QPropertyAnimation(ui->label, "geometry");
        moveDown->setDuration(250); // 持续时间 1 秒
        moveDown->setStartValue(QRect(0, -15, pixmap.width()*(settings->value("tachie/size").toInt()/100.0), pixmap.height()*(settings->value("tachie/size").toInt()/100.0))); // 起始位置
        moveDown->setEndValue(QRect(0, 0, pixmap.width()*(settings->value("tachie/size").toInt()/100.0), pixmap.height()*(settings->value("tachie/size").toInt()/100.0))); // 结束位置
        //将动画按顺序添加到动画组
        animationGroup->addAnimation(moveUp);
        animationGroup->addAnimation(moveDown);
        break;
    }
    case 2: //上下晃动
    {
        QPropertyAnimation *moveleft = new QPropertyAnimation(ui->label, "geometry");
        moveleft->setDuration(250); // 持续时间 1 秒
        moveleft->setStartValue(QRect(0, 0, pixmap.width()*(settings->value("tachie/size").toInt()/100.0), pixmap.height()*(settings->value("tachie/size").toInt()/100.0))); // 起始位置
        moveleft->setEndValue(QRect(-15, 0, pixmap.width()*(settings->value("tachie/size").toInt()/100.0), pixmap.height()*(settings->value("tachie/size").toInt()/100.0))); // 结束位置
        QPropertyAnimation *moveright = new QPropertyAnimation(ui->label, "geometry");
        moveright->setDuration(500); // 持续时间 1 秒
        moveright->setStartValue(QRect(-15, 0, pixmap.width()*(settings->value("tachie/size").toInt()/100.0), pixmap.height()*(settings->value("tachie/size").toInt()/100.0))); // 起始位置
        moveright->setEndValue(QRect(15, 0, pixmap.width()*(settings->value("tachie/size").toInt()/100.0), pixmap.height()*(settings->value("tachie/size").toInt()/100.0))); // 结束位置
        QPropertyAnimation *moveleft2 = new QPropertyAnimation(ui->label, "geometry");
        moveleft2->setDuration(250); // 持续时间 1 秒
        moveleft2->setStartValue(QRect(15, 0, pixmap.width()*(settings->value("tachie/size").toInt()/100.0), pixmap.height()*(settings->value("tachie/size").toInt()/100.0))); // 起始位置
        moveleft2->setEndValue(QRect(0, 0, pixmap.width()*(settings->value("tachie/size").toInt()/100.0), pixmap.height()*(settings->value("tachie/size").toInt()/100.0))); // 结束位置
        //将动画按顺序添加到动画组
        animationGroup->addAnimation(moveleft);
        animationGroup->addAnimation(moveright);
        animationGroup->addAnimation(moveleft2);
        break;
    }
    case 3: //放大
    {
        ui->label->setScaledContents(true); // 确保图片随尺寸缩放
        // 使用 QPropertyAnimation 创建放大动画
        QPropertyAnimation *animation = new QPropertyAnimation(ui->label, "geometry");
        // 创建动画
        animation->setDuration(500); // 动画时长 500 毫秒
        animation->setStartValue(ui->label->geometry());
        animation->setEndValue(QRect(ui->label->geometry().x()-50,ui->label->geometry().y()-50,ui->label->geometry().width()+100,ui->label->geometry().height()+100));
        animation->start(QAbstractAnimation::DeleteWhenStopped);
        break;
    }
    case 4: //缩小
    {
        ui->label->setScaledContents(true); // 确保图片随尺寸缩放
        // 使用 QPropertyAnimation 创建放大动画
        QPropertyAnimation *animation = new QPropertyAnimation(ui->label, "geometry");
        // 创建动画
        animation->setDuration(500); // 动画时长 500 毫秒
        animation->setStartValue(ui->label->geometry());
        animation->setEndValue(QRect(ui->label->geometry().x()+50,ui->label->geometry().y()+50,ui->label->geometry().width()-100,ui->label->geometry().height()-100));
        animation->start(QAbstractAnimation::DeleteWhenStopped);
        break;
    }
    default:
        break;
    }
    //启动动画
    animationGroup->start();
}
tachie::~tachie()
{
    delete ui;
}
/*鼠标事件*/
//鼠标按下事件
void tachie::mousePressEvent(QMouseEvent *event)
{
    switch(event->button())
    {
    case Qt::LeftButton:
        isLeftPressDown = true;
        this->mouseGrabber(); //返回当前抓取鼠标输入的窗口
        m_movePoint = event->globalPos() - this->frameGeometry().topLeft();
        break;
    case Qt::RightButton:
        emit show_dialogwin_to_main();
        qDebug()<<"【发送】立绘 --- 打开对话框 ---> 主窗口";
        break;
    default:
        tachie::mousePressEvent(event);
    }
}
//鼠标移动事件
void tachie::mouseMoveEvent(QMouseEvent *event)
{
    if(isLeftPressDown)  //没有按下左键时
    {
        move(event->globalPos() - m_movePoint); //移动窗口
        event->accept();
    }
}
//鼠标释放事件
void tachie::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit changeLocation_to_main(this->x(),this->y());
        qDebug()<<"【发送】立绘 --- 保存位置 ---> 主窗口"<<this->x()<<" "<<this->y();
        isLeftPressDown = false;
        this->releaseMouse(); //释放鼠标抓取
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}
