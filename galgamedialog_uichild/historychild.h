#ifndef HISTORYCHILD_H
#define HISTORYCHILD_H

#include <QWidget>

class galgamedialog;

namespace Ui {
class historychild;
}

class historychild : public QWidget
{
    Q_OBJECT

public:
    explicit historychild(const QString &name,const QString &msg,QWidget *parent = nullptr);
    ~historychild();

private slots:
    void on_pushButton_clicked(bool checked);

private:
    Ui::historychild *ui;
    QString msg_l;
    galgamedialog *galWin;
};

#endif // HISTORYCHILD_H
