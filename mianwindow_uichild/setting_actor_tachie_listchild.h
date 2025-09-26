#ifndef SETTING_ACTOR_TACHIE_LISTCHILD_H
#define SETTING_ACTOR_TACHIE_LISTCHILD_H

#include <QWidget>

namespace Ui {
class setting_actor_tachie_listchild;
}

class setting_actor_tachie_listchild : public QWidget
{
    Q_OBJECT

public:
    explicit setting_actor_tachie_listchild(const QString &name,const QPixmap img,QWidget *parent = nullptr);
    ~setting_actor_tachie_listchild();

private slots:
    void on_comboBox_anime_currentIndexChanged(int index);

    void on_comboBox_ptc_currentIndexChanged(int index);

private:
    Ui::setting_actor_tachie_listchild *ui;
};

#endif // SETTING_ACTOR_TACHIE_LISTCHILD_H
