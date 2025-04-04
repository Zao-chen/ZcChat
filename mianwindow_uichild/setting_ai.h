#ifndef SETTING_AI_H
#define SETTING_AI_H

#include <QWidget>

class MainWindow;

namespace Ui {
class setting_ai;
}

class setting_ai : public QWidget
{
    Q_OBJECT

public:
    explicit setting_ai(QWidget *parent = nullptr);
    void refreshUI(); //重载窗口
    ~setting_ai();

private slots:
    void on_lineEdit_url_textChanged(const QString &arg1);

    void on_checkBox_feedback_clicked(bool checked);

    void on_lineEdit_openai_url_textChanged(const QString &arg1);


    void on_lineEdit_openai_key_textChanged(const QString &arg1);

    void on_lineEdit_openai_model_textChanged(const QString &arg1);

private:
    Ui::setting_ai *ui;
    MainWindow *mainWin;
};

#endif // SETTING_AI_H
