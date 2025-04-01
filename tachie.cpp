#include "tachie.h"
#include "ui_tachie.h"

#include <QMouseEvent>
#include <QSettings>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QGraphicsOpacityEffect>
#include <QStandardPaths>

tachie::tachie(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tachie)
{
    ui->setupUi(this);
    /*无边框设置*/
    setAttribute(Qt::WA_TranslucentBackground);
    /*初始化*/
    init_from_main(true);
}
/*初始化*/
void tachie::init_from_main(bool pin)
{
    //置顶状态修改
    qInfo()<<"置顶状态"<<pin;
    if (pin)
        setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    else
        setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint);
    //重新显示窗口
    show();
    QString appFolder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat";
    //创建 QSettings 对象
    QSettings *settings = new QSettings(appFolder + "/Setting.ini", QSettings::IniFormat);
    QSettings *settings_actor = new QSettings(appFolder + "/characters_config/" + settings->value("actor/name").toString() + "/config.ini", QSettings::IniFormat);
    QPixmap pixmap;
    pixmap.load(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/characters/" + settings->value("actor/name").toString() + "/正常.png");
    //缩放新图片并设置到 label
    QPixmap scaledPixmap = pixmap.scaled(
        pixmap.width() * (settings_actor->value("tachie/size").toInt() / 100.0),
        pixmap.height() * (settings_actor->value("tachie/size").toInt() / 100.0),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);
    ui->label->setPixmap(scaledPixmap);
    this->setFixedSize(pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0),pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0));
    qInfo()<<"立绘缩放为："<<settings_actor->value("tachie/size").toInt()/100.0;
}
/*重置立绘位置*/
void tachie::resetlocation_from_main()
{
    this->move(0,0);
}
/*立绘修改*/
void tachie::changetachie_from_galdialog(QString name)
{
    QSettings *settings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/Setting.ini", QSettings::IniFormat);
    QSettings *settings_actor = new QSettings(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/characters_config/" + settings->value("actor/name").toString() + "/config.ini", QSettings::IniFormat);
    qInfo()<<"【接收】对话框 --- 修改立绘"+name+" ---> 立绘";
    QPixmap pixmap;
    if (!pixmap.load(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/characters/" + settings->value("actor/name").toString() + "/" + name.replace(" ", "") + ".png"))
    {
        qWarning() << "未找到立绘，使用默认立绘";
        pixmap.load(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/characters/" + settings->value("actor/name").toString() + "/正常.png");
    }
    // 将 label_2 设置为原有图片
    ui->label_2->setPixmap(ui->label->pixmap());
    ui->label->hide();
    // 缩放新图片并设置到 label
    QPixmap scaledPixmap = pixmap.scaled(
        pixmap.width() * (settings_actor->value("tachie/size").toInt() / 100.0),
        pixmap.height() * (settings_actor->value("tachie/size").toInt() / 100.0),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);
    ui->label->setPixmap(scaledPixmap);
    // 设置 label_1 的淡入效果
    QGraphicsOpacityEffect *effect1 = new QGraphicsOpacityEffect(ui->label);
    ui->label->setGraphicsEffect(effect1);
    QPropertyAnimation *animation1 = new QPropertyAnimation(effect1, "opacity");
    animation1->setDuration(200);  // 动画持续时间（毫秒）
    animation1->setStartValue(0.5);
    animation1->setEndValue(1.0);
    // 设置 label_2 的淡出效果
    QGraphicsOpacityEffect *effect2 = new QGraphicsOpacityEffect(ui->label_2);
    ui->label_2->setGraphicsEffect(effect2);
    QPropertyAnimation *animation2 = new QPropertyAnimation(effect2, "opacity");
    animation2->setDuration(500);
    animation2->setStartValue(1.0);
    animation2->setEndValue(0.0);
    // 同步动画播放
    animation1->start();
    animation2->start();
    ui->label->show();
    qInfo()<<"立绘缩放为："<<settings_actor->value("tachie/size").toInt()/100.0;
    //动画
    QSettings *config = new QSettings(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/characters/" + settings->value("actor/name").toString() + "/anim.ini", QSettings::IniFormat);
    //创建动画组
    QSequentialAnimationGroup *animationGroup = new QSequentialAnimationGroup;
    qInfo()<<"播放动画"<<config->value(name+"/animation").toInt();
    switch (config->value(name+"/animation").toInt()) {
    case 1: //左右晃动
    {
        QPropertyAnimation *moveUp = new QPropertyAnimation(ui->widget, "geometry");
        moveUp->setDuration(250); //持续时间 1 秒
        moveUp->setStartValue(QRect(0, 0, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 起始位置
        moveUp->setEndValue(QRect(0, -15, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 结束位置
        QPropertyAnimation *moveDown = new QPropertyAnimation(ui->widget, "geometry");
        moveDown->setDuration(250); //持续时间 1 秒
        moveDown->setStartValue(QRect(0, -15, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 起始位置
        moveDown->setEndValue(QRect(0, 0, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 结束位置
        //将动画按顺序添加到动画组
        animationGroup->addAnimation(moveUp);
        animationGroup->addAnimation(moveDown);
        break;
    }
    case 2: //上下晃动
    {
        QPropertyAnimation *moveleft = new QPropertyAnimation(ui->widget, "geometry");
        moveleft->setDuration(250); //持续时间 1 秒
        moveleft->setStartValue(QRect(0, 0, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 起始位置
        moveleft->setEndValue(QRect(-15, 0, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 结束位置
        QPropertyAnimation *moveright = new QPropertyAnimation(ui->widget, "geometry");
        moveright->setDuration(500); //持续时间 1 秒
        moveright->setStartValue(QRect(-15, 0, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 起始位置
        moveright->setEndValue(QRect(15, 0, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 结束位置
        QPropertyAnimation *moveleft2 = new QPropertyAnimation(ui->widget, "geometry");
        moveleft2->setDuration(250); //持续时间 1 秒
        moveleft2->setStartValue(QRect(15, 0, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 起始位置
        moveleft2->setEndValue(QRect(0, 0, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 结束位置
        //将动画按顺序添加到动画组
        animationGroup->addAnimation(moveleft);
        animationGroup->addAnimation(moveright);
        animationGroup->addAnimation(moveleft2);
        break;
    }
    case 3: //放大
    {
        //获取 QLabel 的原始几何和宽高比
        QRect originalGeometry = ui->widget->geometry();
        qreal aspectRatio = static_cast<qreal>(originalGeometry.width()) / originalGeometry.height();
        //计算目标尺寸，保持宽高比
        int delta = -50; //缩小的总大小
        int newWidth = originalGeometry.width() - delta;
        int newHeight = static_cast<int>(newWidth / aspectRatio);
        // 调整目标 QRect，确保宽高比一致
        QRect targetGeometry(originalGeometry.x() + delta / 2,
                             originalGeometry.y() + (originalGeometry.height() - newHeight) / 2,
                             newWidth,
                             newHeight);
        //使用 QPropertyAnimation 创建放大动画
        QPropertyAnimation *animation = new QPropertyAnimation(ui->widget, "geometry");
        animation->setDuration(500); //动画时长 500 毫秒
        animation->setStartValue(originalGeometry);
        animation->setEndValue(targetGeometry);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
        break;
    }
    case 4: //缩小
    {
        //获取 QLabel 的原始几何和宽高比
        QRect originalGeometry = ui->widget->geometry();
        qreal aspectRatio = static_cast<qreal>(originalGeometry.width()) / originalGeometry.height();
        //计算目标尺寸，保持宽高比
        int delta = 50; // 缩小的总大小
        int newWidth = originalGeometry.width() - delta;
        int newHeight = static_cast<int>(newWidth / aspectRatio);
        //调整目标 QRect，确保宽高比一致
        QRect targetGeometry(originalGeometry.x() + delta / 2,
                             originalGeometry.y() + (originalGeometry.height() - newHeight) / 2,
                             newWidth,
                             newHeight);
        //使用 QPropertyAnimation 创建放大动画
        QPropertyAnimation *animation = new QPropertyAnimation(ui->widget, "geometry");
        animation->setDuration(500); //动画时长 500 毫秒
        animation->setStartValue(originalGeometry);
        animation->setEndValue(targetGeometry);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
        break;
    }
    case 5:
    {
        for(int i=0;i!=10;i++)
        {
            QPropertyAnimation *moveUp = new QPropertyAnimation(ui->widget, "geometry");
            moveUp->setDuration(20); //持续时间 1 秒
            moveUp->setStartValue(QRect(0, 0, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 起始位置
            moveUp->setEndValue(QRect(0, -4, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 结束位置
            QPropertyAnimation *moveDown = new QPropertyAnimation(ui->widget, "geometry");
            moveDown->setDuration(40); //持续时间 1 秒
            moveDown->setStartValue(QRect(0, -4, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 起始位置
            moveDown->setEndValue(QRect(0, 4, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 结束位置
            QPropertyAnimation *moveUp2 = new QPropertyAnimation(ui->widget, "geometry");
            moveUp2->setDuration(20); //持续时间 1 秒
            moveUp2->setStartValue(QRect(0, 0, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 起始位置
            moveUp2->setEndValue(QRect(0, -4, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 结束位置
            QPropertyAnimation *moveleft = new QPropertyAnimation(ui->widget, "geometry");
            moveleft->setDuration(20); //持续时间 1 秒
            moveleft->setStartValue(QRect(0, 0, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 起始位置
            moveleft->setEndValue(QRect(-4, 0, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 结束位置
            QPropertyAnimation *moveright = new QPropertyAnimation(ui->widget, "geometry");
            moveright->setDuration(40); //持续时间 1 秒
            moveright->setStartValue(QRect(-4, 0, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 起始位置
            moveright->setEndValue(QRect(4, 0, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 结束位置
            QPropertyAnimation *moveleft2 = new QPropertyAnimation(ui->widget, "geometry");
            moveleft2->setDuration(20); //持续时间 1 秒
            moveleft2->setStartValue(QRect(4, 0, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 起始位置
            moveleft2->setEndValue(QRect(0, 0, pixmap.width()*(settings_actor->value("tachie/size").toInt()/100.0), pixmap.height()*(settings_actor->value("tachie/size").toInt()/100.0))); // 结束位置
            //将动画按顺序添加到动画组
            animationGroup->addAnimation(moveUp);
            animationGroup->addAnimation(moveDown);
            animationGroup->addAnimation(moveUp2);
            animationGroup->addAnimation(moveleft);
            animationGroup->addAnimation(moveright);
            animationGroup->addAnimation(moveleft2);
        }
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
        qInfo()<<"【发送】立绘 --- 打开对话框 ---> 主窗口";
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
        qInfo()<<"【发送】立绘 --- 保存位置 ---> 主窗口"<<this->x()<<" "<<this->y();
        isLeftPressDown = false;
        this->releaseMouse(); //释放鼠标抓取
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}
