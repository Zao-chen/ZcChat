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

private slots:
    void on_pushButton_copy_clicked();
    void on_pushButton_open_clicked();

private:
    void showModal(const QString &message);
    Ui::messagewindow *ui;
};

#endif // MESSAGEWINDOW_H
