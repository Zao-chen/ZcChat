#ifndef SETTING_VITS_H
#define SETTING_VITS_H

#include <QWidget>

namespace Ui {
class setting_vits;
}

class setting_vits : public QWidget
{
    Q_OBJECT

public:
    explicit setting_vits(QWidget *parent = nullptr);
    ~setting_vits();

private slots:
    void on_checkBox_enable_clicked(bool checked);


    void on_lineEdit_url_textChanged(const QString &arg1);

private:
    Ui::setting_vits *ui;
};

#endif // SETTING_VITS_H
