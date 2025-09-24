#ifndef SETTING_ACTOR_INTERFACE_H
#define SETTING_ACTOR_INTERFACE_H

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
    void reloadActorSettings(); //重载设置
    ~setting_actor();

private slots:
    void on_lineEdit_llm_agent_textChanged(const QString &arg1);
    void on_comboBox_vits_api_currentIndexChanged(int index);
    void on_comboBox_vits_model_currentTextChanged(const QString &arg1);
    void on_lineEdit_vits_id_textChanged(const QString &arg1);
    void on_lineEdit_vits_customUrl_textChanged(const QString &arg1);
    void on_comboBox_vits_language_currentTextChanged(const QString &arg1);
    void on_lineEdit_speechInput_wakeWord_textChanged(const QString &arg1);
    void on_lineEdit_speechInput_endWord_textChanged(const QString &arg1);
    void on_comboBox_ai_api_currentIndexChanged(int index);
    void on_textEdit_OpenaiPrompt_textChanged();
    void on_pushButton_viewhistory_clicked();
    void on_pushButton_resethistory_clicked();
    void on_pushButton_question_clicked();
    void on_checkBox_addthreetime_clicked(bool checked);
    void on_textEdit_OpenaiPrompt_style_textChanged();
    void on_lineEdit_vits_Prompt_emo_textChanged(const QString &arg1);
    void on_lineEdit_vits_Prompt_la_textChanged(const QString &arg1);

private:
    QSettings *settings;
    QSettings *settings_charactersConfig_config;
    QSettings *settings_characters_config;
    Ui::setting_actor *ui;
    MainWindow *mainWin;
};

#endif // SETTING_ACTOR_INTERFACE_H
