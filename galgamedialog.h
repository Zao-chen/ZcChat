#ifndef GALGAMEDIALOG_H
#define GALGAMEDIALOG_H

#include <QNetworkAccessManager>
#include <QMediaPlayer>
#include <QWidget>

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
    void on_pushButton_clicked();
    void updateText(); //逐字显示-更新
signals:
    void change_tachie_to_tachie(QString name);
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
    QNetworkAccessManager *m_manager;
    QString Urlpost();
    QByteArray getUrl(const QString &input);
    //逐字显示
    QTimer *timer;
    QString fullText;
    int currentIndex;
    void changetext(QString text);
    virtual void paintEvent(QPaintEvent *event) override; //重绘,
};

#endif // GALGAMEDIALOG_H
