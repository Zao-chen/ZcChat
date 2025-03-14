#ifndef SETTING_GENERAL_H
#define SETTING_GENERAL_H

#include <QWidget>

class MainWindow;

namespace Ui {
class setting_general;
}

class setting_general : public QWidget
{
    Q_OBJECT

public:
    explicit setting_general(QWidget *parent = nullptr);
    ~setting_general();

private slots:
    void on_comboBox_actor_currentTextChanged(const QString &arg1);
    void on_checkBox_autostart_clicked(bool checked);
    void on_checkBox_autoopen_clicked(bool checked);
    void on_spinBox_dialogtime_valueChanged(int arg1);

    void on_spinBox_dialogscale_valueChanged(int arg1);

private:
    Ui::setting_general *ui;
    MainWindow *mainWin;
};

#endif // SETTING_GENERAL_H
