#ifndef SETTING_GENERAL_H
#define SETTING_GENERAL_H

#include <QWidget>

namespace Ui {
class setting_general;
}

class setting_general : public QWidget
{
    Q_OBJECT

public:
    explicit setting_general(QWidget *parent = nullptr);
    ~setting_general();

private:
    Ui::setting_general *ui;
};

#endif // SETTING_GENERAL_H
