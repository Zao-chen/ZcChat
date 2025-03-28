#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "galgamedialog.h"
#include "tachie.h"
#include "mianwindow_uichild/setting_general.h"
#include "mianwindow_uichild/setting_ai.h"
#include "mianwindow_uichild/setting_vits.h"
#include "mianwindow_uichild/setting_voiceinput.h"
#include "mianwindow_uichild/setting_actor.h"
#include "mianwindow_uichild/about.h"
#include "ElaWindow.h"
#include <QSystemTrayIcon>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public ElaWindow
{
    Q_OBJECT

signals:
    void resetlocation_to_tachie();

public:
    MainWindow(QWidget *parent = nullptr);
    QString local_version = "v4.6.0";
    /*菜单窗口创建*/
    setting_general *setting_general_win;
    setting_ai *setting_ai_win;
    setting_vits *setting_vits_win;
    setting_voiceinput *setting_voiceinput_win;
    setting_actor *setting_actor_win;
    about *about_win;
    galgamedialog *dialog_win;
    tachie *tachie_win;
    /*设置时候的一些传递*/
    //通用设置
    void ChangeSetting_ActorChoose(const QString &arg1);
    void ChangeSetting_AutoStart(bool checked);
    void ChangeSetting_Lan(const QString &arg1);
    //语音输入设置
    void ChangeSetting_speechInputAPI(int index);
    void ChangSetting_speechInputWake(bool checked);
    //角色设置
    void ChangeSetting_tachieSize(int arg1);
    void ChangeSetting_dialogSize(int arg1);
    void ChangeSetting_VitsAPI(int index);
    void ChangeSetting_VitsModel(const QString &arg1);
    void ChangeSetting_VitsLanguage(const QString &arg1);
    /*一些函数*/
    bool already_init=false; //是否已经初始化了
    void resetTachie();
    QString reply;
    QString tagName;
    ~MainWindow();

private slots:
    /*信号的一些传递*/
    void show_dialogwin_from_tachie(); //右键立绘显示对话框
    void changeTachieLocation_from_tachie(int x,int y); //立绘移动后显示
    void getEnergy_from_gal(int energy);
    /*托盘*/
    void on_showMainAction();
    void on_exitAppAction();
    void on_restartAppAction();
    void on_pinTachie();
    void hideWindow();

signals:
    void init_to_tachie(bool pin); //初始化立绘
    void init_to_dialog(); //初始化对话框

private:
    Ui::MainWindow *ui;
    /*托盘相关*/
    QSystemTrayIcon *m_sysTrayIcon; //系统托盘
    QMenu *m_menu; //菜单
    QAction *m_showMainAction; //托盘动作
    QAction *m_exitAppAction; //托盘动作
    QAction *m_restartAppAction; //托盘动作
    QAction *m_pinTachie; //托盘动作
    /*网络相关*/
    QByteArray getUrl(const QString &input); //网络
    QNetworkAccessManager *m_manager;
    /*保存配置的函数*/
    void saveSetting(const QString &key, const QVariant &value);
    void saveActorSetting(const QString &key, const QVariant &value);
    /*一些函数*/
    void reloadActorSetting();
    /*能量显示*/
    QTimer *energyTimer; // 定时器
    int currentEnergy;   // 当前能量值
    void updateEnergyDisplay();
    bool isDialogOpen = false; //用于切换对话框显示和隐藏
    /*一些变量*/
    bool isPin = true; //是否置顶
};
#endif // MAINWINDOW_H
