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

QString local_version = "v3.2.2";

MainWindow::MainWindow(QWidget *parent)
    : ElaWindow(parent)
    , ui(new Ui::MainWindow)
{
    /*窗口初始化*/
    qInfo()<<"MainWindows（设置页）初始化……";
    ui->setupUi(this);
    setUserInfoCardVisible(false);
    setWindowIcon(QIcon(":/img/img/logo.png"));
    QSettings *settings = new QSettings(qApp->applicationDirPath()+"/Setting.ini",QSettings::IniFormat);
    setting_general_win = new setting_general(this);
    addPageNode("通用设置",setting_general_win,ElaIconType::House);
    setting_ai_win = new setting_ai(this);
    addPageNode("AI模型设置",setting_ai_win,ElaIconType::UserRobot);
    setting_vits_win = new setting_vits(this);
    addPageNode("语音合成设置",setting_vits_win,ElaIconType::Speaker);
    setting_voiceinput_win = new setting_voiceinput(this);
    addPageNode("语音输入设置",setting_voiceinput_win,ElaIconType::CircleMicrophone);
    setting_actor_win = new setting_actor(this);
    addPageNode("角色配置",setting_actor_win,ElaIconType::GingerbreadMan);

    /*
     * 读取配置
    */
    qInfo()<<"读取配置……";
    /*通用设置*/
    //角色选择配置
    QDir dir(qApp->applicationDirPath()+"/characters");
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList folderList = dir.entryList();
    ui->comboBox_actor_choose->addItems(folderList);
    setting_general_win->findChild<QComboBox*>()->addItems(folderList);
    qInfo()<<"读取到角色列表："<<folderList;
    setting_general_win->findChild<QComboBox*>()->setCurrentIndex(folderList.indexOf(settings->value("/actor/name").toString()));
    //软件配置项
    qInfo()<<"读取软件设置……";
    setting_general_win->findChild<QCheckBox*>("checkBox_autostart")->setChecked(settings->value("/soft/auto_start").toBool());
    setting_general_win->findChild<QCheckBox*>("checkBox_autoopen")->setChecked(settings->value("/soft/auto_open").toBool());
    //窗口配置
    setting_general_win->findChild<QSpinBox*>("spinBox_dialogtime")->setValue(settings->value("/dialog/time").toInt());
    /*AI模型设置*/
    setting_ai_win->findChild<QLineEdit*>("lineEdit_url")->setText(settings->value("/llm/url").toString());
    setting_ai_win->findChild<QCheckBox*>("checkBox_feedback")->setChecked(settings->value("/llm/feedback").toBool());
    /*语音输入设置*/
    //基础设定
    //基础设置
    setting_voiceinput_win->findChild<QComboBox*>("comboBox_api")->addItems({"whisper-asr-webservice","百度语音识别"});
    setting_voiceinput_win->findChild<QComboBox*>("comboBox_api")->setCurrentIndex(settings->value("/speechInput/api").toInt());
    setting_voiceinput_win->findChild<QStackedWidget*>("stackedWidget_speechInput")->setCurrentIndex(settings->value("/speechInput/api").toInt());
    setting_voiceinput_win->findChild<QLineEdit*>("lineEdit_url")->setText(settings->value("/speechInput/url").toString());
    setting_voiceinput_win->findChild<QLineEdit*>("lineEdit_baiduapikey")->setText(settings->value("/speechInput/baidu_apikey").toString());
    setting_voiceinput_win->findChild<QLineEdit*>("lineEdit_baidusecretkey")->setText(settings->value("/speechInput/baidu_secretkey").toString());
    //语音设置
    setting_voiceinput_win->findChild<QCheckBox*>("checkBox_enable")->setChecked(settings->value("/speechInput/enable").toBool());
    setting_voiceinput_win->findChild<QCheckBox*>("checkBox_wake")->setChecked(settings->value("/speechInput/wake_enable").toBool());
    setting_voiceinput_win->findChild<QCheckBox*>("checkBox_interrupt")->setChecked(settings->value("/speechInput/interrupt").toBool());
    setting_voiceinput_win->findChild<QSpinBox*>("spinBox_energy")->setValue(settings->value("/speechInput/energy").toInt());
    setting_voiceinput_win->findChild<QSpinBox*>("spinBox_size")->setValue(settings->value("/speechInput/size").toInt());
    /*语音合成配置*/
    setting_vits_win->findChild<QCheckBox*>("checkBox_enable")->setChecked(settings->value("/vits/enable").toBool());
    setting_vits_win->findChild<QLineEdit*>("lineEdit_url")->setText(settings->value("/vits/url").toString());


    ui->comboBox_vits_model->addItems({"vits","w2v2-vits","bert-vits2","gpt-sovits"});
    ui->comboBox_vits_API->addItems({"vits-simple-api","自定义"});
    ui->comboBox_vits_language->addItems({"ja","zh"});






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
    model->appendRow(new QStandardItem(QStringLiteral("通用设置")));
    model->appendRow(new QStandardItem(QStringLiteral("AI模型设置")));
    model->appendRow(new QStandardItem(QStringLiteral("语音合成设置")));
    model->appendRow(new QStandardItem(QStringLiteral("语音输入设置")));
    model->appendRow(new QStandardItem(QStringLiteral("角色配置")));
    ui->treeView_up->setModel(model);
    QModelIndex modelindex = ui->treeView_up->model()->index(0, 0);
    ui->treeView_up->setCurrentIndex(modelindex);
    /*窗口初始化*/
    dialog_win = new galgamedialog;
    dialog_win->move(settings->value("/tachie/location_x").toInt(),settings->value("/tachie/location_y").toInt());
    tachie_win = new tachie;
    tachie_win->show();
    tachie_win->move(settings->value("/tachie/location_x").toInt(),settings->value("/tachie/location_y").toInt());



    /*信号槽连接*/
    connect(tachie_win, SIGNAL(show_dialogwin_to_main()), this, SLOT(show_dialogwin_from_tachie()));
    connect(tachie_win, SIGNAL(changeLocation_to_main(int,int)), this, SLOT(changeTachieLocation_from_tachie(int,int)));
    connect(dialog_win, SIGNAL(change_tachie_to_tachie(QString)), tachie_win, SLOT(changetachie_from_galdialog(QString)));
    connect(this, SIGNAL(init_to_tachie()), tachie_win, SLOT(init_from_main()));
    connect(this, SIGNAL(init_to_dialog()), dialog_win, SLOT(init_from_main()));
    connect(dialog_win, SIGNAL(energy_to_main(int)), this, SLOT(getEnergy_from_gal(int)));
    connect(this, SIGNAL(resetlocation_to_tachie()), tachie_win, SLOT(resetlocation_from_main()));
    /*自启动*/
    //运行自启动脚本
    if(settings->value("/soft/auto_open").toBool())
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
    reloadActorSetting();
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
    if(isDialogOpen)
    {
        dialog_win->hide();
        isDialogOpen = false;
    }
    else
    {
        dialog_win->show();
        isDialogOpen = true;
    }
}
/*重载角色配置*/
void MainWindow::reloadActorSetting()
{
    QSettings *settings = new QSettings(qApp->applicationDirPath()+"/Setting.ini",QSettings::IniFormat);
    QSettings *settings_actor = new QSettings(qApp->applicationDirPath()+"/characters/"+settings->value("actor/name").toString()+"/config.ini",QSettings::IniFormat);
    ui->spinBox_actor_tachie_size->setValue(settings_actor->value("/tachie/size").toInt());
    ui->lineEdit_vits_customUrl->setText(settings_actor->value("/vits/custom_url").toString());
    ui->lineEdit_vits_id->setText(settings_actor->value("/vits/id").toString());
    ui->comboBox_vits_model->setCurrentText(settings_actor->value("/vits/vitsmodel").toString());
    ui->comboBox_vits_API->setCurrentIndex(settings_actor->value("/vits/api").toInt());
    ui->lineEdit_llm_agent->setText(settings_actor->value("/llm/agent").toString());
    ui->comboBox_vits_language->setCurrentIndex(ui->comboBox_vits_language->findText(settings_actor->value("/vits/lan").toString()));
    ui->stackedWidget_vits->setCurrentIndex(settings_actor->value("/vits/api").toInt());
    ui->lineEdit_speechInput_url_wakeWord->setText(settings_actor->value("/speechInput/wake_word").toString());
    ui->lineEdit_speechInput_url_endWord->setText(settings_actor->value("/speechInput/end_word").toString());
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
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    settings->setValue("/tachie/location_x",x);
    settings->setValue("/tachie/location_y",y);
}
/*主窗口翻页*/
void MainWindow::on_treeView_up_clicked(const QModelIndex &index)
{
    qInfo()<<"主窗口 翻页到"+QString::number(index.row());
    ui->stackedWidget->setCurrentIndex(index.row());
    QSettings *settings = new QSettings("Setting.ini",QSettings::IniFormat);
    ui->label_editActor->setText("当前配置角色: "+settings->value("/actor/name").toString());
    ui->label_title->setText(ui->treeView_up->model()->data(index,Qt::DisplayRole).toString());
    reloadActorSetting();
}
/*保存配置*/
void MainWindow::saveSetting(const QString &key, const QVariant &value) {
    QScopedPointer<QSettings> settings(new QSettings("Setting.ini", QSettings::IniFormat)); //使用 QScopedPointer来自动管理资源。
    settings->setValue(key, value);
}
void MainWindow::saveActorSetting(const QString &key, const QVariant &value) {
    QScopedPointer<QSettings> settings(new QSettings("Setting.ini", QSettings::IniFormat)); //使用 QScopedPointer来自动管理资源。
    QSettings *settings_actor = new QSettings(qApp->applicationDirPath()+"/characters/"+settings->value("actor/name").toString()+"/config.ini",QSettings::IniFormat);
    settings_actor->setValue(key, value);
}
/*配置项修改和保存*/
void MainWindow::on_checkBox_dialog_enable_clicked(bool checked)
{
    if(checked) dialog_win->show();
    else dialog_win->hide();
}
void MainWindow::on_spinBox_actor_tachie_size_valueChanged(int arg1)
{
    saveActorSetting("/tachie/size",arg1);
    emit init_to_tachie();
}
void MainWindow::on_lineEdit_llm_agent_textChanged(const QString &arg1)
{
    saveActorSetting("/llm/agent",arg1);
}
void MainWindow::on_lineEdit_vits_id_textChanged(const QString &arg1)
{
    saveActorSetting("/vits/id",arg1);
}
void MainWindow::ChangeSetting_ActorChoose(const QString &arg1)
{
    if(already_init) saveSetting("/actor/name",arg1);
    emit init_to_tachie();
}
void MainWindow::on_comboBox_vits_model_currentTextChanged(const QString &arg1)
{
    if(already_init) saveActorSetting("/vits/vitsmodel",arg1);
}
void MainWindow::ChangeSetting_AutoStart(bool checked)
{
    saveSetting("/soft/auto_start",checked);
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
        saveActorSetting("/vits/api",index);
    }
}
void MainWindow::on_lineEdit_vits_customUrl_textChanged(const QString &arg1)
{
    saveActorSetting("/vits/custom_url",arg1);
}
void MainWindow::on_comboBox_vits_language_currentTextChanged(const QString &arg1)
{
    if(already_init)
    {
        saveActorSetting("/vits/lan",arg1);
    }
}
void MainWindow::ChangeSetting_speechInputAPI(int index)
{
    if(already_init)
    {
        saveSetting("/speechInput/api",index);
    }
    ui->stackedWidget_speechInput->setCurrentIndex(index);
}
void MainWindow::ChangSetting_speechInputWake(bool checked)
{
    emit init_to_dialog();
}
void MainWindow::on_lineEdit_speechInput_url_wakeWord_textChanged(const QString &arg1)
{
    saveActorSetting("/speechInput/wake_word",arg1);
}
void MainWindow::on_lineEdit_speechInput_url_endWord_textChanged(const QString &arg1)
{
    saveActorSetting("/speechInput/end_word",arg1);
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
