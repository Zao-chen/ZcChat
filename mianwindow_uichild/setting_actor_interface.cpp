#include "setting_actor_interface.h"
#include "ui_setting_actor_interface.h"
#include "../mainwindow.h"
#include <QStandardPaths>
#include <QPlainTextEdit>
#include <QDir>
#include <QDesktopServices>
#include <QProcess>

setting_actor::setting_actor(QWidget *parent)
    : QWidget(parent)
    , settings(new QSettings(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/Setting.ini", QSettings::IniFormat, this))
    , settings_actor(new QSettings(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/characters_config/" + settings->value("actor/name").toString() + "/config.ini", QSettings::IniFormat))

    , ui(new Ui::setting_actor)
{
    ui->setupUi(this);
    mainWin = qobject_cast<MainWindow *>(this->parent());
}

setting_actor::~setting_actor()
{
    delete ui;
}

//设置修改
void setting_actor::on_lineEdit_llm_agent_textChanged(const QString &arg1)
{
    settings_actor->setValue("/llm/agent",arg1);
}
void setting_actor::on_comboBox_vits_api_currentIndexChanged(int index)
{
    mainWin->ChangeSetting_VitsAPI(index);
    ui->stackedWidget_vits->setCurrentIndex(index);
}
void setting_actor::on_comboBox_vits_model_currentTextChanged(const QString &arg1)
{
    mainWin->ChangeSetting_VitsModel(arg1);
}
void setting_actor::on_lineEdit_vits_id_textChanged(const QString &arg1)
{
    settings_actor->setValue("/vits/id",arg1);
}
void setting_actor::on_lineEdit_vits_customUrl_textChanged(const QString &arg1)
{
    settings_actor->setValue("/vits/custom_url",arg1);
}
void setting_actor::on_comboBox_vits_language_currentTextChanged(const QString &arg1)
{
    mainWin->ChangeSetting_VitsLanguage(arg1);
}
void setting_actor::on_lineEdit_speechInput_wakeWord_textChanged(const QString &arg1)
{
    settings_actor->setValue("/speechInput/wake_word",arg1);
}
void setting_actor::on_lineEdit_speechInput_endWord_textChanged(const QString &arg1)
{
    settings_actor->setValue("/speechInput/end_word",arg1);
}
//重载窗口
void setting_actor::refreshUI()
{
    ui->retranslateUi(this);
}


void setting_actor::on_comboBox_ai_api_currentIndexChanged(int index)
{
    ui->stackedWidget_LLM->setCurrentIndex(index);
    settings_actor->setValue("/llm/llm",index);
}
void setting_actor::on_textEdit_OpenaiPrompt_textChanged()
{
    settings_actor->setValue("/llm/prompt",ui->textEdit_OpenaiPrompt->toPlainText());
}


void setting_actor::on_pushButton_viewhistory_clicked()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +"/ZcChat/characters_config/" + settings->value("actor/name").toString() + "/history.ini";
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void setting_actor::on_pushButton_resethistory_clicked()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+ "/ZcChat/characters_config/"+ settings->value("actor/name").toString()+ "/history.ini";
    QFile::remove(path);
    QString program = QCoreApplication::applicationFilePath();
    //获取当前程序的路径
    QStringList arguments = QCoreApplication::arguments();
    //获取当前程序的启动参数
    QProcess::startDetached(program, arguments);
    //启动新进程
    QCoreApplication::quit();
    //退出当前程序
}


void setting_actor::on_pushButton_question_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/Zao-chen/ZcChat/wiki/%E8%BE%93%E5%87%BA%E6%A0%BC%E5%BC%8F%E5%A2%9E%E5%BC%BA"));
}


void setting_actor::on_checkBox_addthreetime_clicked(bool checked)
{
    settings_actor->setValue("/llm/threetime",checked);
    ui->stackedWidget_style->setCurrentIndex(checked);
}

//格式提示词
void setting_actor::on_textEdit_OpenaiPrompt_style_textChanged()
{
    settings_actor->setValue("/llm/prompt_style",ui->textEdit_OpenaiPrompt_style->toPlainText());
}
//心情提示词
void setting_actor::on_lineEdit_vits_Prompt_emo_textChanged(const QString &arg1)
{
    settings_actor->setValue("/llm/prompt_emo",arg1);
}
//翻译提示词
void setting_actor::on_lineEdit_vits_Prompt_la_textChanged(const QString &arg1)
{
    settings_actor->setValue("/llm/prompt_la",arg1);
}
//刷新配置
void setting_actor::reloadActorSettings() {
    // 先删除旧的 settings_actor
    delete settings_actor;
    settings_actor = nullptr;

    // 重新读取 actor 名称
    QString actorName = settings->value("actor/name").toString();

    // 新建 settings_actor
    settings_actor = new QSettings(
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/ZcChat/characters_config/" + actorName + "/config.ini",
        QSettings::IniFormat,
        this
        );
}
