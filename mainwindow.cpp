#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QStandardItemModel>
#include <QSettings>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : ElaWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //配置项
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    ui->spinBox->setValue(settings->value("/tachie/size").toInt());
    ui->lineEdit_llm_url->setText(settings->value("/llm/url").toString());
    ui->lineEdit_llm_agent->setText(settings->value("/llm/agent").toString());
    ui->checkBox_4->setChecked(settings->value("/vits/enable").toBool());
    ui->lineEdit_vits_url->setText(settings->value("/vits/url").toString());
    ui->lineEdit_tachie_name->setText(settings->value("/tachie/name").toString());
    //托盘
    m_sysTrayIcon = new QSystemTrayIcon(this); //新建QSystemTrayIcon对象
    QIcon icon = QIcon(":/img/img/logo.png"); //资源文件添加的图标
    m_sysTrayIcon->setIcon(icon);
    m_sysTrayIcon->setToolTip("ZcChat"); //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    connect(m_sysTrayIcon, &QSystemTrayIcon::activated, //给QSystemTrayIcon添加槽函数
            [=](QSystemTrayIcon::ActivationReason reason)
            {
                switch(reason)
                {
                case QSystemTrayIcon::Trigger: //单击托盘图标
                    this->show();
                    break;
                case QSystemTrayIcon::DoubleClick: //双击托盘图标
                    m_sysTrayIcon->showMessage("ZcChat",
                                               "左键打开主界面，右键打开菜单",
                                               QSystemTrayIcon::Information,
                                               1000);
                    break;
                default:
                    break;
                }
            });
    createActions(); //建立托盘操作的菜单
    createMenu();
    m_sysTrayIcon->show(); //在系统托盘显示此对象
    //构造Model
    QStandardItemModel* model = new QStandardItemModel(ui->treeView_up);
    model->appendRow(new QStandardItem(QStringLiteral("立绘设置")));
    model->appendRow(new QStandardItem(QStringLiteral("对话框设置")));
    model->appendRow(new QStandardItem(QStringLiteral("模型设置")));
    model->appendRow(new QStandardItem(QStringLiteral("语音设置")));
    ui->treeView_up->setModel(model);
    QModelIndex modelindex = ui->treeView_up->model()->index(0, 0);
    ui->treeView_up->setCurrentIndex(modelindex);
    //窗口设置
    dialog_win = new galgamedialog;
    tachie_win = new tachie;
    tachie_win->show();
    connect(tachie_win, SIGNAL(show_dialogwin_to_main()), this, SLOT(show_dialogwin_from_tachie()));
    connect(dialog_win, SIGNAL(change_tachie_to_tachie(QString)), tachie_win, SLOT(changetachie_from_galdialog(QString)));
    connect(this, SIGNAL(init_to_tachie()), tachie_win, SLOT(init_from_main()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkBox_2_clicked(bool checked)
{
    if(checked) tachie_win->show();
    else tachie_win->hide();
}

void MainWindow::show_dialogwin_from_tachie()
{
    qDebug()<<"【接收】立绘 --- 打开对话框 ---> 主窗口";
    if(ui->checkBox_3->isChecked())
    {
        dialog_win->hide();
        ui->checkBox_3->setChecked(false);
    }
    else
    {
        dialog_win->show();
        ui->checkBox_3->setChecked(true);
    }
}

void MainWindow::on_checkBox_3_clicked(bool checked)
{
    if(checked) dialog_win->show();
    else dialog_win->hide();
}


void MainWindow::on_treeView_up_clicked(const QModelIndex &index)
{
    qDebug()<<"主窗口 翻页到"+QString::number(index.row());
    ui->stackedWidget->setCurrentIndex(index.row());
    switch (index.row()) {
    case 0:
        ui->label_title->setText("立绘设置");
        break;
    case 1:
        ui->label_title->setText("对话框设置");
        break;
    case 2:
        ui->label_title->setText("模型设置");
        break;
    case 3:
        ui->label_title->setText("语音设置");
        break;
    default:
        break;
    }
}
/*配置项修改*/
void MainWindow::on_spinBox_valueChanged(int arg1)
{
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    settings->setValue("/tachie/size",arg1);
    delete settings;
    emit init_to_tachie();
}
void MainWindow::on_lineEdit_llm_url_textChanged(const QString &arg1)
{
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    settings->setValue("/llm/url",arg1);
    delete settings;
}
void MainWindow::on_lineEdit_llm_agent_textChanged(const QString &arg1)
{
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    settings->setValue("/llm/agent",arg1);
    delete settings;
}
void MainWindow::on_checkBox_4_clicked(bool checked)
{
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    settings->setValue("/vits/enable",checked);
    delete settings;
}
void MainWindow::on_lineEdit_vits_url_textChanged(const QString &arg1)
{
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    settings->setValue("/vits/url",arg1);
    delete settings;
}
void MainWindow::on_lineEdit_tachie_name_textChanged(const QString &arg1)
{
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    settings->setValue("/tachie/name",arg1);
    delete settings;
    emit init_to_tachie();
}
void MainWindow::on_lineEdit_vits_id_textChanged(const QString &arg1)
{
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    settings->setValue("/vits/id",arg1);
    delete settings;
}
/*托盘*/
//托盘动作
void MainWindow::createActions()
{
    m_showMainAction = new QAction("主界面", this);
    connect(m_showMainAction,SIGNAL(triggered()),this,SLOT(on_showMainAction()));
    m_exitAppAction = new QAction("退出", this);
    connect(m_exitAppAction,SIGNAL(triggered()),this,SLOT(on_exitAppAction()));
}
//创建托盘菜单
void MainWindow::createMenu()
{
    m_menu = new QMenu(this);
    m_menu->addAction(m_showMainAction); //新增菜单项
    m_menu->addSeparator(); //增加分隔符
    m_menu->addAction(m_exitAppAction); //新增菜单项---退出程序
    m_sysTrayIcon->setContextMenu(m_menu); //把QMenu赋给QSystemTrayIcon对象
}
//托盘主界面
void MainWindow::on_showMainAction()
{
    this->show();

}
//托盘推出
void MainWindow::on_exitAppAction()
{
    qApp->exit();
}
//窗口关闭
void MainWindow::closeEvent(QCloseEvent *event)
{
    //忽略窗口关闭事件
    qDebug()<<"1";
    QApplication::setQuitOnLastWindowClosed( true );
    this->hide();
}
//隐藏窗口
void MainWindow::hideWindow()
{
    this->hide();
}


