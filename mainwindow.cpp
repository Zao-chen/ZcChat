#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QStandardItemModel>
#include <QSettings>
#include <QMenu>
#include <QProcess>
#include <QFile>
#include <QDesktopServices>
#include <Qdir>


MainWindow::MainWindow(QWidget *parent)
    : ElaWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/img/img/logo.png"));
    //一些初始项
    QDir dir(qApp->applicationDirPath()+"/tachie");
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList folderList = dir.entryList();
    ui->comboBox_tachio_choose->addItems(folderList);
    ui->comboBox_vits_model->addItems({"vits","w2v2-vits","bert-vits2","gpt-sovits"});
    ui->comboBox_vits_API->addItems({"vits-simple-api","自定义"});
    //配置项
    QSettings *settings = new QSettings(qApp->applicationDirPath()+"/Setting.ini",QSettings::IniFormat);
    ui->spinBox_tachie_size->setValue(settings->value("/tachie/size").toInt());
    ui->lineEdit_llm_url->setText(settings->value("/llm/url").toString());
    ui->lineEdit_llm_agent->setText(settings->value("/llm/agent").toString());
    ui->checkBox_vits_enable->setChecked(settings->value("/vits/enable").toBool());
    ui->checkBox_soft_autostart->setChecked(settings->value("/soft/autostart").toBool());
    ui->lineEdit_vits_url->setText(settings->value("/vits/url").toString());
    ui->lineEdit_vits_customUrl->setText(settings->value("/vits/custom_url").toString());
    ui->lineEdit_vits_id->setText(settings->value("/vits/id").toString());
    ui->comboBox_tachio_choose->setCurrentIndex(folderList.indexOf(settings->value("/tachie/name").toString()));
    ui->comboBox_vits_model->setCurrentText(settings->value("/vits/vitsmodel").toString());
    ui->comboBox_vits_API->setCurrentIndex(settings->value("/vits/api").toInt());
    ui->stackedWidget_vits->setCurrentIndex(settings->value("/vits/api").toInt());
    ui->lineEdit_tachi_location_x->setText(settings->value("/tachie/location_x").toString());
    ui->lineEdit_tachi_location_y->setText(settings->value("/tachie/location_y").toString());
    ui->checkBox_llm_autoopen_enable->setChecked(settings->value("/llm/autoOpen").toBool());
    ui->lineEdit_llm_location->setText(settings->value("/llm/location").toString());
    ui->checkBox_vits_autoopen->setChecked(settings->value("/vits/autoOpen").toBool());
    ui->checkBox_llm_errorfeedback->setChecked(settings->value("/llm/feedback").toBool());
    ui->lineEdit_vits_location->setText(settings->value("/vits/location").toString());
    ui->spinBox_dialog->setValue(settings->value("/dialog/time").toInt());
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
    model->appendRow(new QStandardItem(QStringLiteral("软件设置")));
    model->appendRow(new QStandardItem(QStringLiteral("立绘设置")));
    model->appendRow(new QStandardItem(QStringLiteral("对话框设置")));
    model->appendRow(new QStandardItem(QStringLiteral("模型设置")));
    model->appendRow(new QStandardItem(QStringLiteral("语音设置")));
    ui->treeView_up->setModel(model);
    QModelIndex modelindex = ui->treeView_up->model()->index(0, 0);
    ui->treeView_up->setCurrentIndex(modelindex);
    //窗口设置
    dialog_win = new galgamedialog;
    dialog_win->move(ui->lineEdit_tachi_location_x->text().toInt(),ui->lineEdit_tachi_location_y->text().toInt());
    tachie_win = new tachie;
    tachie_win->show();
    tachie_win->move(ui->lineEdit_tachi_location_x->text().toInt(),ui->lineEdit_tachi_location_y->text().toInt());
    //信号槽连接
    connect(tachie_win, SIGNAL(show_dialogwin_to_main()), this, SLOT(show_dialogwin_from_tachie()));
    connect(tachie_win, SIGNAL(changeLocation_to_main(int,int)), this, SLOT(changeTachieLocation_from_tachie(int,int)));
    connect(dialog_win, SIGNAL(change_tachie_to_tachie(QString)), tachie_win, SLOT(changetachie_from_galdialog(QString)));
    connect(this, SIGNAL(init_to_tachie()), tachie_win, SLOT(init_from_main()));
    //自启动
    if(ui->checkBox_llm_autoopen_enable->isChecked())
    {
        QFile file("steart_letta.cmd");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << "@echo off\n";
            out << "cd /d "+ui->lineEdit_llm_location->text()+"\n";
            out << "letta server\n";
            out << "pause\n";
            file.close();
        }
        QDesktopServices::openUrl(QUrl::fromLocalFile("steart_letta.cmd"));
    }
    if(ui->checkBox_vits_autoopen->isChecked())
    {
        QFile file("steart_vits.cmd");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << "@echo off\n";
            out << "cd /d "+ui->lineEdit_vits_location->text()+"\n";
            out << "start start.bat\n";
            file.close();
        }
        QDesktopServices::openUrl(QUrl::fromLocalFile("steart_vits.cmd"));
    }
    already_init = true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//显示对话框
