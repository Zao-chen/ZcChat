#include "galgamedialog.h"
#include "ui_galgamedialog.h"

#include <QMouseEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QAudioOutput>
#include <QBuffer>
#include <QFile>
#include <QSettings>
#include <QTimer>
#include <QPainter>

galgamedialog::galgamedialog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::galgamedialog)
{
    ui->setupUi(this);
    /*无边框设置*/
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setWindowOpacity(0.9);
    setAttribute(Qt::WA_TranslucentBackground);
    /*内容初始化*/
    ui->pushButton->hide();
    ui->label_name->setText("你");
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &galgamedialog::updateText);
}

galgamedialog::~galgamedialog()
{
    delete ui;
}
void galgamedialog::keyPressEvent(QKeyEvent* event)
{
    keys.append(event->key());
}
void galgamedialog::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return)
    {
        if (!keys.contains(Qt::Key_Shift)) //过滤换行
        {
            QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
            //对话框设置
            ui->label_name->setText(settings->value("/tachie/name").toString());
            ui->textEdit->setEnabled(false);
            ui->pushButton->hide();
            //去除换行
            QTextCursor cursor=ui->textEdit->textCursor();//得到当前text的光标
            if(cursor.hasSelection()) cursor.clearSelection();
            cursor.deletePreviousChar();//删除前一个字符
            QJsonDocument jsonDoc = QJsonDocument::fromJson(Urlpost().toUtf8());


            QString message = "正常|[error]未知错误|错误error";
            // 获取顶层对象
            QJsonObject rootObj = jsonDoc.object();

            // 获取 messages 数组
            QJsonArray messages = rootObj["messages"].toArray();

            // 遍历 messages
            for (const QJsonValue &messageVal : messages) {
                QJsonObject messageObj = messageVal.toObject();
                if (messageObj["message_type"].toString() == "function_call") {
                    QJsonObject functionCall = messageObj["function_call"].toObject();

                    // 判断 name 是否为 "send_message"
                    if (functionCall["name"].toString() == "send_message") {
                        QString arguments = functionCall["arguments"].toString();

                        // 提取 "message" 内容
                        QJsonDocument argumentsDoc = QJsonDocument::fromJson(arguments.toUtf8());
                        if (!argumentsDoc.isNull() && argumentsDoc.isObject()) {
                            message = argumentsDoc.object()["message"].toString();
                            qDebug() << "Extracted message:" << message;
                        }
                    }
                }
            }

            if(message.isNull())
            {
                message = "正常|[error] Letta返回了["+jsonDoc.toJson()+"]，可能是Letta未启动/agent配置错误|错误error";
            }
            else if(message.split("|").size()!=3)
            {
                if(!settings->value("/llm/feedback").toBool()) message = "正常|[error] Letta正常返回，但是返回值格式错误，返回值["+message+"]|错误error";
                else message = "正常|"+message+"|";
            }

            emit change_tachie_to_tachie(message.split("|")[0]);
            qDebug()<<"【发送】对话框 --- 修改立绘"+message.split("|")[0]+" ---> 立绘";


            if(settings->value("/vits/enable").toBool())
            {
                QNetworkAccessManager* manager = new QNetworkAccessManager(this);
                QNetworkReply* reply;
                if(settings->value("/vits/api").toInt()==1)
                    reply = manager->get(QNetworkRequest(QUrl(settings->value("/vits/custom_url").toString().replace("{msg}",message.split("|")[2]))));
                else
                    reply = manager->get(QNetworkRequest(QUrl(settings->value("/vits/url").toString()+"/voice/"+settings->value("/vits/vitsmodel").toString()+"?text="+message.split("|")[2]+"&id="+settings->value("/vits/id").toString()+"&format=mp3")));
                connect(reply, &QNetworkReply::finished, this, [=]() {
                    if (reply->error() == QNetworkReply::NoError) {
                        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
                        {
                            QFile outputFile(qApp->applicationDirPath()+"/temp.mp3");
                            if (outputFile.open(QIODevice::WriteOnly)) {
                                outputFile.write(reply->readAll());
                                outputFile.close();
                            }
                            // 创建 QMediaPlayer 对象
                            QMediaPlayer *player = new QMediaPlayer;
                            // 创建 QAudioOutput 对象来控制音量
                            QAudioOutput *audioOutput = new QAudioOutput;
                            // 将 QAudioOutput 连接到 QMediaPlayer
                            player->setAudioOutput(audioOutput);
                            // 设置媒体源文件
                            player->setSource(QUrl::fromLocalFile(qApp->applicationDirPath()+"/temp.mp3"));
                            // 设置音量（0 - 100）
                            audioOutput->setVolume(1);  // 0.0 为最小音量，1.0 为最大音量
                            // 播放音频
                            player->play();
                            changetext(message.split("|")[1]);
                            ui->pushButton->show();
                        }
                    }
                    else
                    {
                        ui->textEdit->setText("[error] Vits错误，请检查Vits配置或者关闭语言输出");
                        ui->pushButton->show();
                    }
                });
            }
            else
            {
                changetext(message.split("|")[1]);
                ui->pushButton->show();
            }
        }
    }

    keys.removeAll(event->key());
}
QString galgamedialog::Urlpost()
{
    /*post请求*/
    qDebug()<<"post!";
    QNetworkRequest request;
    //头设置
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    request.setUrl(QUrl(settings->value("/llm/url").toString()+"/v1/agents/"+settings->value("/llm/agent").toString()+"/messages"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    QNetworkAccessManager* naManager = new QNetworkAccessManager(this);
    // 创建根对象
    QJsonObject rootObject;
    // 创建messages数组
    QJsonArray messagesArray;
    // 创建messages数组中的第一个对象
    QJsonObject messageObject;
    messageObject["role"] = "user";
    messageObject["text"] = ui->textEdit->toPlainText();
    ui->textEdit->setText("...");
    // 将messageObject添加到messagesArray
    messagesArray.append(messageObject);
    // 将messagesArray添加到rootObject
    rootObject["messages"] = messagesArray;
    // 添加其他键值对到rootObject
    rootObject["stream_steps"] = false;
    rootObject["stream_tokens"] = false;
    // 创建QJsonDocument并设置其为rootObject
    QJsonDocument jsonDoc(rootObject);
    // 发送 POST 请求
    QEventLoop loop;
    QNetworkReply* reply = naManager->post(request, jsonDoc.toJson());
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QString read;
    read = reply->readAll();
    read = read.replace(" ","");
    reply->deleteLater();       //记得释放内存
    qDebug()<<"POST:"<<read;
    return read;
}
QByteArray galgamedialog::getUrl(const QString &input)
{
    m_manager = new QNetworkAccessManager(this);//新建QNetworkAccessManager对象
    QString read;
    QEventLoop loop;
    QNetworkReply *reply = m_manager->get(QNetworkRequest(QUrl(input)));
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    return reply->readAll();
}
void galgamedialog::changetext(QString text) //逐字显示
{
    qDebug()<<"输出中文:"<<text;
    timer->stop();
    fullText = text;
    currentIndex = 0;
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    timer->start(settings->value("/dialog/time").toInt());
}
//逐字显示-更新
void galgamedialog::updateText() {
    if (currentIndex <= fullText.length()) {
        ui->textEdit->setText(fullText.left(++currentIndex));
        qDebug()<<"逐字输出中文:"<<fullText.left(++currentIndex);
    }
    return;
}
//三个鼠标事件的重写
//鼠标按下事件
void galgamedialog::mousePressEvent(QMouseEvent *event)
{
    switch(event->button())
    {
    case Qt::LeftButton:
        isLeftPressDown = true;
        this->mouseGrabber(); //返回当前抓取鼠标输入的窗口
        m_movePoint = event->globalPos() - this->frameGeometry().topLeft();
        break;
    case Qt::RightButton:
        break;
    default:
        galgamedialog::mousePressEvent(event);
    }
}
//鼠标移动事件
void galgamedialog::mouseMoveEvent(QMouseEvent *event)
{
    if(isLeftPressDown)  //没有按下左键时
    {
        move(event->globalPos() - m_movePoint); //移动窗口
        event->accept();
    }
}
//鼠标释放事件
void galgamedialog::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isLeftPressDown = false;
        this->releaseMouse(); //释放鼠标抓取
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}

void galgamedialog::on_pushButton_clicked()
{
    ui->label_name->setText("你");
    ui->textEdit->clear();
    ui->textEdit->setEnabled(true);
    ui->pushButton->hide();
}


void galgamedialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);// 创建一个QPainter对象并指定绘制设备为this，即当前窗口
    painter.setRenderHint(QPainter::Antialiasing);  // 设置绘制选项为反锯齿，使绘制的图形边缘更加平滑
    painter.setBrush(QBrush(QColor(240,243,244))); //设置画刷颜色,这里为白色
    painter.setPen(Qt::transparent); //设置画笔颜色为透明，即不绘制边框线
    QRect rect = this->rect(); //获取当前窗口的矩形区域
    painter.drawRoundedRect(rect, 15, 15);  // 绘制一个带有圆角的矩形窗口，圆角半径为15px，如果把窗口设置成正方形，圆角半径设大，就会变成一个圆了

}
//需添加头文件 <QPainter>和 <QStyleOption>
