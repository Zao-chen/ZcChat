#ifndef SETTING_ACTOR_H
#define SETTING_ACTOR_H

#include <QWidget>

namespace Ui {
class setting_actor;
}

class setting_actor : public QWidget
{
    Q_OBJECT

public:
    explicit setting_actor(QWidget *parent = nullptr);
    ~setting_actor();

private:
    Ui::setting_actor *ui;
};

#endif // SETTING_ACTOR_H
