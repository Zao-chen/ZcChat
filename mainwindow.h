#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "galgamedialog.h"
#include "tachie.h"
#include "ElaWindow.h"
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public ElaWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void playAudioFromUrl(const QUrl &url);
    galgamedialog *dialog_win;
    tachie *tachie_win;
    ~MainWindow();

private slots:
    void on_checkBox_2_clicked(bool checked);
    void show_dialogwin_from_tachie();

    void on_checkBox_3_clicked(bool checked);

    void on_treeView_up_clicked(const QModelIndex &index);
    void on_showMainAction();
    void on_exitAppAction();    void createMenu();
    void hideWindow();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *m_sysTrayIcon; //系统托盘
    QMenu *m_menu;                  //菜单
    QAction *m_showMainAction;      //动作
    QAction *m_exitAppAction;       //动作
    void createActions();
protected:
    void closeEvent (QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
