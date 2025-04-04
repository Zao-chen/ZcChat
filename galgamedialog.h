#ifndef GALGAMEDIALOG_H
#define GALGAMEDIALOG_H

#include <QNetworkAccessManager>
#include <QMediaPlayer>
#include <QWidget>
#include <QMediaRecorder>
#include <QMediaCaptureSession>
#include "utils/vad.h"
#include <QAudioSource>
#include <QSettings>
#include <QStandardPaths>

#include "third_party/json/json.hpp"
using json_t = nlohmann::json;

class history; //前向声明子窗口类

namespace Ui {
class galgamedialog;
}

class galgamedialog : public QWidget
{
    Q_OBJECT

public:
    explicit galgamedialog(QWidget *parent = nullptr);
    void spawnVoice(QString message,bool onlySound); //语言合成函数
    void refreshUI(); //重载窗口
    /*滚轮事件*/
    void handleWheelUp();   // 处理向上滚轮
    void handleWheelDown(); // 处理向下滚轮
    ~galgamedialog();

private slots:
    /*界面上的一些按钮*/
    void on_pushButton_next_clicked(); //下一步
    void on_pushButton_input_pressed(); //语音输入
    void on_pushButton_input_released(); //语音输入
    void on_pushButton_history_clicked(); //历史
    /*一些槽*/
    void updateText(); //逐字显示-更新
    void init_from_main(); //来自主界面的初始化请求

signals:
    void change_tachie_to_tachie(QString name); //修改立绘
    void energy_to_main(int energy); //给主界面发送能力值

protected:
    /*鼠标事件重写*/
    void moveEvent(QMoveEvent *event) override;  //捕获窗口移动事件
    void wheelEvent(QWheelEvent *event) override; //滚轮事件声明

private:
    Ui::galgamedialog *ui;
    /*鼠标按下移动及释放事件*/
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    /*键盘事件*/
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    QList<int> keys; //按键按键获取
    QPoint m_movePoint; //鼠标的位置
    bool isLeftPressDown; // 判断左键是否按下
    /*网络*/
    QNetworkAccessManager *m_manager;
    QString UrlpostLLM();
    QString UrlpostLLM_openai();
    QString UrlpostWithFile();
    QByteArray getUrl(const QString &input);
    /*逐字显示*/
    QTimer *timer;
    QString fullText;
    int currentIndex;
    void changetext(QString text);
    bool isInChangetext;
    /*重绘*/
    virtual void paintEvent(QPaintEvent *event) override;
    /*录音*/
    QMediaRecorder *audioRecorder = nullptr;
    QMediaCaptureSession captureSession;
    /*vad*/
    VAD *vad; // VAD 对象
    QAudioSource *audioSource; // 音频输入对象
    QIODevice *audioDevice; // 音频设备
    QAudioFormat format; // 音频格式
    /*一些函数*/
    void send_to_llm(); //发送llm请求
    bool is_record = false; //正在录音？
    QMediaPlayer *player = new QMediaPlayer; //创建QMediaPlayer 对象
    bool is_in_llm = false; //是否正在llm请求中
    history *history_win; //子窗口指针
    QPoint lastPos; //记录主窗口上次位置
    bool isHistoryOpen = false;//用于切换对话框显示和隐藏
    //配置文件
    QSettings *settings; //声明 settings 为成员变量
    QSettings *settings_actor;
    QSettings *chathistory; //声明 settings 为成员变量

    QList<json_t> llm_messages; // 用于存储对话消息
};
#endif // GALGAMEDIALOG_H
