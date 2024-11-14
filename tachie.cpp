#include "tachie.h"
#include "ui_tachie.h"

#include <QMouseEvent>

tachie::tachie(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tachie)
{
    ui->setupUi(this);
    /*无边框设置*/
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowFlags (windowFlags () | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    QPixmap pixmap(qApp->applicationDirPath()+"/Murasame/正常.png");
    ui->label->setPixmap(pixmap.scaled(pixmap.width()*0.5,pixmap.height()*0.5));
}

tachie::~tachie()
{
    delete ui;
}
//三个鼠标事件的重写
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
        isLeftPressDown = false;
        this->releaseMouse(); //释放鼠标抓取
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}
void tachie::changetachie_from_galdialog(QString name)
{
    qDebug()<<"【接收】对话框 --- 修改立绘"+name+" ---> 立绘";
    QPixmap pixmap(qApp->applicationDirPath()+"/Murasame/"+name+".png");
    ui->label->setPixmap(pixmap.scaled(pixmap.width()*0.5,pixmap.height()*0.5));
}
