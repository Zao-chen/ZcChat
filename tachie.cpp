#include "tachie.h"
#include "ui_tachie.h"

#include <QMouseEvent>
#include <QSettings>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

tachie::tachie(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tachie)
{
    ui->setupUi(this);
    /*无边框设置*/
    this->setWindowFlags(Qt::Tool);		//当前窗口的构造函数下调用
    this->setWindowFlags(Qt::ToolTip); 	//同时隐藏任务栏图标和标题栏图标
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowFlags (windowFlags () | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    QSettings *settings = new QSettings(qApp->applicationDirPath()+"/Setting.ini",QSettings::IniFormat);
    settings = new QSettings(qApp->applicationDirPath()+"/Setting.ini",QSettings::IniFormat);
    QPixmap pixmap(qApp->applicationDirPath()+"/tachie/"+settings->value("tachie/name").toString()+"/正常.png");
    ui->label->setPixmap(pixmap.scaled(pixmap.width()*(settings->value("tachie/size").toInt()/100.0),pixmap.height()*(settings->value("tachie/size").toInt()/100.0),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
}
//初始化
void tachie::init_from_main()
{
    QSettings *settings = new QSettings(qApp->applicationDirPath()+"/Setting.ini",QSettings::IniFormat);
    qDebug()<<qApp->applicationDirPath()+"/tachie/"+settings->value("tachie/name").toString()+"/正常.png";
    QPixmap pixmap(qApp->applicationDirPath()+"/tachie/"+settings->value("tachie/name").toString()+"/正常.png");
    ui->label->setPixmap(pixmap.scaled(pixmap.width()*(settings->value("tachie/size").toInt()/100.0),pixmap.height()*(settings->value("tachie/size").toInt()/100.0),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
}
//立绘修改
void tachie::changetachie_from_galdialog(QString name)
{
    QSettings *settings = new QSettings(qApp->applicationDirPath()+"/Setting.ini",QSettings::IniFormat);
    qDebug()<<"【接收】对话框 --- 修改立绘"+name+" ---> 立绘";
    qDebug()<<qApp->applicationDirPath()+"/tachie/"+settings->value("tachie/name").toString()+"/"+name+".png";
    QPixmap pixmap(qApp->applicationDirPath()+"/tachie/"+settings->value("tachie/name").toString()+"/"+name+".png");
    ui->label->setPixmap(pixmap.scaled(pixmap.width()*(settings->value("tachie/size").toInt()/100.0),pixmap.height()*(settings->value("tachie/size").toInt()/100.0),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

    //动画
    QSettings *config = new QSettings(qApp->applicationDirPath()+"/tachie/"+settings->value("/tachie/name").toString()+"/config.ini",QSettings::IniFormat);
    // 创建动画组
    QSequentialAnimationGroup *animationGroup = new QSequentialAnimationGroup;
    switch (config->value(name+"/animation").toInt()) {
    case 1:
    {
        // 第一个动画：向上移动
        QPropertyAnimation *moveUp = new QPropertyAnimation(ui->label, "geometry");
        moveUp->setDuration(250); // 持续时间 1 秒
        moveUp->setStartValue(QRect(0, 0, pixmap.width()*(settings->value("tachie/size").toInt()/100.0), pixmap.height()*(settings->value("tachie/size").toInt()/100.0))); // 起始位置
        moveUp->setEndValue(QRect(0, -15, pixmap.width()*(settings->value("tachie/size").toInt()/100.0), pixmap.height()*(settings->value("tachie/size").toInt()/100.0))); // 结束位置
        // 第二个动画：向下移动
        QPropertyAnimation *moveDown = new QPropertyAnimation(ui->label, "geometry");
        moveDown->setDuration(250); // 持续时间 1 秒
        moveDown->setStartValue(QRect(0, -15, pixmap.width()*(settings->value("tachie/size").toInt()/100.0), pixmap.height()*(settings->value("tachie/size").toInt()/100.0))); // 起始位置
        moveDown->setEndValue(QRect(0, 0, pixmap.width()*(settings->value("tachie/size").toInt()/100.0), pixmap.height()*(settings->value("tachie/size").toInt()/100.0))); // 结束位置
        // 将动画按顺序添加到动画组
        animationGroup->addAnimation(moveUp);
        animationGroup->addAnimation(moveDown);
        break;
    }
    default:
        break;
    }
    // 启动动画
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
