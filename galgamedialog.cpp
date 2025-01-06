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
#include <QAudioInput>
#include <QMediaFormat>
#include <QAudioDevice>
#include <QDir>
#include <QMediaDevices>
#include <QHttpPart>
#include <QAudioFormat>
#include <QAudioSource>
#include <QDebug>
#include <QUrlQuery>

galgamedialog::galgamedialog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::galgamedialog)
{
    ui->setupUi(this);
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    /*无边框设置*/
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setWindowOpacity(0.9);
    setAttribute(Qt::WA_TranslucentBackground);
    /*内容初始化*/
    ui->pushButton->hide();
    ui->label_name->setText("你");
    emit init_from_main();
    /*逐字显示*/
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &galgamedialog::updateText);
}
galgamedialog::~galgamedialog()
{
    delete ui;
}
/*按键相关*/
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
            send_to_llm();
        }
    }
    keys.removeAll(event->key());
}
/*LLMpost请求*/
QString galgamedialog::UrlpostLLM()
{
    QNetworkAccessManager* naManager = new QNetworkAccessManager(this);
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    QNetworkRequest request;
    //头设置
    request.setUrl(QUrl(settings->value("/llm/url").toString()+"/v1/agents/"+settings->value("/llm/agent").toString()+"/messages"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    //json处理
    QJsonObject rootObject;
    QJsonArray messagesArray;
    QJsonObject messageObject;
    messageObject["role"] = "user";
    messageObject["text"] = ui->textEdit->toPlainText();
    ui->textEdit->setText("...");
    messagesArray.append(messageObject); //将messageObject添加到messagesArray
    rootObject["messages"] = messagesArray; //将messagesArray添加到rootObject
    rootObject["stream_steps"] = false; //添加其他键值对到rootObject
    rootObject["stream_tokens"] = false; //创建QJsonDocument并设置其为rootObject
    QJsonDocument jsonDoc(rootObject);
    //获取结果
    QEventLoop loop;
    QNetworkReply* reply = naManager->post(request, jsonDoc.toJson());
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QString read;
    read = reply->readAll();
    read = read.replace(" ","");
    reply->deleteLater(); //记得释放内存
    qDebug()<<"获取到LLMpost结果:"<<read;
    return read;
}
/*语言识别post请求*/
QString galgamedialog::UrlpostWithFile()
{
    QFile *file = new QFile(QDir::currentPath() + "/output.m4a");
    if(file->size()<=7000)
    {
        qDebug()<<file->size()<<"文件太小，认定为噪音";
        return "";
    }
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    if(settings->value("/speechInput/api").toInt()==0)
    {
        QNetworkAccessManager *manager = new QNetworkAccessManager();
        //设置请求 URL
        QUrl url(settings->value("/speechInput/url").toString()+"/asr?language=zh&output=txt");
        QNetworkRequest request(url);
        //设置请求头
        request.setRawHeader("accept", "application/json");
        //创建 QHttpMultiPart 对象
        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        //添加文件字段
        if (!file->open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to open file!";
            return "Failed to open file!";
        }
        QHttpPart filePart;
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"audio_file\"; filename=\"output.m4a\""));
        filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("audio/x-m4a"));
        filePart.setBodyDevice(file);
        //这行代码确保了文件在 multiPart 被销毁时能够正确销毁
        file->setParent(multiPart);
        multiPart->append(filePart);
        //发送 POST 请求
        QEventLoop loop;
        QNetworkReply *reply = manager->post(request, multiPart);
        //确保多部分表单在回复结束后被销毁
        multiPart->setParent(reply);
        //确保在请求完成后退出事件循环
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();  //等待请求完成
        //处理响应结果
        if (reply->error() == QNetworkReply::NoError)
        {
            QString msg = reply->readAll();
            qDebug() << "Response:" << msg;
            return msg;
        }
        else
        {
            qDebug() << "Error:" << reply->errorString();
            qDebug() << "Response details:" << reply->readAll();
        }
        reply->deleteLater();
        return "error";
    }
    else
    {
        QNetworkAccessManager manager;

        // 创建请求 URL 和查询参数
        QUrl url("https://aip.baidubce.com/oauth/2.0/token");
        QUrlQuery query;
        query.addQueryItem("grant_type", "client_credentials");
        query.addQueryItem("client_id", settings->value("/speechInput/baidu_apikey").toString()); // 替换为你的实际client_id
        query.addQueryItem("client_secret", settings->value("/speechInput/baidu_secretkey").toString()); // 替换为你的实际client_secret
        url.setQuery(query);
        // 创建请求对象并设置头信息
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        // 创建事件循环
        QEventLoop loop;
        // 发送 POST 请求
        QNetworkReply *reply = manager.post(request, QByteArray());
        // 当请求完成时，退出事件循环
        QString accessToken;
        QObject::connect(reply, &QNetworkReply::finished, [&]() {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray response = reply->readAll();
                QJsonDocument doc = QJsonDocument::fromJson(response);
                if (!doc.isNull()) {
                    QJsonObject jsonObj = doc.object();
                    // 获取access_token
                    accessToken = jsonObj.value("access_token").toString();
                    qDebug() << "Access Token:" << accessToken;
                } else {
                    qDebug() << "Failed to parse JSON response";
                }
            } else {
                qDebug() << "Request failed:" << reply->errorString();
            }
            reply->deleteLater();
            loop.quit();  // 退出事件循环
        });
        // 进入事件循环，等待请求完成
        loop.exec();
        /*识别*/
        QNetworkAccessManager manager_api;
        // 读取本地音频文件为二进制数据
        QString audioFilePath = QDir::currentPath() + "/output.m4a"; // 音频文件路径
        QFile file(audioFilePath);
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "无法打开音频文件" << audioFilePath;
        }
        QByteArray audioData = file.readAll();
        file.close();
        QString resultJson;
        if (!audioData.isEmpty()) {
            // 将音频数据进行 Base64 编码
            QString base64AudioData = audioData.toBase64();
            // 创建 JSON 请求体
            QJsonObject json;
            json["format"] = "m4a";  // 音频格式
            json["rate"] = 16000;     // 采样率
            json["channel"] = 1;      // 单声道
            json["token"] = accessToken; // 使用获取的 access_token
            json["cuid"] = "WVqcZK3Tv7iX0kjI4aYuGg4VDUwjZ7k5"; // 替换为设备 ID
            json["speech"] = base64AudioData; // 音频数据的 Base64 编码
            json["len"] = audioData.size(); // 音频文件原始大小（字节数）
            // 将 JSON 数据转换为字节流
            QJsonDocument doc(json);
            QByteArray jsonData = doc.toJson();
            // 创建 POST 请求
            QUrl url("https://vop.baidu.com/server_api");
            QNetworkRequest request(url);
            // 设置请求头部
            request.setRawHeader("Content-Type", "application/json");
            request.setRawHeader("Accept", "application/json");
            // 创建事件循环
            QEventLoop loop;
            // 发送 POST 请求
            QNetworkReply *reply_api= manager_api.post(request, jsonData);
            // 连接请求的 finished 信号
            QObject::connect(reply_api, &QNetworkReply::finished, [&]() {
                if (reply_api->error() == QNetworkReply::NoError) {
                    // 处理响应结果
                    resultJson= reply_api->readAll();
                    qDebug() << "识别成功：" << resultJson;
                } else {
                    qWarning() << "请求失败：" << reply_api->errorString();
                }
                loop.quit();  // 请求完成后退出事件循环
                reply_api->deleteLater();
            });
            // 启动事件循环，等待回复
            loop.exec();
        } else {
            qWarning() << "音频文件为空，跳过请求";
        }
        qDebug()<<"解析"<<resultJson;
        // 解析 JSON 数据
        QString result;
        QJsonDocument doc = QJsonDocument::fromJson(resultJson.toUtf8());
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            // 获取 "result" 字段
            if (obj.contains("result") && obj["result"].isArray()) {
                QJsonArray resultArray = obj["result"].toArray();
                for (const QJsonValue &value : resultArray) {
                    qDebug() << value.toString();
                    result=value.toString();
                }
            }
        }
        return result;
    }
}
/*get请求（用于vits）*/
QByteArray galgamedialog::getUrl(const QString &input)
{
    m_manager = new QNetworkAccessManager(this); //新建QNetworkAccessManager对象
    QEventLoop loop;
    QNetworkReply *reply = m_manager->get(QNetworkRequest(QUrl(input)));
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    return reply->readAll();
}
/*逐字显示*/
void galgamedialog::changetext(QString text)
{
    qDebug()<<"输出中文:"<<text;
    timer->stop();
    fullText = text;
    currentIndex = 0;
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    timer->start(settings->value("/dialog/time").toInt());
}
/*逐字显示-更新*/
void galgamedialog::updateText() {
    if (currentIndex <= fullText.length()) {
        ui->textEdit->setText(fullText.left(++currentIndex));
        qDebug()<<"逐字输出中文:"<<fullText.left(++currentIndex);
    }
    return;
}
/*三个鼠标事件的重写*/
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
/*点击继续*/
void galgamedialog::on_pushButton_clicked()
{
    ui->label_name->setText("你");
    ui->textEdit->clear();
    ui->textEdit->setEnabled(true);
    ui->pushButton->hide();
}
/*圆角边框*/
void galgamedialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this); //创建一个QPainter对象并指定绘制设备为this，即当前窗口
    painter.setRenderHint(QPainter::Antialiasing); //设置绘制选项为反锯齿，使绘制的图形边缘更加平滑
    painter.setBrush(QBrush(QColor(240,243,244))); //设置画刷颜色,这里为白色
    painter.setPen(Qt::transparent); //设置画笔颜色为透明，即不绘制边框线
    QRect rect = this->rect(); //获取当前窗口的矩形区域
    painter.drawRoundedRect(rect, 15, 15);  // 绘制一个带有圆角的矩形窗口，圆角半径为15px，如果把窗口设置成正方形，圆角半径设大，就会变成一个圆了
}
/*录音相关*/
void galgamedialog::on_pushButton_input_pressed()
{
    qDebug() << "开始录音";
    player->stop();
    if (audioRecorder->recorderState() == QMediaRecorder::StoppedState) {
        QMediaFormat format;
        format.setAudioCodec(QMediaFormat::AudioCodec::AAC); //对应编码器
        audioRecorder->setMediaFormat(format);
        audioRecorder->setAudioSampleRate(44100); //设置采样率
        audioRecorder->setAudioChannelCount(2); // 设置声道数
        audioRecorder->setQuality(QMediaRecorder::HighQuality); //设置录制质量
        audioRecorder->setOutputLocation(QUrl::fromLocalFile(QDir::currentPath() + "/output.m4a"));
        audioRecorder->record();
        qDebug() << "录音状态:" << audioRecorder->recorderState();
        qDebug() << "文件路径:" << audioRecorder->outputLocation();
    }
}
void galgamedialog::on_pushButton_input_released()
{
    qDebug() << "结束录音";
    if (audioRecorder)
    {
        audioRecorder->stop();
    }
}
void galgamedialog::init_from_main()
{
    qDebug()<<"dialog初始化";
    /*录音*/
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    if(settings->value("/speechInput/enable").toBool())
    {
        ui->pushButton_input->show();
        ui->checkBox_autoInput->show();
        audioRecorder = new QMediaRecorder(this);
        captureSession.setRecorder(audioRecorder);
        captureSession.setAudioInput(new QAudioInput(this));
        QAudioInput *audioInput = captureSession.audioInput();
        if (audioInput) {
            audioInput->setDevice(QMediaDevices::defaultAudioInput());
            qDebug() << "使用的音频输入设备:" << QMediaDevices::defaultAudioInput().description();
        } else {
            qDebug() << "无法初始化音频输入设备";
            return;
        }
        connect(audioRecorder, &QMediaRecorder::recorderStateChanged, this, [=](QMediaRecorder::RecorderState state) {
            qDebug() << "录音状态已更改:" << state;
            if (state == QMediaRecorder::StoppedState) {
                qDebug() << "录音已停止";
                qDebug() << "录音状态:" << audioRecorder->recorderState();
                QString msg = UrlpostWithFile();
                if(msg!="")
                {
                    ui->textEdit->setText(msg);
                    if(ui->checkBox_autoInput->isChecked())
                    {
                        send_to_llm();
                    }
                }
            }
        });
    }
    else //不开启语言输入
    {
        ui->pushButton_input->hide();
        ui->checkBox_autoInput->hide();
    }
}
void galgamedialog::send_to_llm()
{
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    //对话框设置
    ui->label_name->setText(settings->value("/tachie/name").toString());
    ui->textEdit->setEnabled(false);
    ui->pushButton->hide();
    //去除换行
    QTextCursor cursor=ui->textEdit->textCursor(); //得到当前text的光标
    if(cursor.hasSelection()) cursor.clearSelection();
    cursor.deletePreviousChar(); //删除前一个字符
    //发送post请求
    QJsonDocument jsonDoc = QJsonDocument::fromJson(UrlpostLLM().toUtf8());
    qDebug()<<"接收到post信息："<<jsonDoc;
    //获取到的json处理
    QString message = "正常|[error] 未知错误，请检查letta的报错日志，返回值为["+jsonDoc.toJson()+"]|错误error";
    QJsonObject rootObj = jsonDoc.object();
    QJsonArray messages = rootObj["messages"].toArray();
    for (const QJsonValue &messageVal : messages) {
        QJsonObject messageObj = messageVal.toObject();
        if (messageObj["message_type"].toString() == "tool_call_message") {
            QJsonObject functionCall = messageObj["tool_call"].toObject();
            if (functionCall["name"].toString() == "send_message") {
                QString arguments = functionCall["arguments"].toString();
                QJsonDocument argumentsDoc = QJsonDocument::fromJson(arguments.toUtf8());
                if (!argumentsDoc.isNull() && argumentsDoc.isObject()) {
                    message = argumentsDoc.object()["message"].toString();
                    qDebug() << "Extracted message:" << message;
                }
            }
        }
    }
    //信息判断
    if(message.isNull())
    {
        message = "正常|[error] Letta返回了["+jsonDoc.toJson()+"]，可能是Letta未启动/agent配置错误|错误error";
    }
    else if(message.split("|").size()!=3)
    {
        if(!settings->value("/llm/feedback").toBool()) message = "正常|[error] Letta正常返回，但是返回值格式错误，返回值["+message+"]|错误error";
        //如果忽略报错
        else message = "正常|"+message+"|";
    }
    emit change_tachie_to_tachie(message.split("|")[0]);
    qDebug()<<"【发送】对话框 --- 修改立绘"+message.split("|")[0]+" ---> 立绘";
    //语音合成
    if(settings->value("/vits/enable").toBool())
    {
        QNetworkAccessManager* manager = new QNetworkAccessManager(this);
        QNetworkReply* reply;
        QString text;
        //语音语言选择
        if(settings->value("/vits/lan").toString()=="ja")
            text = message.split("|")[2];
        else
            text = message.split("|")[1];
        //语音api选择
        if(settings->value("/vits/api").toInt()==1)
            reply = manager->get(QNetworkRequest(QUrl(settings->value("/vits/custom_url").toString().replace("{msg}",text))));
        else
            reply = manager->get(QNetworkRequest(QUrl(settings->value("/vits/url").toString()+"/voice/"+settings->value("/vits/vitsmodel").toString()+"?text="+text+"&id="+settings->value("/vits/id").toString()+"&format=mp3"+"&lang="+settings->value("/vits/lan").toString())));
        //播放返回值
        connect(reply, &QNetworkReply::finished, this, [=]() {
            if (reply->error() == QNetworkReply::NoError) {
                if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
                {
                    QFile outputFile(qApp->applicationDirPath()+"/temp.mp3");
                    if (outputFile.open(QIODevice::WriteOnly))
                    {
                        outputFile.write(reply->readAll());
                        outputFile.close();
                    }
                    QAudioOutput *audioOutput = new QAudioOutput; //创建 QAudioOutput 对象来控制音量
                    player->setAudioOutput(audioOutput); //将 QAudioOutput 连接到 QMediaPlayer
                    player->setSource(QUrl::fromLocalFile(qApp->applicationDirPath()+"/temp.mp3")); //设置媒体源文件
                    audioOutput->setVolume(1); //0.0 为最小音量，1.0 为最大音量
                    player->play(); //播放音频
                    changetext(message.split("|")[1]); //逐字显示
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
        changetext(message.split("|")[1]); //逐字显示
        ui->pushButton->show();
    }
}

void galgamedialog::on_checkBox_autoInput_clicked(bool checked)
{
    if(checked)
    {
        /*录音*/
        // 设置音频格式
        format.setSampleRate(16000); // 设置采样率
        format.setChannelCount(1);   // 设置通道数
        format.setSampleFormat(QAudioFormat::Int16); // 设置采样格式为 16 位整数
        // 获取默认的音频输入设备
        QAudioDevice inputDevice = QMediaDevices::defaultAudioInput();
        if (!inputDevice.isFormatSupported(format)) {
            qWarning() << "Default format not supported, trying to use the nearest.";
            format = inputDevice.preferredFormat();
        }
        // 创建 VAD 对象
        vad = new VAD(this);
        // 连接 VAD 的信号到槽函数
        connect(vad, &VAD::voiceDetected, this, [&](bool detected) {
            if (detected)
            {
                if(!is_record)
                {
                    ui->pushButton_input->pressed();
                    is_record = true;
                    qDebug() << "修改t："<<is_record;
                }
            }
            else
            {
                if(is_record)
                {
                    qDebug() << "修改："<<is_record;
                    ui->pushButton_input->released();
                    is_record = false;
                    qDebug() << "修改："<<is_record;
                }
            }
        });
        // 创建音频输入对象
        audioInput = new QAudioSource(inputDevice, format, this);

        // 启动音频输入
        audioDevice = audioInput->start();
        if (!audioDevice) {
            qWarning() << "Failed to start audio input!";
        }
        // 当有音频数据可用时，调用 VAD 进行处理
        connect(audioDevice, &QIODevice::readyRead, this, [=]() {
            QByteArray audioData = audioDevice->readAll();
            vad->processAudio(audioData, format);
        });
    }
    else
    {
        if (audioInput) {
            audioInput->stop(); // 停止音频输入
            qDebug() << "Audio input stopped.";
        }
        if (audioDevice) {
            audioDevice->close(); // 关闭音频设备
            qDebug() << "Audio device closed.";
        }
    }
}

