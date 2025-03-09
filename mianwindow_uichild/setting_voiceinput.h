#ifndef SETTING_VOICEINPUT_H
#define SETTING_VOICEINPUT_H

#include <QWidget>

namespace Ui {
class setting_voiceinput;
}

class setting_voiceinput : public QWidget
{
    Q_OBJECT

public:
    explicit setting_voiceinput(QWidget *parent = nullptr);
    ~setting_voiceinput();

private:
    Ui::setting_voiceinput *ui;
};

#endif // SETTING_VOICEINPUT_H
