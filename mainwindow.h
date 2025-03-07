#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "galgamedialog.h"
#include "tachie.h"
#include "ElaWidget.h"
#include <QSystemTrayIcon>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public ElaWidget
{
    Q_OBJECT
signals:
    void resetlocation_to_tachie();
public:
    MainWindow(QWidget *parent = nullptr);
    void playAudioFromUrl(const QUrl &url);
    galgamedialog *dialog_win;
    tachie *tachie_win;
    ~MainWindow();

private slots:
    //配置项修改
    void on_checkBox_tachie_enable_clicked(bool checked);
    void show_dialogwin_from_tachie();
    void changeTachieLocation_from_tachie(int x,int y);
    void on_checkBox_dialog_enable_clicked(bool checked);
    void on_treeView_up_clicked(const QModelIndex &index);
    void on_spinBox_actor_tachie_size_valueChanged(int arg1);
    void on_lineEdit_llm_url_textChanged(const QString &arg1);
    void on_lineEdit_llm_agent_textChanged(const QString &arg1);
    void on_checkBox_vits_enable_clicked(bool checked);
    void on_lineEdit_vits_url_textChanged(const QString &arg1);
    void on_lineEdit_vits_id_textChanged(const QString &arg1);
    void on_spinBox_dialog_valueChanged(int arg1);
    void on_comboBox_actor_choose_currentTextChanged(const QString &arg1);
    void on_checkBox_llm_errorfeedback_clicked(bool checked);
    void on_comboBox_vits_model_currentTextChanged(const QString &arg1);
    void on_checkBox_soft_autostart_clicked(bool checked);
    void on_comboBox_vits_API_currentIndexChanged(int index);
    void on_lineEdit_vits_customUrl_textChanged(const QString &arg1);
    void on_checkBox_speechInput_enable_clicked(bool checked);
    void on_lineEdit_speechInput_url_textChanged(const QString &arg1);
    void on_comboBox_vits_language_currentTextChanged(const QString &arg1);
    void on_comboBox_speechInput_API_currentIndexChanged(int index);
    void on_lineEdit_speechInput_BaiduAPIKey_textChanged(const QString &arg1);
    void on_lineEdit_speechInput_BaiduSecretKey_textChanged(const QString &arg1);
    void on_checkBox_speechInput_wake_clicked(bool checked);
    void on_lineEdit_speechInput_url_wakeWord_textChanged(const QString &arg1);
    void on_lineEdit_speechInput_url_endWord_textChanged(const QString &arg1);
    void on_spinBox_energy_valueChanged(int arg1);
    void on_spinBox_size_valueChanged(int arg1);
    void getEnergy_from_gal(int energy);
    void on_checkBox_speechInput_interrupt_clicked(bool checked);
    void on_checkBox_soft_autoOpen_clicked(bool checked);
    //托盘slot
    void on_showMainAction();
    void on_exitAppAction();
    void on_openGithub();
    void on_restartAppAction();
    void hideWindow();
    //按钮
    void on_pushButton_reset_clicked();


signals:
    void init_to_tachie(); //初始化立绘
    void init_to_dialog(); //初始化对话框

private:
    Ui::MainWindow *ui;
    /*托盘相关*/
    QSystemTrayIcon *m_sysTrayIcon; //系统托盘
    QMenu *m_menu; //菜单
    QAction *m_showMainAction; //托盘动作
    QAction *m_exitAppAction; //托盘动作
    QAction *m_restartAppAction; //托盘动作
    QAction *m_openGithub; //托盘动作
    bool already_init=false; //是否已经初始化了
    /*网络相关*/
    QByteArray getUrl(const QString &input); //网络
    QNetworkAccessManager *m_manager;

    void saveSetting(const QString &key, const QVariant &value);
    void saveActorSetting(const QString &key, const QVariant &value);
    void checkForUpdates();
    void reloadActorSetting();
    /*能量显示*/
    QTimer *energyTimer; // 定时器
    int currentEnergy;   // 当前能量值
    void updateEnergyDisplay();
};
#endif // MAINWINDOW_H
