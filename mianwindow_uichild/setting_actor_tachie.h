#ifndef SETTING_ACTOR_TACHIE_H
#define SETTING_ACTOR_TACHIE_H

#include <QWidget>
#include <QSettings>
#include "setting_actor_tachie_listchild.h"

class MainWindow;

namespace Ui {
class setting_actor_tachie;
}

class setting_actor_tachie : public QWidget
{
    Q_OBJECT

public:
    explicit setting_actor_tachie(QWidget *parent = nullptr);
    ~setting_actor_tachie();

private slots:
    void on_spinBox_tachie_size_valueChanged(int arg1);
    void on_pushButton_resettachie_clicked();
    void on_pushButton_loadtachie_clicked();

    void on_lineEdit_Default_textChanged(const QString &arg1);

private:
    Ui::setting_actor_tachie *ui;
    MainWindow *mainWin;
    QSettings *settings;
    QSettings *settings_charactersConfig_config;
    QSettings *settings_characters_config;
    setting_actor_tachie_listchild *ui_setting_actor_tachie_listchild_win;
};

#endif // SETTING_ACTOR_TACHIE_H
