#ifndef TACHIE_H
#define TACHIE_H

#include <QWidget>

namespace Ui {
class tachie;
}

class tachie : public QWidget
{
    Q_OBJECT
signals:
    void show_dialogwin_to_main();
public:
    explicit tachie(QWidget *parent = nullptr);
    ~tachie();

private:
    Ui::tachie *ui;
    //鼠标按下移动及释放事件
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    QPoint m_movePoint; //鼠标的位置
    bool isLeftPressDown; // 判断左键是否按下

private slots:
    void changetachie_from_galdialog(QString name);
    void init_from_main();
};

#endif // TACHIE_H
