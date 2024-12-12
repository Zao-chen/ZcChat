#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "galgamedialog.h"
#include "tachie.h"
#include "ElaWidget.h"
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public ElaWidget
{
    Q_OBJECT

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
    void on_spinBox_tachie_size_valueChanged(int arg1);
    void on_lineEdit_llm_url_textChanged(const QString &arg1);
    void on_lineEdit_llm_agent_textChanged(const QString &arg1);
    void on_checkBox_vits_enable_clicked(bool checked);
    void on_lineEdit_vits_url_textChanged(const QString &arg1);
    //void on_lineEdit_tachie_name_textChanged(const QString &arg1);
    void on_lineEdit_vits_id_textChanged(const QString &arg1);
    //托盘slot
    void on_showMainAction();
    void on_exitAppAction();
    void createMenu();
    void hideWindow();

    void on_checkBox_llm_autoopen_enable_clicked(bool checked);

    void on_lineEdit_llm_location_textChanged(const QString &arg1);

    void on_checkBox_vits_autoopen_clicked(bool checked);

    void on_lineEdit_vits_location_textChanged(const QString &arg1);

    void on_spinBox_dialog_valueChanged(int arg1);

    void on_comboBox_tachio_choose_currentTextChanged(const QString &arg1);

    void on_checkBox_llm_errorfeedback_clicked(bool checked);

    void on_comboBox_vits_model_currentTextChanged(const QString &arg1);

signals:
    void init_to_tachie(); //初始化立绘

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *m_sysTrayIcon; //系统托盘
    QMenu *m_menu; //菜单
    QAction *m_showMainAction; //托盘动作
    QAction *m_exitAppAction; //托盘动作
    void createActions();
    bool already_init=false;
};
#endif // MAINWINDOW_H
