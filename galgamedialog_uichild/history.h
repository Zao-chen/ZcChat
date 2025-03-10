#ifndef HISTORY_H
#define HISTORY_H

#include <QWidget>

#include <QVBoxLayout>

namespace Ui {
class history;
}

class history : public QWidget
{
    Q_OBJECT

public:
    explicit history(QWidget *parent = nullptr);
    void addChildWindow(QString name,QString msg);  // 供外部调用的接口
    ~history();

private:
    Ui::history *ui;
    QVBoxLayout *scrollLayout;         // 子窗口布局
    /*重绘*/
    virtual void paintEvent(QPaintEvent *event) override;
};

#endif // HISTORY_H
