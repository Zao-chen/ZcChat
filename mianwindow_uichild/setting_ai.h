#ifndef SETTING_AI_H
#define SETTING_AI_H

#include <QWidget>

namespace Ui {
class setting_ai;
}

class setting_ai : public QWidget
{
    Q_OBJECT

public:
    explicit setting_ai(QWidget *parent = nullptr);
    ~setting_ai();

private:
    Ui::setting_ai *ui;
};

#endif // SETTING_AI_H
