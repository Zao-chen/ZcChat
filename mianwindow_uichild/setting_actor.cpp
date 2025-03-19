#include "setting_actor.h"
#include "ui_setting_actor.h"
#include "../mainwindow.h"

QSettings settings("Setting.ini", QSettings::IniFormat);


setting_actor::setting_actor(QWidget *parent)
    : QWidget(parent)
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
void setting_actor::on_spinBox_tachie_size_valueChanged(int arg1)
{
    mainWin->ChangeSetting_tachieSize(arg1);
}
void setting_actor::on_lineEdit_llm_agent_textChanged(const QString &arg1)
{
    QScopedPointer<QSettings> settings_actor(new QSettings(qApp->applicationDirPath()+"/characters/"+settings.value("actor/name").toString()+"/config.ini", QSettings::IniFormat)); //使用 QScopedPointer来自动管理资源。

    settings_actor->setValue("/llm/agent",arg1);
    qDebug()<<qApp->applicationDirPath()+"/characters/"+settings.value("actor/name").toString()+"/config.ini";
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
    QScopedPointer<QSettings> settings_actor(new QSettings(qApp->applicationDirPath()+"/characters/"+settings.value("actor/name").toString()+"/config.ini", QSettings::IniFormat)); //使用 QScopedPointer来自动管理资源。
    settings_actor->setValue("/vits/id",arg1);
}
void setting_actor::on_lineEdit_vits_customUrl_textChanged(const QString &arg1)
{
    QScopedPointer<QSettings> settings_actor(new QSettings(qApp->applicationDirPath()+"/characters/"+settings.value("actor/name").toString()+"/config.ini", QSettings::IniFormat)); //使用 QScopedPointer来自动管理资源。
    settings_actor->setValue("/vits/custom_url",arg1);
}
void setting_actor::on_comboBox_vits_language_currentTextChanged(const QString &arg1)
{
    mainWin->ChangeSetting_VitsLanguage(arg1);
}
void setting_actor::on_lineEdit_speechInput_wakeWord_textChanged(const QString &arg1)
{
    QScopedPointer<QSettings> settings_actor(new QSettings(qApp->applicationDirPath()+"/characters/"+settings.value("actor/name").toString()+"/config.ini", QSettings::IniFormat)); //使用 QScopedPointer来自动管理资源。
    settings_actor->setValue("/speechInput/wake_word",arg1);
}
void setting_actor::on_lineEdit_speechInput_endWord_textChanged(const QString &arg1)
{
    QScopedPointer<QSettings> settings_actor(new QSettings(qApp->applicationDirPath()+"/characters/"+settings.value("actor/name").toString()+"/config.ini", QSettings::IniFormat)); //使用 QScopedPointer来自动管理资源。
    settings_actor->setValue("/speechInput/end_word",arg1);
}

//调用installTranslator后，系统会给窗体发送信号将产生了changeEvent槽产生event
void setting_actor::changeEvent(QEvent *e)
{
    qDebug()<<"22";
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        qDebug()<<"change";
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
