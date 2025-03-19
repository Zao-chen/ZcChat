#ifndef SETTING_ACTOR_H
#define SETTING_ACTOR_H

#include <QWidget>

class MainWindow;

namespace Ui {
class setting_actor;
}

class setting_actor : public QWidget
{
    Q_OBJECT

public:
    explicit setting_actor(QWidget *parent = nullptr);
    ~setting_actor();

private slots:
    void on_spinBox_tachie_size_valueChanged(int arg1);
    void on_lineEdit_llm_agent_textChanged(const QString &arg1);
    void on_comboBox_vits_api_currentIndexChanged(int index);
    void on_comboBox_vits_model_currentTextChanged(const QString &arg1);
    void on_lineEdit_vits_id_textChanged(const QString &arg1);
    void on_lineEdit_vits_customUrl_textChanged(const QString &arg1);
    void on_comboBox_vits_language_currentTextChanged(const QString &arg1);
    void on_lineEdit_speechInput_wakeWord_textChanged(const QString &arg1);
    void on_lineEdit_speechInput_endWord_textChanged(const QString &arg1);
    void changeEvent(QEvent *e);//changeEvent槽函数声明

private:
    Ui::setting_actor *ui;
    MainWindow *mainWin;
};

#endif // SETTING_ACTOR_H
