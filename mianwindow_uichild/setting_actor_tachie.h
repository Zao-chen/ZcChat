#ifndef SETTING_ACTOR_TACHIE_H
#define SETTING_ACTOR_TACHIE_H

#include <QWidget>

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

private:
    Ui::setting_actor_tachie *ui;
    MainWindow *mainWin;
};

#endif // SETTING_ACTOR_TACHIE_H
