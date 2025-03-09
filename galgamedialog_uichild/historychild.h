#ifndef HISTORYCHILD_H
#define HISTORYCHILD_H

#include <QWidget>

namespace Ui {
class historychild;
}

class historychild : public QWidget
{
    Q_OBJECT

public:
    explicit historychild(const QString &name,const QString &msg,QWidget *parent = nullptr);
    ~historychild();

private:
    Ui::historychild *ui;
};

#endif // HISTORYCHILD_H
