#ifndef MESSAGEWINDOW_H
#define MESSAGEWINDOW_H

#include <QDialog>

namespace Ui {
class messagewindow;
}

class messagewindow : public QDialog
{
    Q_OBJECT

public:
    explicit messagewindow(const QString &message,QWidget *parent = nullptr);
    ~messagewindow();

private:
    void showModal(const QString &message);
    Ui::messagewindow *ui;
};

#endif // MESSAGEWINDOW_H
