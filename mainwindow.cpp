#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QStandardItemModel>
#include <QSettings>
#include <QMenu>
#include <QProcess>
#include <QFile>
#include <QDesktopServices>
#include <Qdir>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

QString local_version = "v3.0.0";

MainWindow::MainWindow(QWidget *parent)
    : ElaWidget(parent)
    , ui(new Ui::MainWindow)
{
    qInfo()<<"MainWindows（设置页）初始化……";
    ui->setupUi(this);
    setWindowIcon(QIcon(":/img/img/logo.png"));
    /*一些初始项*/
    //立绘列表初始化
    QDir dir(qApp->applicationDirPath()+"/tachie");
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList folderList = dir.entryList();
    ui->comboBox_tachio_choose->addItems(folderList);
    //一些combobox初始化
    ui->comboBox_vits_model->addItems({"vits","w2v2-vits","bert-vits2","gpt-sovits"});
    ui->comboBox_vits_API->addItems({"vits-simple-api","自定义"});
    ui->comboBox_vits_language->addItems({"ja","zh"});
    ui->comboBox_speechInput_API->addItems({"whisper-asr-webservice","百度语音识别"});
    /*配置项读取*/
    QSettings *settings = new QSettings(qApp->applicationDirPath()+"/Setting.ini",QSettings::IniFormat);
    //软件配置项
    ui->checkBox_soft_autostart->setChecked(settings->value("/soft/autostart").toBool());
    ui->checkBox_soft_autoOpen->setChecked(settings->value("/soft/autoOpen").toBool());
    //立绘配置项
    ui->spinBox_tachie_size->setValue(settings->value("/tachie/size").toInt());
    ui->comboBox_tachio_choose->setCurrentIndex(folderList.indexOf(settings->value("/tachie/name").toString()));
    ui->lineEdit_tachi_location_x->setText(settings->value("/tachie/location_x").toString());
    ui->lineEdit_tachi_location_y->setText(settings->value("/tachie/location_y").toString());
    //对话框配置项
    ui->spinBox_dialog->setValue(settings->value("/dialog/time").toInt());
    //llm配置项
    ui->lineEdit_llm_url->setText(settings->value("/llm/url").toString());
    ui->lineEdit_llm_agent->setText(settings->value("/llm/agent").toString());
    ui->checkBox_llm_errorfeedback->setChecked(settings->value("/llm/feedback").toBool());
    //语音配置项
    ui->checkBox_vits_enable->setChecked(settings->value("/vits/enable").toBool());
    ui->lineEdit_vits_url->setText(settings->value("/vits/url").toString());
    ui->lineEdit_vits_customUrl->setText(settings->value("/vits/custom_url").toString());
    ui->lineEdit_vits_id->setText(settings->value("/vits/id").toString());
    ui->comboBox_vits_model->setCurrentText(settings->value("/vits/vitsmodel").toString());
    ui->comboBox_vits_API->setCurrentIndex(settings->value("/vits/api").toInt());
    ui->comboBox_vits_language->setCurrentIndex(ui->comboBox_vits_language->findText(settings->value("/vits/lan").toString()));
    ui->stackedWidget_vits->setCurrentIndex(settings->value("/vits/api").toInt());
    //语音输入配置项
    ui->checkBox_speechInput_enable->setChecked(settings->value("/speechInput/enable").toBool());
    ui->lineEdit_speechInput_url->setText(settings->value("/speechInput/url").toString());
    ui->comboBox_speechInput_API->setCurrentIndex(settings->value("/speechInput/api").toInt());
    ui->stackedWidget_speechInput->setCurrentIndex(settings->value("/speechInput/api").toInt());
    ui->lineEdit_speechInput_BaiduAPIKey->setText(settings->value("/speechInput/baidu_apikey").toString());
    ui->lineEdit_speechInput_BaiduSecretKey->setText(settings->value("/speechInput/baidu_secretkey").toString());
    ui->checkBox_speechInput_wake->setChecked(settings->value("/speechInput/wake_enable").toBool());
    ui->spinBox_energy->setValue(settings->value("/speechInput/energy").toInt());
    ui->spinBox_size->setValue(settings->value("/speechInput/size").toInt());
    ui->lineEdit_speechInput_url_wakeWord->setText(settings->value("/speechInput/wakeWord").toString());
    ui->lineEdit_speechInput_url_endWord->setText(settings->value("/speechInput/endWord").toString());
    ui->checkBox_speechInput_interrupt->setChecked(settings->value("/speechInput/interrupt").toBool());
    /*托盘*/
    //初始化托盘
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
    //托盘菜单
    m_showMainAction = new QAction("主界面", this);
    connect(m_showMainAction,SIGNAL(triggered()),this,SLOT(on_showMainAction()));
    m_openGithub = new QAction("Github", this);
    connect(m_openGithub,SIGNAL(triggered()),this,SLOT(on_openGithub()));
    m_exitAppAction = new QAction("退出", this);
    connect(m_exitAppAction,SIGNAL(triggered()),this,SLOT(on_exitAppAction()));
    m_restartAppAction = new QAction("重启", this);
    connect(m_restartAppAction,SIGNAL(triggered()),this,SLOT(on_restartAppAction()));
    m_menu = new QMenu(this);
    m_menu->addAction(m_showMainAction); //新增菜单项
    m_menu->addAction(m_openGithub); //新增菜单项--打开github
    m_menu->addSeparator(); //增加分隔符
    m_menu->addAction(m_restartAppAction); //新增菜单项---重启
    m_menu->addAction(m_exitAppAction); //新增菜单项---退出程序
    m_sysTrayIcon->setContextMenu(m_menu); //把QMenu赋给QSystemTrayIcon对象
    m_sysTrayIcon->show(); //在系统托盘显示此对象
    /*构造菜单*/
    QStandardItemModel* model = new QStandardItemModel(ui->treeView_up);
    model->appendRow(new QStandardItem(QStringLiteral("软件设置")));
    model->appendRow(new QStandardItem(QStringLiteral("立绘设置")));
    model->appendRow(new QStandardItem(QStringLiteral("对话框设置")));
    model->appendRow(new QStandardItem(QStringLiteral("AI模型设置")));
    model->appendRow(new QStandardItem(QStringLiteral("语音合成设置")));
    model->appendRow(new QStandardItem(QStringLiteral("语音输入设置")));
    ui->treeView_up->setModel(model);
    QModelIndex modelindex = ui->treeView_up->model()->index(0, 0);
    ui->treeView_up->setCurrentIndex(modelindex);
    /*窗口初始化*/
    dialog_win = new galgamedialog;
    dialog_win->move(ui->lineEdit_tachi_location_x->text().toInt(),ui->lineEdit_tachi_location_y->text().toInt());
    tachie_win = new tachie;
    tachie_win->show();
    tachie_win->move(ui->lineEdit_tachi_location_x->text().toInt(),ui->lineEdit_tachi_location_y->text().toInt());
    /*信号槽连接*/
    connect(tachie_win, SIGNAL(show_dialogwin_to_main()), this, SLOT(show_dialogwin_from_tachie()));
    connect(tachie_win, SIGNAL(changeLocation_to_main(int,int)), this, SLOT(changeTachieLocation_from_tachie(int,int)));
    connect(dialog_win, SIGNAL(change_tachie_to_tachie(QString)), tachie_win, SLOT(changetachie_from_galdialog(QString)));
    connect(this, SIGNAL(init_to_tachie()), tachie_win, SLOT(init_from_main()));
    connect(this, SIGNAL(init_to_dialog()), dialog_win, SLOT(init_from_main()));
    connect(dialog_win, SIGNAL(energy_to_main(int)), this, SLOT(getEnergy_from_gal(int)));
    connect(this, SIGNAL(resetlocation_to_tachie()), tachie_win, SLOT(resetlocation_from_main()));
    /*自启动*/
    //letta自启动
    if(ui->checkBox_soft_autoOpen->isChecked())
    {
        qInfo()<<"运行autoOpen.cmd…";
        QDesktopServices::openUrl(QUrl::fromLocalFile("autoOpen.cmd"));
    }
    /*新版本获取*/
    m_manager = new QNetworkAccessManager(this);//新建QNetworkAccessManager对象
    checkForUpdates();
    already_init = true;
    /*获取能量定时器*/
    energyTimer = new QTimer(this);
    connect(energyTimer, &QTimer::timeout, this, &MainWindow::updateEnergyDisplay);
    energyTimer->start(100); // 每100毫秒触发一次
    qInfo()<<"MainWindows（设置页）加载完成！";
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*对话框显示隐藏*/
void MainWindow::show_dialogwin_from_tachie()
{
    qInfo()<<"【接收】立绘 --- 打开对话框 ---> 主窗口";
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
/*检查更新*/
void MainWindow::checkForUpdates() {
    QString reply = getUrl("https://api.github.com/repos/Zao-chen/ZcChat/releases/latest");
    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();
    QString tagName = jsonObj.value("tag_name").toString();
    qInfo()<<"获取最新版，返回值："<<reply;
    if (reply == "error" || tagName.isEmpty())
    {
        ui->label_mainMessage->setText(local_version + "  获取新版本失败");
    }
    else if (local_version != tagName)
    {
        ui->label_mainMessage->setText(local_version + "  发现新版本" + tagName);
    }
    else
    {
        ui->label_mainMessage->setText(local_version);
    }
}
/*get请求（用于获取版本）*/
QByteArray MainWindow::getUrl(const QString &input)
{
    QEventLoop loop;
    QNetworkReply *reply = m_manager->get(QNetworkRequest(QUrl(input)));
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        return reply->readAll();
    } else {
        return "error";
    }
}
/*保存立绘位置*/
void MainWindow::changeTachieLocation_from_tachie(int x,int y)
{
    qInfo()<<"【接收】立绘移动到："<<this->x()<<" "<<this->y();
    ui->lineEdit_tachi_location_x->setText(QString::number(x));
    ui->lineEdit_tachi_location_y->setText(QString::number(y));
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    settings->setValue("/tachie/location_x",x);
    settings->setValue("/tachie/location_y",y);
}
/*主窗口翻页*/
void MainWindow::on_treeView_up_clicked(const QModelIndex &index)
{
    qInfo()<<"主窗口 翻页到"+QString::number(index.row());
    ui->stackedWidget->setCurrentIndex(index.row());
    ui->label_title->setText(ui->treeView_up->model()->data(index,Qt::DisplayRole).toString());
}
/*保存配置*/
void MainWindow::saveSetting(const QString &key, const QVariant &value) {
    QScopedPointer<QSettings> settings(new QSettings("Setting.ini", QSettings::IniFormat)); //使用 QScopedPointer来自动管理资源。
    settings->setValue(key, value);
}
/*配置项修改和保存*/
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
    saveSetting("/tachie/size",arg1);
    emit init_to_tachie();
}
void MainWindow::on_lineEdit_llm_url_textChanged(const QString &arg1)
{
    saveSetting("/llm/url",arg1);
}
void MainWindow::on_lineEdit_llm_agent_textChanged(const QString &arg1)
{
    saveSetting("/llm/agent",arg1);
}
void MainWindow::on_checkBox_vits_enable_clicked(bool checked)
{
    saveSetting("/vits/enable",checked);
}
void MainWindow::on_lineEdit_vits_url_textChanged(const QString &arg1)
{
    saveSetting("/vits/url",arg1);
}
void MainWindow::on_lineEdit_vits_id_textChanged(const QString &arg1)
{
    saveSetting("/vits/id",arg1);
}
void MainWindow::on_checkBox_soft_autoOpen_clicked(bool checked)
{
    saveSetting("/soft/autoOpen",checked);
}
void MainWindow::on_spinBox_dialog_valueChanged(int arg1)
{
    saveSetting("/dialog/time",arg1);
}
void MainWindow::on_comboBox_tachio_choose_currentTextChanged(const QString &arg1)
{
    if(already_init) saveSetting("/tachie/name",arg1);
    emit init_to_tachie();
}
void MainWindow::on_checkBox_llm_errorfeedback_clicked(bool checked)
{
    saveSetting("/llm/feedback",checked);
}
void MainWindow::on_comboBox_vits_model_currentTextChanged(const QString &arg1)
{
    if(already_init) saveSetting("/vits/vitsmodel",arg1);
}
void MainWindow::on_checkBox_soft_autostart_clicked(bool checked)
{
    saveSetting("/soft/autostart",checked);
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
        saveSetting("/vits/api",index);
    }
}
void MainWindow::on_lineEdit_vits_customUrl_textChanged(const QString &arg1)
{
    saveSetting("/vits/custom_url",arg1);
}
void MainWindow::on_comboBox_vits_language_currentTextChanged(const QString &arg1)
{
    if(already_init)
    {
        saveSetting("/vits/lan",arg1);
    }
}
void MainWindow::on_checkBox_speechInput_enable_clicked(bool checked)
{
    saveSetting("/speechInput/enable",checked);
    emit init_to_dialog();
}
void MainWindow::on_lineEdit_speechInput_url_textChanged(const QString &arg1)
{
    saveSetting("/speechInput/url",arg1);
}
void MainWindow::on_comboBox_speechInput_API_currentIndexChanged(int index)
{
    if(already_init)
    {
        saveSetting("/speechInput/api",index);
    }
    ui->stackedWidget_speechInput->setCurrentIndex(index);
}
void MainWindow::on_lineEdit_speechInput_BaiduAPIKey_textChanged(const QString &arg1)
{
    saveSetting("/speechInput/baidu_apikey",arg1);
}
void MainWindow::on_lineEdit_speechInput_BaiduSecretKey_textChanged(const QString &arg1)
{
    saveSetting("/speechInput/baidu_secretkey",arg1);
}
void MainWindow::on_checkBox_speechInput_wake_clicked(bool checked)
{
    saveSetting("/speechInput/wake_enable",checked);
    emit init_to_dialog();
}
void MainWindow::on_lineEdit_speechInput_url_wakeWord_textChanged(const QString &arg1)
{
    saveSetting("/speechInput/wakeWord",arg1);
}
void MainWindow::on_lineEdit_speechInput_url_endWord_textChanged(const QString &arg1)
{
    saveSetting("/speechInput/endWord",arg1);
}
void MainWindow::on_spinBox_energy_valueChanged(int arg1)
{
    saveSetting("/speechInput/energy",arg1);
}
void MainWindow::on_spinBox_size_valueChanged(int arg1)
{
    saveSetting("/speechInput/size",arg1);
}
void MainWindow::on_checkBox_speechInput_interrupt_clicked(bool checked)
{
    saveSetting("/speechInput/interrupt",checked);
}
/*重置立绘位置*/
void MainWindow::on_pushButton_reset_clicked()
{
    emit resetlocation_to_tachie();
}
/*托盘主界面*/
void MainWindow::on_showMainAction()
{
    this->show();
}
/*托盘退出*/
void MainWindow::on_exitAppAction()
{
    qApp->exit();
}
/*托盘重启*/
void MainWindow::on_restartAppAction()
{
    // 获取当前程序的路径
    QString program = QCoreApplication::applicationFilePath();
    // 获取当前程序的启动参数
    QStringList arguments = QCoreApplication::arguments();
    // 启动新进程
    QProcess::startDetached(program, arguments);
    // 退出当前程序
    QCoreApplication::quit();
}
/*托盘打开github*/
void MainWindow::on_openGithub()
{
    QDesktopServices::openUrl(QUrl("https://github.com/Zao-chen/ZcChat"));
}
/*隐藏窗口*/
void MainWindow::hideWindow()
{
    this->hide();
}
//能量显示
void MainWindow::getEnergy_from_gal(int energy)
{
    currentEnergy = energy; // 更新当前能量值
}
void MainWindow::updateEnergyDisplay() {
    ui->lcdNumber->display(currentEnergy);
}


