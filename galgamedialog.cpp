#include "galgamedialog.h"
#include "ui_galgamedialog.h"
#include "galgamedialog_uichild/history.h"

#include <QMouseEvent>
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
#include <QUrlQuery>
#include <QProcess>
#include <windows.h>
#include <QDesktopServices>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QPainterPath>
#include <QNetworkInterface>
#include <QStandardPaths>
#include <QDebug>
#include <QScrollBar>

#include "third_party/json/json.hpp"
using json_t = nlohmann::json;

galgamedialog::galgamedialog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::galgamedialog)
    , settings(new QSettings(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/Setting.ini", QSettings::IniFormat, this))
    , settings_actor(new QSettings(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/characters_config/" + settings->value("actor/name").toString() + "/config.ini", QSettings::IniFormat))
    , chathistory(new QSettings(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/characters_config/" + settings->value("actor/name").toString() + "/history.ini", QSettings::IniFormat))
{
    qInfo()<<"初始化galgamedialog……";
    ui->setupUi(this);
    /*窗口初始化*/
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setWindowOpacity(0.95);
    setAttribute(Qt::WA_TranslucentBackground);
    /*内容初始化*/
    ui->pushButton_next->hide();
    ui->label_name->setText(tr("你"));

    ui->verticalScrollBar->setVisible(false);
    // 获取 textEdit 的滚动条
    QScrollBar *scroll = ui->textEdit->verticalScrollBar();
    // 滚动范围变更时，更新 slider 的最大值和可见性
    connect(scroll, &QScrollBar::rangeChanged, this, [=](int min, int max) {
        int newMax = max / 5;  // 将 max 除以 5，调整滑块的范围
        ui->verticalScrollBar->setRange(min, newMax);
        ui->verticalScrollBar->setEnabled(max > min);  // 禁用 slider 避免可操作但无效
        ui->verticalScrollBar->setVisible(max > min);  // 可选：隐藏 slider
    });

    // 让外部滚动条控制 textEdit 的滚动条
    connect(ui->verticalScrollBar, &QScrollBar::valueChanged, this, [=](int value) {
        // 通过 value 来反向计算，并同步到 textEdit
        QScrollBar *scroll = ui->textEdit->verticalScrollBar();
        int newValue = value * 5;  // 将 value 放大，保持同步
        scroll->setValue(newValue);
    });


    /*子窗口*/
    history_win = new history(this);
    /*读取历史*/
    // 读取所有消息
    int index = 0;
    while (true)
    {
        QString key = QString("Messages/%1").arg(index);
        if (!chathistory->contains(key + "/role")) break; // 如果没有更多消息，退出循环
        json_t message;
        message["role"] = chathistory->value(key + "/role").toString().toStdString();
        message["content"] = chathistory->value(key + "/content").toString().toStdString();
        if(chathistory->value(key + "/role").toString()=="user")  history_win->addChildWindow(tr("你"),chathistory->value(key + "/content").toString());
        else if(chathistory->value(key + "/role").toString()=="assistant") history_win->addChildWindow(settings->value("/actor/name").toString(),chathistory->value(key + "/content").toString());
        llm_messages.push_back(message);
        index++;
    }
    /*逐字显示*/
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &galgamedialog::updateText);
    /*录音*/
    //初始化
    audioRecorder_record = new QMediaRecorder(this);
    captureSession.setRecorder(audioRecorder_record);
    captureSession.setAudioInput(new QAudioInput(this));
    QAudioInput *audioInput = captureSession.audioInput();
    if (audioInput)
    {
        audioInput->setDevice(QMediaDevices::defaultAudioInput());
        qInfo() << "使用的音频输入设备:" << QMediaDevices::defaultAudioInput().description();
    }
    else qWarning() << "无法初始化音频输入设备";
    //录音连接
    connect(audioRecorder_record, &QMediaRecorder::recorderStateChanged, this, [=](QMediaRecorder::RecorderState state)
            {
                if (state == QMediaRecorder::StoppedState) //如果停止录音，开始识别
                {
                    qInfo()<<"结束录音->识别";
                    QString msg = UrlpostWithFile();
                    if(msg!="")
                    {
                        ui->textEdit->setText(msg);
                        if(ui->checkBox_autoInput->isChecked())
                        {
                            qInfo() << "自动发送！";
                            send_to_llm();
                        }
                        if(settings->value("/speechInput/wake_enable").toBool()) //开启语音唤醒
                        {
                            /*结束词检查*/
                            bool containsAny = false;
                            // 遍历 list，检查 msg 是否包含任意一个子字符串
                            for (const QString &str : settings_actor->value("/speechInput/end_word").toString().split("|"))
                            {
                                if (msg.contains(str))
                                {
                                    containsAny = true; // 如果包含，设置标志为 true
                                    break; // 找到匹配后立即退出循环
                                }
                            }
                            if(containsAny)
                            {
                                qInfo() << "发现结束词";
                                ui->checkBox_autoInput->setChecked(false);
                            }
                            /*唤醒词检查*/
                            containsAny = false; // 初始化标志为 false
                            // 遍历 list，检查 msg 是否包含任意一个子字符串
                            for (const QString &str : settings_actor->value("/speechInput/wake_word").toString().split("|"))
                            {
                                if (msg.contains(str))
                                {
                                    containsAny = true; // 如果包含，设置标志为 true
                                    break; // 找到匹配后立即退出循环
                                }
                            }
                            if(containsAny)
                            {
                                qInfo() << "发现唤醒词";
                                ui->checkBox_autoInput->setChecked(true);
                                send_to_llm();
                            }
                        }
                    }
                }
            });
    /*VAD*/
    /*录音*/
    // 设置音频格式
    format.setSampleRate(16000); //设置采样率
    format.setChannelCount(1); //设置通道数
    format.setSampleFormat(QAudioFormat::Int16); //设置采样格式为 16 位整数
    //获取默认的音频输入设备
    QAudioDevice inputDevice = QMediaDevices::defaultAudioInput();
    //创建 VAD 对象
    vad = new VAD(this);
    //连接 VAD 的信号到槽函数
    connect(vad,&VAD::energy_to_main,this,[=](int energy)
            {
                emit energy_to_main(energy);
            });
    connect(vad, &VAD::voiceDetected, this, [&](bool detected)
            {
                if (detected && !is_record && !is_in_llm && settings->value("/speechInput/wake_enable").toBool()) //如果开启了语言唤醒并且有语言
                {
                    on_pushButton_input_pressed();
                    is_record = true;
                }
                else if(!detected)
                {
                    if (is_record && !is_in_llm && settings->value("/speechInput/wake_enable").toBool()) //语言唤醒结束
                    {
                        on_pushButton_input_released();
                        is_record = false;
                    }
                }
            });
    //创建音频输入对象
    audioSource = new QAudioSource(inputDevice, format, this);
    init_from_main();
    qInfo()<<"galgamedialog加载完成！";
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
        if (!keys.contains(Qt::Key_Shift)) //过滤Shift换行
            send_to_llm();
    keys.removeAll(event->key());
}




/*openai post请求*/
QString galgamedialog::UrlpostLLM_openai(std::string user,std::string system,bool msgonly)
{
    qInfo() << "发送 API openai 请求……";
    is_in_llm = true;
    QNetworkAccessManager* naManager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    // 添加用户消息到消息列表
    json_t userMessage =
        {
            {"role", "user"},
            {"content", user}  // 从 UI 获取用户输入的内容
        };

    if(!msgonly) llm_messages.push_back(userMessage); // 保存用户消息

    // 设置 URL 和请求头
    request.setUrl(QUrl(settings->value("llm/openai_url").toString()));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    request.setRawHeader("Authorization", "Bearer " + settings->value("llm/openai_key").toByteArray());
    // 构建 JSON 数据
    json_t rootObject =
        {
            {"model", settings->value("llm/openai_model").toString().toStdString()},
            {"messages", json_t::array()} // 创建一个空数组
        };
    // 添加系统消息
    json_t systemMessage =
        {
            {"role", "system"},
            {"content", system}
        };
    rootObject["messages"].push_back(systemMessage); // 将系统消息添加到数组中
    // 将所有的用户和助手消息添加到 messages 数组中

    if(!msgonly) for (const auto& message : llm_messages) rootObject["messages"].push_back(message);

    // 输出发送的 JSON 数据
    qInfo() << "发送 post 请求：" << QString::fromStdString(rootObject.dump());
    // 发起 POST 请求
    ui->textEdit->clear();
    ui->textEdit->setFocus();
    //恢复初始格式
    QFont f;
    f.setPointSize(14);
    ui->textEdit->selectAll();
    ui->textEdit->setCurrentFont(f);
    ui->textEdit->setTextColor(Qt::black);
    ui->textEdit->setTextBackgroundColor(Qt::white);
    ui->textEdit->setText("...");
    QEventLoop loop;
    QNetworkReply* reply = naManager->post(request, QByteArray::fromStdString(rootObject.dump()));
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    // 获取响应数据
    QString read = reply->readAll();
    reply->deleteLater();  // 释放内存
    qInfo() << "获取到 API 响应：" << read;
    is_in_llm = false;
    return read;
}
/*LLMpost请求*/
QString galgamedialog::UrlpostLLM()
{
    qInfo()<<"发送llmPost请求……";
    is_in_llm = true;
    QNetworkAccessManager* naManager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    //头设置
    request.setUrl(QUrl(settings->value("/llm/url").toString()+"/v1/agents/"+settings_actor->value("/llm/agent").toString()+"/messages"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    //直接解析 JSON 字符串，嵌入动态内容
    json_t rootObject =
        {
            {"messages", {
                             {
                                 {"role", "user"},
                                 {"content", ui->textEdit->toPlainText().toStdString()}
                             }
                         }}
        };
    rootObject["messages"][0]["content"] = ui->textEdit->toPlainText().toStdString();
    //输出发送的 JSON
    qInfo() << "发送post：" << QString::fromStdString(rootObject.dump());
    ui->textEdit->clear();
    ui->textEdit->setFocus();
    //恢复初始格式
    QFont f;
    f.setPointSize(14);
    ui->textEdit->selectAll();
    ui->textEdit->setCurrentFont(f);
    ui->textEdit->setTextColor(Qt::black);
    ui->textEdit->setTextBackgroundColor(Qt::white);
    ui->textEdit->setText("...");
    //获取结果
    QEventLoop loop;
    QNetworkReply* reply = naManager->post(request, QByteArray::fromStdString(rootObject.dump()));
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QString read;
    read = reply->readAll();
    reply->deleteLater(); //记得释放内存
    qInfo()<<"获取到llmPost请求结果："<<read;
    is_in_llm = false;
    return read;
}
/*语言识别post请求*/
QString galgamedialog::UrlpostWithFile()
{
    QFile *file = new QFile( QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat" + "/output.m4a");
    if(file->size()<=settings->value("/speechInput/size").toInt())
    {
        qInfo()<<file->size()<<" 文件太小，认定为噪音";
        return "";
    }
    if(settings->value("/speechInput/api").toInt()==0)
    {
        qInfo()<<"使用whisper-asr-webservice语音识别……";
        QNetworkAccessManager *manager = new QNetworkAccessManager();
        //设置请求 URL
        QUrl url(settings->value("/speechInput/url").toString()+"/asr?language=zh&output=txt");
        QNetworkRequest request(url);
        //设置请求头
        request.setRawHeader("accept", "application/json");
        //创建 QHttpMultiPart 对象
        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        //添加文件字段
        if (!file->open(QIODevice::ReadOnly))
        {
            qCritical()<<tr("语音识别-whisper-打开录音文件失败");
            return "无法打开文件!";
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
        loop.exec(); //等待请求完成
        //处理响应结果
        if (reply->error() == QNetworkReply::NoError)
        {
            QString msg = reply->readAll();
            qInfo()<<"语音识别-whisper-结果:" << msg;
            return msg;
        }
        else
        {
            qCritical()<<tr("语音识别-whisper-错误:") << reply->errorString()<<"：" << reply->readAll();
        }
        reply->deleteLater();
        return "error";
    }
    else
    {
        qInfo()<<"使用百度语音识别……";
        QNetworkAccessManager manager;
        /*获取tocken*/
        //创建请求 URL 和查询参数
        QUrl url("https://aip.baidubce.com/oauth/2.0/token");
        QUrlQuery query;
        query.addQueryItem("grant_type", "client_credentials");
        query.addQueryItem("client_id", settings->value("/speechInput/baidu_apikey").toString());
        query.addQueryItem("client_secret", settings->value("/speechInput/baidu_secretkey").toString());
        url.setQuery(query);
        //创建请求对象并设置头信息
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        //创建事件循环
        QEventLoop loop;
        //发送 POST 请求
        QNetworkReply *reply = manager.post(request, QByteArray());
        //当请求完成时，退出事件循环
        QString accessToken;
        QObject::connect(reply, &QNetworkReply::finished, [&]()
                         {
                             if (reply->error() == QNetworkReply::NoError)
                             {
                                 QByteArray response = reply->readAll();
                                 auto jsonObj = nlohmann::json::parse(response.toStdString());
                                 // 获取 access_token 并转换为 QString
                                 accessToken = QString::fromStdString(jsonObj["access_token"].get<std::string>());
                                 qInfo() << "语音识别-百度-token-获取到token:" << accessToken;
                             }
                             else
                             {
                                 qCritical()<<tr("语音识别-百度-token-请求失败")<<reply->errorString();
                                 return;
                             }
                             reply->deleteLater();
                             loop.quit(); //退出事件循环
                         });
        //进入事件循环，等待请求完成
        loop.exec();
        /*发送识别*/
        QNetworkAccessManager manager_api;
        // 读取本地音频文件为二进制数据
        QString audioFilePath =  QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat" + "/output.m4a"; // 音频文件路径
        QFile file(audioFilePath);
        if (!file.open(QIODevice::ReadOnly)) qCritical()<<tr("语音识别-百度-识别-打开文件失败") << audioFilePath;
        QByteArray audioData = file.readAll();
        file.close();
        QString resultJson;
        if (!audioData.isEmpty())
        {
            // 将音频数据进行 Base64 编码
            QString base64AudioData = audioData.toBase64();
            // 创建 JSON 请求体，严格保持原有格式
            nlohmann::json json;
            json["format"] = "m4a";                          // 音频格式
            json["rate"] = 16000;                             // 采样率
            json["channel"] = 1;                              // 单声道
            json["token"] = accessToken.toStdString();        // access_token
            json["cuid"] = QUuid::createUuid().toString().toStdString(); // 设备ID
            json["speech"] = base64AudioData.toStdString();   // 音频数据 (Base64)
            json["len"] = audioData.size();                   // 音频长度 (字节数)
            // 将 JSON 转换为 QByteArray，注意使用 json.dump() 生成紧凑格式
            QByteArray jsonData = QByteArray::fromStdString(json.dump());
            // 创建 POST 请求
            QUrl url("https://vop.baidu.com/server_api");
            QNetworkRequest request(url);
            request.setRawHeader("Content-Type", "application/json");
            request.setRawHeader("Accept", "application/json");
            // 创建事件循环
            QEventLoop loop;
            QNetworkReply *reply_api = manager_api.post(request, jsonData);
            qInfo()<<"语音识别-百度-识别-发送post";
            // 处理响应
            QObject::connect(reply_api, &QNetworkReply::finished, [&]() {
                if (reply_api->error() == QNetworkReply::NoError) {
                    resultJson = reply_api->readAll();
                    qInfo() << "语音识别-百度-识别成功：" << resultJson;
                }
                else
                {
                    qCritical() << tr("语音识别-百度-识别失败：") << reply_api->errorString();
                }
                loop.quit();
                reply_api->deleteLater();
            });
            // 启动事件循环，等待回复
            loop.exec();
        }
        else
        {
            qWarning() << "语音识别-百度-识别-音频文件为空，跳过请求";
        }
        //解析 JSON 数据
        QString result;
        try
        {
            if (resultJson.isEmpty())
            {
                qCritical() << tr("输入的 JSON 为空");
                result.clear();
            }
            nlohmann::json doc = nlohmann::json::parse(resultJson.toStdString());
            qInfo() << "解析的 JSON 内容: " << resultJson;
            if (doc.contains("result") && doc["result"].is_array() && !doc["result"].empty())
            {
                for (const auto& value : doc["result"])
                {
                    if (value.is_string()) {
                        result = QString::fromStdString(value.get<std::string>());
                        qInfo() << "语音识别-百度-识别-获取到结果: " << result;
                    }
                }
            }
            else
            {
                qWarning() << "JSON 中没有有效的 'result' 字段";
            }
        }
        catch (...)
        {
            qCritical() << tr("语音识别-百度-识别-Json解析错误: ") << resultJson;
            result.clear();
        }
        return result;
    }
}
/*Vits的get请求*/
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
//开始逐字显示
void galgamedialog::changetext(QString text)
{
    isInChangetext = true;
    timer->stop();
    fullText = text;
    currentIndex = 0;
    timer->start(settings->value("/dialog/time").toInt());
}
//逐字显示更新
void galgamedialog::updateText()
{
    if (currentIndex <= fullText.length())
        ui->textEdit->setText(fullText.left(++currentIndex));
    else
        isInChangetext = false;
    return;
}
/*对话框的移动*/
//鼠标按下事件
void galgamedialog::mousePressEvent(QMouseEvent *event)
{
    switch(event->button())
    {
    case Qt::LeftButton:
        isLeftPressDown = true;
        this->mouseGrabber(); //返回当前抓取鼠标输入的窗口
        m_movePoint = event->globalPosition().toPoint() - this->frameGeometry().topLeft();
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
    if(isLeftPressDown) //没有按下左键时
    {
        move(event->globalPosition().toPoint() - m_movePoint);
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
void galgamedialog::on_pushButton_next_clicked()
{
    if(isInChangetext)
    {
        timer->stop();
        ui->textEdit->setText(fullText);
        isInChangetext = false;
    }
    else
    {
        ui->label_name->setText(tr("你"));
        ui->textEdit->clear();
        ui->textEdit->setEnabled(true);
        ui->pushButton_next->hide();
    }
}
/*圆角边框*/
void galgamedialog::paintEvent(QPaintEvent *event)
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
        //使用圆角矩形而不是普通矩形绘制阴影
        QRectF shadowRect((5 - i), (5 - i) , this->width() - (5 - i) * 2, this->height() - (5 - i) * 2);
        shadowPath.addRoundedRect(shadowRect, 15, 15);  // 添加圆角矩形路径
        //增加透明度效果，模拟阴影逐渐变淡
        color.setAlpha(50 - qSqrt(i) * 22);
        painter.setPen(color);
        painter.drawPath(shadowPath);  // 绘制阴影路径
    }
}
/*录音相关*/
void galgamedialog::on_pushButton_input_pressed()
{
    qInfo()<<"开始录音";
    if(settings->value("/speechInput/interrupt").toBool()) player->stop();
    if (audioRecorder_record->recorderState() == QMediaRecorder::StoppedState)
    {
        QMediaFormat format;
        format.setAudioCodec(QMediaFormat::AudioCodec::AAC); //对应编码器
        audioRecorder_record->setMediaFormat(format);
        audioRecorder_record->setAudioSampleRate(44100); //设置采样率
        audioRecorder_record->setAudioChannelCount(2); // 设置声道数
        audioRecorder_record->setQuality(QMediaRecorder::HighQuality); //设置录制质量
        audioRecorder_record->setOutputLocation(QUrl::fromLocalFile( QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat" + "/output.m4a"));
        audioRecorder_record->record();
    }
}
void galgamedialog::on_pushButton_input_released()
{
    qInfo()<<"结束录音";
    if (audioRecorder_record) audioRecorder_record->stop();
}
/*初始化*/
void galgamedialog::init_from_main()
{
    if(!settings->value("/speechInput/enable").toBool()) //不开启语言输入
    {
        qInfo()<<"不使用语音输入";
        ui->pushButton_input->hide();
        ui->pushButton_input->setEnabled(false);
        ui->checkBox_autoInput->hide();
    }
    else
    {
        qInfo()<<"使用语言输入";
        ui->pushButton_input->show();
        ui->pushButton_input->setEnabled(true);
        ui->checkBox_autoInput->show();
    }
    // 设置窗口和子控件缩放比例
    qreal scaleFactor = settings->value("/dialog/size").toDouble()/100;
    // 调整窗口大小
    this->resize(650*scaleFactor,200*scaleFactor);

    // VAD 处理
    if (settings->value("/speechInput/wake_enable").toBool())
    {
        qInfo() << "启用VAD";
        audioDevice = audioSource->start();
        connect(audioDevice, &QIODevice::readyRead, this, [=]()
                {
                    QByteArray audioData = audioDevice->readAll();
                    vad->processAudio(audioData, format);
                });
    }
    else
    {
        qInfo() << "不使用 VAD";
        audioSource->stop(); // 停止音频源
        disconnect(audioDevice, &QIODevice::readyRead, nullptr, nullptr);  // 断开信号
    }
}


std::string galgamedialog::getOpenAiFeedbackContant(QString orig,bool msgonly)
{
    try
    {
        // 解析 JSON 响应
        json_t responseJson = json_t::parse(orig.toStdString());
        // 输出整个解析后的 JSON
        qInfo() << "解析后的 JSON：" << QString::fromStdString(responseJson.dump());
        // 检查是否有错误信息
        if (responseJson.contains("error")) {
            // 处理错误信息
            std::string errorMessage = responseJson["error"]["message"];
            qCritical() << "API 错误：" << QString::fromStdString(errorMessage);
        }
        // 访问嵌套的内容 "content"
        std::string content = responseJson["choices"][0]["message"]["content"];
        // 打印内容
        qInfo() << "模型回复内容：" << QString::fromStdString(content);
        if(!msgonly)
        {
            json_t userMessage = {
                {"role", "assistant"},
                {"content", content}  // 从 UI 获取用户输入的内容
            };
            llm_messages.push_back(userMessage); // 保存用户消息
            for (int i = 0; i < llm_messages.size(); ++i)
            {
                QString key = QString("Messages/%1").arg(i);
                chathistory->setValue(key + "/role", QString::fromStdString(llm_messages[i]["role"]));
                chathistory->setValue(key + "/content", QString::fromStdString(llm_messages[i]["content"]));
            }
        }
        return content;
    }
    catch (const nlohmann::json::parse_error& e) {
        // 捕获并输出解析错误
        qCritical() << "解析 JSON 错误：" << e.what();
    }
    catch (const std::exception& e) {
        // 捕获其他异常
        qCritical() << "发生错误：" << e.what();
    }
}

//发送llm消息
void galgamedialog::send_to_llm()
{
    history_win->addChildWindow(tr("你"),ui->textEdit->toPlainText());
    //对话框设置
    ui->label_name->setText(settings->value("/actor/name").toString());
    ui->textEdit->setEnabled(false);
    ui->pushButton_next->hide();
    //去除换行
    QTextCursor cursor=ui->textEdit->textCursor(); //得到当前text的光标
    if(cursor.hasSelection()) cursor.clearSelection();
    cursor.deletePreviousChar(); //删除前一个字符
    QString message="";
    //发送post请求
    if(settings_actor->value("llm/llm").toInt()==0)
    {
        QString result_from_llm = UrlpostLLM();
        nlohmann::json jsonDoc = nlohmann::json::parse(result_from_llm.toStdString(), nullptr, false);
        qInfo() << "接收到post信息：" << QString::fromStdString(jsonDoc.dump());
        //获取到的json处理
        message = "正常|[error] 错误，请检查letta的报错日志，返回值为[" + QString::fromStdString(jsonDoc.dump()) + "]|错误error";
        //解析 JSON 字符串
        if (jsonDoc.is_discarded()) qCritical() << "letta返回值解析失败";
        else if (jsonDoc.contains("messages") && jsonDoc["messages"].is_array()) // 查找 "content" 字段
        {
            for (const auto& value : jsonDoc["messages"])
            {
                if (value.contains("content"))
                {
                    message = QString::fromStdString(value["content"].get<std::string>());
                    break; //找到第一个匹配的 content 字段后退出
                }
            }
        }
    }
    else
    {
        message = "正常|[error] opeanai请求错误 |错误error";
        QString result_from_llm;
        if(settings_actor->value("llm/threetime").toBool())
        {
            qInfo("格式增强已开启");
            result_from_llm = QString::fromStdString(getOpenAiFeedbackContant(UrlpostLLM_openai(ui->textEdit->toPlainText().toStdString(),settings_actor->value("llm/prompt").toString().toStdString(),false),true));
            qInfo()<<"对话返回："<<result_from_llm;
            QString emo = QString::fromStdString(getOpenAiFeedbackContant(UrlpostLLM_openai("",(QString::fromStdString("在说这句话的时候是什么心情？,请在以下选项中选择一个:开心，伤心;只输出结果;")+result_from_llm).toStdString(),true),true));
            qInfo()<<"心情："<<emo;
            QString ja= QString::fromStdString(getOpenAiFeedbackContant(UrlpostLLM_openai("",(QString::fromStdString("翻译成日语,只输出结果:")+result_from_llm).toStdString(),true),true));
            qInfo()<<"日语："<<ja;
            result_from_llm = emo + "|" + result_from_llm +  + "|" + ja;
            qInfo()<<"最后输出内容："<<result_from_llm;
            message = result_from_llm;
        }
        else
        {
            qInfo("格式增强未开启");
            result_from_llm = UrlpostLLM_openai(ui->textEdit->toPlainText().toStdString(),settings_actor->value("llm/prompt").toString().toStdString(),false);
            message = QString::fromStdString(getOpenAiFeedbackContant(result_from_llm,false));
        }
    }

    // 创建正则表达式，匹配 <think> 到 </think> 之间的内容（非贪婪）
    QRegularExpression regex("<think>.*?</think>", QRegularExpression::DotMatchesEverythingOption);
    message = message.replace(regex, "");

    qInfo() << "读取到message" << message;
    //信息判断
    if(message.isNull())
    {
        message = "正常|[error] LLM请求错误|error";
    }
    else if(message.split("|").size()!=3)
    {
        if(!settings->value("/llm/feedback").toBool()) message = "正常|[error] 正常返回，但是返回值格式错误，返回值["+message+"]|错误error";
        else message = "正常|"+message+"|";
    }
    qInfo()<<"切换立绘："+message.split("|")[0];
    //语音合成
    if(settings->value("/vits/enable").toBool())
    {
        qInfo() << "正在进行语言合成";
        spawnVoice(message,false);
    }
    else
    {
        changetext(message.split("|")[1].replace(" ","")); //逐字显示
        history_win->addChildWindow(settings->value("/actor/name").toString(),message);
        ui->pushButton_next->show();
        emit change_tachie_to_tachie(message.split("|")[0]);
    }
    /*发送指令相关*/
    //读取内容
    if(message.contains("{Mute}"))
    {
        ::SendMessage((HWND)(this->winId()), WM_APPCOMMAND, 0, MAKELPARAM(0, FAPPCOMMAND_KEY << 12 | APPCOMMAND_VOLUME_MUTE));
        qInfo()<<"切换静音状态";
    }
    else if(message.contains("{Shutdown}"))
    {
        QProcess::startDetached("shutdown", QStringList() << "/s" << "/t" << "0" << "/f");
        qInfo()<<"关机";
    }
    else if(message.contains("{Next_Music}"))
    {
        INPUT input = {0};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = VK_MEDIA_NEXT_TRACK;
        SendInput(1, &input, sizeof(INPUT));
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
        qInfo()<<"下一首音乐";
    }
    else if(message.contains("{Previous_Music}"))
    {
        INPUT input = {0};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = VK_MEDIA_PREV_TRACK;  // 切换到上一首音乐
        SendInput(1, &input, sizeof(INPUT));
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
        qInfo() << "上一首音乐";
    }
    else if(message.contains("{Switch_Music_Pause}"))
    {
        INPUT input = {0};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = VK_MEDIA_PLAY_PAUSE;  // 播放/暂停切换
        SendInput(1, &input, sizeof(INPUT));
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
        qInfo() << "播放/暂停切换";
    }
    else if(message.contains("{Volume_Up}"))
    {
        INPUT input = {0};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = VK_VOLUME_UP;  // 增大音量
        SendInput(1, &input, sizeof(INPUT));
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
        qInfo() << "音量增大";
    }
    else if(message.contains("{Volume_Down}"))
    {
        INPUT input = {0};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = VK_VOLUME_DOWN;  // 减小音量
        SendInput(1, &input, sizeof(INPUT));
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
        qInfo() << "音量减小";
    }
    else if(message.contains("{Sleep}"))
    {
        SetSystemPowerState(FALSE, TRUE);  // FALSE 表示休眠，TRUE 表示强制休眠
        qInfo() << "休眠";
    }
    else if(message.contains("{OpenUrl_"))
    {
        QRegularExpression regex("\\{(.*?)\\}");
        QRegularExpressionMatch match = regex.match(message);
        if (match.hasMatch())
            QDesktopServices::openUrl(QUrl(match.captured(1).replace("OpenUrl_","")));
        qInfo() << "打开url";
    }
}
/*语言合成函数*/
void galgamedialog::spawnVoice(QString message,bool onlySound)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply* reply;
    QString text;
    //语音语言选择
    if(settings_actor->value("/vits/lan").toString() == "ja")
        text = message.split("|")[2];
    else
        text = message.split("|")[1];
    QRegularExpression regex("\\{(.*?)\\}");
    QRegularExpressionMatch match = regex.match(message);
    if (match.hasMatch())
        text = text.replace(match.captured(1),"");
    //语音api选择
    qInfo() << "语言合成内容" << text;
    if(settings_actor->value("/vits/api").toInt()==1)
    {
        reply = manager->get(QNetworkRequest(QUrl(settings_actor->value("/vits/custom_url").toString().replace("{msg}",text))));
        qInfo()<<"发送语言合成"<<settings_actor->value("/vits/custom_url").toString().replace("{msg}",text);
    }
    else
    {
        reply = manager->get(QNetworkRequest(QUrl(settings->value("/vits/url").toString()+"/voice/"+settings_actor->value("/vits/vitsmodel").toString()+"?text="+text+"&id="+settings_actor->value("/vits/id").toString()+"&format=mp3"+"&lang="+settings_actor->value("/vits/lan").toString())));
        qInfo()<<"发送语言合成"<<settings->value("/vits/url").toString()+"/voice/"+settings_actor->value("/vits/vitsmodel").toString()+"?text="+text+"&id="+settings_actor->value("/vits/id").toString()+"&format=mp3"+"&lang="+settings_actor->value("/vits/lan").toString();
    }
    //播放返回值
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError)
        {
            if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
            {
                QFile outputFile(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/temp.mp3");
                if (outputFile.open(QIODevice::WriteOnly))
                {
                    outputFile.write(reply->readAll());
                    outputFile.close();
                }
                // 停止并释放之前的播放资源
                if (player)
                {
                    player->stop();
                    player->setSource(QUrl()); // 清空源文件
                }
                QAudioOutput *audioOutput = new QAudioOutput; //创建 QAudioOutput 对象来控制音量
                player->setAudioOutput(audioOutput); //将 QAudioOutput 连接到 QMediaPlayer
                player->setSource(QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/temp.mp3")); // 设置媒体源文件
                audioOutput->setVolume(1); //0.0 为最小音量，1.0 为最大音量
                player->setPosition(0);
                player->play(); //播放音频
                if(!onlySound)
                {
                    qInfo() << "语言合成播放完成";
                    changetext(message.split("|")[1].replace(" ","")); //逐字显示
                    history_win->addChildWindow(settings->value("/actor/name").toString(),message);
                    ui->pushButton_next->show();
                    emit change_tachie_to_tachie(message.split("|")[0]);
                }
                else
                {
                    qInfo() << "语言重放完成" << text;
                }
            }
        }
        else
        {
            if(!onlySound)
            {
                ui->textEdit->setText(tr("[error] Vits错误，请检查Vits配置或者关闭语言输出"));
                qCritical()<<tr("Vits错误，请检查Vits配置或者关闭语言输出");
                ui->pushButton_next->show();
            }
            else
            {
                qCritical() << "语言重放失败" << text;
            }
        }
    });
}
/*历史按钮*/
void galgamedialog::on_pushButton_history_clicked()
{
    history_win->move(this->x(), this->y() -450); // 确保子窗口出现在父窗口旁边
    if (!isHistoryOpen)
    {
        //显示窗口
        history_win->show();
        isHistoryOpen = true;
        qInfo() << "打开日志窗口……";
        //设置窗口透明度效果
        auto *opacityEffect = new QGraphicsOpacityEffect(history_win);
        history_win->setGraphicsEffect(opacityEffect);
        //获取窗口初始和目标位置
        QRect startRect = history_win->geometry();
        QRect endRect = startRect;
        startRect.moveTop(startRect.top() + 20); // 初始位置向下偏移 100 像素
        //设置窗口起始位置（下方 + 透明）
        history_win->setGeometry(startRect);
        opacityEffect->setOpacity(0.0);
        //创建透明度动画（淡入）
        auto *opacityAnim = new QPropertyAnimation(opacityEffect, "opacity");
        opacityAnim->setDuration(150);
        opacityAnim->setStartValue(0.0);
        opacityAnim->setEndValue(1.0);
        //创建位置动画（向上移动）
        auto *moveAnim = new QPropertyAnimation(history_win, "geometry");
        moveAnim->setDuration(150);
        moveAnim->setStartValue(startRect);
        moveAnim->setEndValue(endRect);
        //并行动画组，确保两种动画同时进行
        auto *group = new QParallelAnimationGroup(history_win);
        group->addAnimation(opacityAnim);
        group->addAnimation(moveAnim);
        //防止动画被回收
        group->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        isHistoryOpen = false;
        qInfo() << "关闭日志窗口……";
        //获取窗口的当前位置和目标位置
        QRect startRect = history_win->geometry();
        QRect endRect = startRect;
        endRect.moveTop(endRect.top() + 20); //目标位置向上偏移 100 像素
        //确保窗口可见且透明度效果已设置
        QGraphicsOpacityEffect *opacityEffect = qobject_cast<QGraphicsOpacityEffect *>(history_win->graphicsEffect());
        if (!opacityEffect)
        {
            opacityEffect = new QGraphicsOpacityEffect(history_win);
            history_win->setGraphicsEffect(opacityEffect);
        }
        //创建透明度动画（淡出）
        QPropertyAnimation *opacityAnim = new QPropertyAnimation(opacityEffect, "opacity");
        opacityAnim->setDuration(150); //动画时间
        opacityAnim->setStartValue(1.0); //从完全不透明
        opacityAnim->setEndValue(0.0); //到完全透明
        //创建位置动画（向上移动）
        QPropertyAnimation *moveAnim = new QPropertyAnimation(history_win, "geometry");
        moveAnim->setDuration(150); //动画时间 1 秒
        moveAnim->setStartValue(startRect); //从当前窗口位置
        moveAnim->setEndValue(endRect); //向上移动 100 像素
        //并行动画组（确保透明度和位置同时变化）
        QParallelAnimationGroup *group = new QParallelAnimationGroup(history_win);
        group->addAnimation(opacityAnim);
        group->addAnimation(moveAnim);
        //动画完成后隐藏窗口
        connect(group, &QParallelAnimationGroup::finished, history_win, &QWidget::hide);
        //启动动画，确保动画执行完再释放内存
        group->start(QAbstractAnimation::DeleteWhenStopped);
    }
}
//历史界面跟随galdialog
void galgamedialog::moveEvent(QMoveEvent *event)
{
    if (history_win && history_win->isVisible())
    {
        QPoint offset = event->pos() - lastPos; //计算偏移量
        history_win->move(history_win->pos() + offset);
    }
    lastPos = event->pos();  // 更新主窗口位置
}
//滚轮打开历史界面
void galgamedialog::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0)
        handleWheelUp(); //向上滚轮
    else if (event->angleDelta().y() < 0)
        handleWheelDown(); //向下滚轮
}
// 向上滚轮处理
void galgamedialog::handleWheelUp()
{
    if(!isHistoryOpen) ui->pushButton_history->click();
}
// 向下滚轮处理
void galgamedialog::handleWheelDown()
{
    if(isHistoryOpen) ui->pushButton_history->click();
}
//重载窗口
void galgamedialog::refreshUI()
{
    ui->retranslateUi(this);
}
