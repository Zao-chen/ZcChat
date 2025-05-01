#ifndef SETTING_ACTOR_H
#define SETTING_ACTOR_H

#include <QWidget>
#include <QSettings>
#include <QStandardPaths>

class MainWindow;

namespace Ui {
class setting_actor;
}

class setting_actor : public QWidget
{
    Q_OBJECT

public:
    explicit setting_actor(QWidget *parent = nullptr);
    void refreshUI(); //重载窗口
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
    void on_pushButton_resettachie_clicked();

    void on_comboBox_ai_api_currentIndexChanged(int index);

    void on_textEdit_OpenaiPrompt_textChanged();

    void on_pushButton_viewhistory_clicked();

    void on_pushButton_resethistory_clicked();

private:
    QSettings *settings;
    QSettings *settings_actor;
    Ui::setting_actor *ui;
    MainWindow *mainWin;
};

#endif // SETTING_ACTOR_H
