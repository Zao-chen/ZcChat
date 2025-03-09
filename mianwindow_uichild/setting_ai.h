#ifndef SETTING_AI_H
#define SETTING_AI_H

#include <QWidget>

class MainWindow;

namespace Ui {
class setting_ai;
}

class setting_ai : public QWidget
{
    Q_OBJECT

public:
    explicit setting_ai(QWidget *parent = nullptr);
    ~setting_ai();

private slots:
    void on_lineEdit_url_textChanged(const QString &arg1);

    void on_checkBox_feedback_clicked(bool checked);

private:
    Ui::setting_ai *ui;
    MainWindow *mainWin;
};

#endif // SETTING_AI_H
