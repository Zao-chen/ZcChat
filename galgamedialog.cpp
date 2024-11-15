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

galgamedialog::galgamedialog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::galgamedialog)
{
    ui->setupUi(this);
    /*无边框设置*/
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setWindowOpacity(0.9);
    /*内容初始化*/
    ui->pushButton->hide();
    ui->label_name->setText("你");
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
            //对话框设置
            ui->label_name->setText("她");
            ui->textEdit->setEnabled(false);
            ui->pushButton->hide();
            //去除换行
            QTextCursor cursor=ui->textEdit->textCursor();//得到当前text的光标
            if(cursor.hasSelection()) cursor.clearSelection();
            cursor.deletePreviousChar();//删除前一个字符
            QJsonDocument jsonDoc = QJsonDocument::fromJson(Urlpost().toUtf8());
            QJsonObject jsonObj = jsonDoc.object();
            QJsonArray messagesArray = jsonObj["messages"].toArray();
            for (const QJsonValue &messageValue : messagesArray) {
                QJsonObject messageObj = messageValue.toObject();
                if (messageObj["message_type"].toString() == "function_call") {
                    QJsonObject functionCallObj = messageObj["function_call"].toObject();
                    QString arguments = functionCallObj["arguments"].toString();
                    QJsonObject argumentsObj = QJsonDocument::fromJson(arguments.toUtf8()).object();
                    QString message = argumentsObj["message"].toString();
                    qDebug() << "Extracted Message:" << message;
                    emit change_tachie_to_tachie(message.split("|")[0]);
                    qDebug()<<"【发送】对话框 --- 修改立绘"+message.split("|")[0]+" ---> 立绘";

                    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
                    if(settings->value("/vits/enable").toBool())
                    {
                        QNetworkAccessManager* manager = new QNetworkAccessManager(this);
                        QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(settings->value("/vits/url").toString()+"/voice/vits?text="+message.split("|")[2]+"&id="+settings->value("/vits/id").toString()+"&format=mp3&lang=zh&length=1")));
                        connect(reply, &QNetworkReply::finished, this, [=]() {
                            if (reply->error() == QNetworkReply::NoError) {
                                if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) {
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
                                    ui->textEdit->setText(message.split("|")[1]);
                                    ui->pushButton->show();
                                }
                            }
                        });
                    }
                    else
                    {
                        ui->textEdit->setText(message.split("|")[1]);
                        ui->pushButton->show();
                    }

                }
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
    return read;
}
QByteArray galgamedialog::getUrl(const QString &input)
{
    /*使用dandanplay进行刮刮乐*/
    m_manager = new QNetworkAccessManager(this);//新建QNetworkAccessManager对象
    QString read;
    QEventLoop loop;
    QNetworkReply *reply = m_manager->get(QNetworkRequest(QUrl(input)));
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    return reply->readAll();
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

