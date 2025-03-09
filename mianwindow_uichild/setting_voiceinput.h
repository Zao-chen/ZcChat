#ifndef SETTING_VOICEINPUT_H
#define SETTING_VOICEINPUT_H

#include <QWidget>

class MainWindow;

namespace Ui {
class setting_voiceinput;
}

class setting_voiceinput : public QWidget
{
    Q_OBJECT

public:
    explicit setting_voiceinput(QWidget *parent = nullptr);
    ~setting_voiceinput();

private slots:
    void on_comboBox_api_currentIndexChanged(int index);

    void on_lineEdit_url_textChanged(const QString &arg1);

    void on_checkBox_enable_clicked(bool checked);

    void on_lineEdit_baiduapikey_textChanged(const QString &arg1);

    void on_lineEdit_baidusecretkey_textChanged(const QString &arg1);

    void on_checkBox_wake_clicked(bool checked);


    void on_checkBox_interrupt_clicked(bool checked);

    void on_spinBox_energy_valueChanged(int arg1);

    void on_spinBox_size_valueChanged(int arg1);

private:
    Ui::setting_voiceinput *ui;
    MainWindow *mainWin;
};

#endif // SETTING_VOICEINPUT_H