void MainWindow::show_dialogwin_from_tachie()
{
    qDebug()<<"【接收】立绘 --- 打开对话框 ---> 主窗口";
    if(ui->checkBox_dialog_enable->isChecked())
    {
        dialog_win->hide();
        ui->checkBox_dialog_enable->setChecked(false);
    }
    else
    {
        dialog_win->show();
        ui->checkBox_dialog_enable->setChecked(true);
    }
}
//保存位置
void MainWindow::changeTachieLocation_from_tachie(int x,int y)
{
    qDebug()<<"【接收】立绘 --- 保存位置 ---> 主窗口"<<this->x()<<" "<<this->y();
    ui->lineEdit_tachi_location_x->setText(QString::number(x));
    ui->lineEdit_tachi_location_y->setText(QString::number(y));
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    settings->setValue("/tachie/location_x",x);
    settings->setValue("/tachie/location_y",y);
}
//翻页
void MainWindow::on_treeView_up_clicked(const QModelIndex &index)
{
    qDebug()<<"主窗口 翻页到"+QString::number(index.row());
    ui->stackedWidget->setCurrentIndex(index.row());
    switch (index.row()) {
    case 0:
        ui->label_title->setText("软件设置");
        break;
    case 1:
        ui->label_title->setText("立绘设置");
        break;
    case 2:
        ui->label_title->setText("对话框设置");
        break;
    case 3:
        ui->label_title->setText("模型设置");
        break;
    case 4:
        ui->label_title->setText("语音设置");
        break;
    default:
        break;
    }
}
/*配置项修改*/
void MainWindow::on_checkBox_dialog_enable_clicked(bool checked)
{
    if(checked) dialog_win->show();
    else dialog_win->hide();
}
void MainWindow::on_checkBox_tachie_enable_clicked(bool checked)
{
    if(checked) tachie_win->show();
    else tachie_win->hide();
}
void MainWindow::on_spinBox_tachie_size_valueChanged(int arg1)
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
void MainWindow::on_checkBox_vits_enable_clicked(bool checked)
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
// void MainWindow::on_lineEdit_tachie_name_textChanged(const QString &arg1)
// {
//     QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
//     settings->setValue("/tachie/name",arg1);
//     delete settings;
//     emit init_to_tachie();
// }
void MainWindow::on_lineEdit_vits_id_textChanged(const QString &arg1)
{
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    settings->setValue("/vits/id",arg1);
    delete settings;
}
void MainWindow::on_checkBox_llm_autoopen_enable_clicked(bool checked)
{
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    settings->setValue("/llm/autoOpen",checked);
    delete settings;
}
void MainWindow::on_lineEdit_llm_location_textChanged(const QString &arg1)
{
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    settings->setValue("/llm/location",arg1);
    delete settings;
}
void MainWindow::on_checkBox_vits_autoopen_clicked(bool checked)
{
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    settings->setValue("/vits/autoOpen",checked);
    delete settings;
}
void MainWindow::on_lineEdit_vits_location_textChanged(const QString &arg1)
{
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    settings->setValue("/vits/location",arg1);
    delete settings;
}
void MainWindow::on_spinBox_dialog_valueChanged(int arg1)
{
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    settings->setValue("/dialog/time",arg1);
    delete settings;
}
void MainWindow::on_comboBox_tachio_choose_currentTextChanged(const QString &arg1)
{
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    settings->setValue("/tachie/name",arg1);
    delete settings;
    emit init_to_tachie();
}
void MainWindow::on_checkBox_llm_errorfeedback_clicked(bool checked)
{
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    settings->setValue("/llm/feedback",checked);
    delete settings;
}
void MainWindow::on_comboBox_vits_model_currentTextChanged(const QString &arg1)
{
    if(already_init)
    {
        QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
        settings->setValue("/vits/vitsmodel",arg1);
        delete settings;
    }
}
void MainWindow::on_checkBox_soft_autostart_clicked(bool checked)
{
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    settings->setValue("/soft/autostart",checked);
    delete settings;
    QString application_name = QApplication::applicationName(); //获取应用名称
    QSettings *nsettings = new QSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat); //创建QSetting, 需要添加QSetting头文件
    if(checked)
    {
        QString application_path = QApplication::applicationFilePath(); //找到应用的目录
        nsettings->setValue(application_name, application_path.replace("/", "\\")); //写入注册表
    }
    else
    {
        nsettings->remove(application_name); //从注册表中删除
    }
}
void MainWindow::on_comboBox_vits_API_currentIndexChanged(int index)
{
    if(already_init)
    {
        ui->stackedWidget_vits->setCurrentIndex(index);
        QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
        settings->setValue("/vits/api",index);
        delete settings;
    }
}
void MainWindow::on_lineEdit_vits_customUrl_textChanged(const QString &arg1)
{
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    settings->setValue("/vits/custom_url",arg1);
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
//隐藏窗口
void MainWindow::hideWindow()
{
    this->hide();
}






