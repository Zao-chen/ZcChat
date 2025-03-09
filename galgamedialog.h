#ifndef GALGAMEDIALOG_H
#define GALGAMEDIALOG_H

#include <QNetworkAccessManager>
#include <QMediaPlayer>
#include <QWidget>
#include <QMediaRecorder>
#include <QMediaCaptureSession>
#include "vad.h"
#include <QAudioSource>
#include <QSettings>

namespace Ui {
class galgamedialog;
}

class galgamedialog : public QWidget
{
    Q_OBJECT

public:
    explicit galgamedialog(QWidget *parent = nullptr);
    ~galgamedialog();
private slots:
    void on_pushButton_clicked(); //下一步
    void updateText(); //逐字显示-更新
    void on_pushButton_input_pressed(); //语音输入
    void on_pushButton_input_released(); //语音输入
    void init_from_main();
    void on_pushButton_log_clicked(bool checked);

signals:
    void change_tachie_to_tachie(QString name);
    void energy_to_main(int energy);


private:
    Ui::galgamedialog *ui;
    //鼠标按下移动及释放事件
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    //键盘事件
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    QList<int> keys; //按键按键获取
    QPoint m_movePoint; //鼠标的位置
    bool isLeftPressDown; // 判断左键是否按下
    //网络
    QNetworkAccessManager *m_manager;
    QString UrlpostLLM();
    QString UrlpostWithFile();
    QByteArray getUrl(const QString &input);
    //逐字显示
    QTimer *timer;
    QString fullText;
    int currentIndex;
    void changetext(QString text);
    //重绘
    virtual void paintEvent(QPaintEvent *event) override;
    //录音
    QMediaRecorder *audioRecorder = nullptr;
    QMediaCaptureSession captureSession;
    //vad
    VAD *vad; // VAD 对象
    QAudioSource *audioSource; // 音频输入对象
    QIODevice *audioDevice; // 音频设备
    QAudioFormat format; // 音频格式
    void send_to_llm();
    bool is_record = false;
    QMediaPlayer *player = new QMediaPlayer; //创建 QMediaPlayer 对象
    QSettings *settings; // 声明 settings 为成员变量
    bool is_in_llm = false;
};

#endif // GALGAMEDIALOG_H
