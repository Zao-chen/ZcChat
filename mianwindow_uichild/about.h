#ifndef ABOUT_H
#define ABOUT_H

#include <QWidget>
#include <QNetworkAccessManager>

class MainWindow;

namespace Ui {
class about;
}

class about : public QWidget
{
    Q_OBJECT

public:
    explicit about(QWidget *parent = nullptr);
    ~about();

private slots:

    void on_pushButton_upDate_clicked();

private:
    Ui::about *ui;
    /*网络相关*/
    QByteArray getUrl(const QString &input); //网络
    QNetworkAccessManager *m_manager;
    MainWindow *mainWin;
    QString result;
    void showError(QString msg);
};

#endif // ABOUT_H
