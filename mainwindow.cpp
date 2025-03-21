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
#include <QTimer>
#include <QLCDNumber>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QStackedWidget>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <qpropertyanimation>
#include <QTranslator>
#include <QStandardPaths>

#include "third_party/json/json.hpp"
using json_t = nlohmann::json;

QString local_version = "v4.4.2-beta";

MainWindow::MainWindow(QWidget *parent)
    : ElaWindow(parent)
    , ui(new Ui::MainWindow)
{
    /*窗口初始化*/
    qInfo()<<"MainWindow初始化……";
    ui->setupUi(this);
    setWindowIcon(QIcon(":/img/img/logo.png"));
    setUserInfoCardTitle("ZcChat "+local_version);
    setUserInfoCardSubTitle("检测新版本中……");
    window()->resize(1280,720);
    QString appFolder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat"; // 应用名称为 ZcChat
    QDir dir1(appFolder);
    if (!dir1.exists()) {
        dir1.mkpath("."); // 创建文件夹
    }
    QSettings *settings = new QSettings(appFolder + "/Setting.ini", QSettings::IniFormat);
    setUserInfoCardPixmap(QPixmap(appFolder + "/characters/" + settings->value("actor/name").toString() + "/正常.png"));
    /*子页面初始化*/
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
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/characters");
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList folderList = dir.entryList();
    setting_general_win->findChild<QComboBox*>("comboBox_actor")->addItems(folderList);
    qInfo()<<"读取到角色列表："<<folderList;
    setting_general_win->findChild<QComboBox*>("comboBox_actor")->setCurrentIndex(folderList.indexOf(settings->value("/actor/name").toString()));
    //软件配置项
    setting_general_win->findChild<QCheckBox*>("checkBox_autostart")->setChecked(settings->value("/soft/auto_start").toBool());
    setting_general_win->findChild<QCheckBox*>("checkBox_autoopen")->setChecked(settings->value("/soft/auto_open").toBool());
    setting_general_win->findChild<QComboBox*>("comboBox_lan")->setCurrentText(settings->value("/soft/lan").toString());
    qInfo()<<"使用语言"<<settings->value("/soft/lan").toString();
    //窗口配置
    setting_general_win->findChild<QSpinBox*>("spinBox_dialogtime")->setValue(settings->value("/dialog/time").toInt());
    setting_general_win->findChild<QSpinBox*>("spinBox_dialogscale")->setValue(settings->value("/dialog/size").toInt());
    /*AI模型设置*/
    setting_ai_win->findChild<QLineEdit*>("lineEdit_url")->setText(settings->value("/llm/url").toString());
    setting_ai_win->findChild<QCheckBox*>("checkBox_feedback")->setChecked(settings->value("/llm/feedback").toBool());
    /*语音输入设置*/
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
    /*语音合成设置*/
    setting_vits_win->findChild<QCheckBox*>("checkBox_enable")->setChecked(settings->value("/vits/enable").toBool());
    setting_vits_win->findChild<QLineEdit*>("lineEdit_url")->setText(settings->value("/vits/url").toString());
    /*角色设置*/
    setting_actor_win->findChild<QComboBox*>("comboBox_vits_api")->addItems({"vits-simple-api","自定义"});
    setting_actor_win->findChild<QComboBox*>("comboBox_vits_model")->addItems({"vits","w2v2-vits","bert-vits2","gpt-sovits"});
    setting_actor_win->findChild<QComboBox*>("comboBox_vits_language")->addItems({"ja","zh"});
    reloadActorSetting();
    /*托盘*/
    qInfo()<<"初始化托盘……";
    //初始化托盘
    m_sysTrayIcon = new QSystemTrayIcon(this); //新建QSystemTrayIcon对象
    m_sysTrayIcon->setIcon(QIcon(":/img/img/logo.png"));
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
    //托盘连接
    m_showMainAction = new QAction("主界面", this);
    connect(m_showMainAction,SIGNAL(triggered()),this,SLOT(on_showMainAction()));
    m_openGithub = new QAction("Github", this);
    connect(m_openGithub,SIGNAL(triggered()),this,SLOT(on_openGithub()));
    m_exitAppAction = new QAction("退出", this);
    connect(m_exitAppAction,SIGNAL(triggered()),this,SLOT(on_exitAppAction()));
    m_restartAppAction = new QAction("重启", this);
    connect(m_restartAppAction,SIGNAL(triggered()),this,SLOT(on_restartAppAction()));
    m_resetTachie = new QAction("重置立绘", this);
    connect(m_resetTachie,SIGNAL(triggered()),this,SLOT(on_resetTachie()));
    //托盘创建
    m_menu = new QMenu(this);
    m_menu->addAction(m_showMainAction); //新增菜单项--打开主界面
    m_menu->addAction(m_openGithub); //新增菜单项--打开github
    m_menu->addSeparator(); //增加分隔符
    m_menu->addAction(m_resetTachie); //新增菜单项---重置立绘
    m_menu->addAction(m_restartAppAction); //新增菜单项---重启
    m_menu->addAction(m_exitAppAction); //新增菜单项---退出程序
    m_sysTrayIcon->setContextMenu(m_menu);
    m_sysTrayIcon->show(); //在系统托盘显示此对象
    /*窗口初始化*/
    qInfo()<<"初始化窗口……";
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
        qInfo()<<"运行autoOpen.cmd……";
        QDesktopServices::openUrl(QUrl::fromLocalFile("autoOpen.cmd"));
    }
    /*获取能量定时器*/
    energyTimer = new QTimer(this);
    connect(energyTimer, &QTimer::timeout, this, &MainWindow::updateEnergyDisplay);
    energyTimer->start(100); //每100毫秒触发一次
    /*新版本获取*/
    qInfo()<<"获取新版本消息……";
    m_manager = new QNetworkAccessManager(this); //新建QNetworkAccessManager对象
    already_init = true;
    QString reply = getUrl("https://api.github.com/repos/Zao-chen/ZcChat/releases/latest");
    QString tagName;
    try
    {
        auto jsonData = json_t::parse(reply.toUtf8());
        if (jsonData.contains("tag_name")) tagName = QString::fromStdString(jsonData["tag_name"]);
    }
    catch (const std::exception& e)
    {
        qWarning() << "版本获取失败，json解析错误: " << e.what();
        tagName.clear(); // 如果解析失败，设置为空
    }
    if (local_version.contains("beta"))
    {
        qInfo()<<"正在运行beta版"+local_version+"，最新正式版"+tagName;
        setUserInfoCardSubTitle("你正在运行beta版，遇到问题请提issue。最新正式版："+tagName);
    }
    else if (reply == "error" || tagName.isEmpty())
    {
        qWarning()<<"获取新版本失败："<<reply;
        setUserInfoCardSubTitle("获取新版本失败");
    }
    else if (local_version != tagName)
    {
        qInfo()<<"发现新版本"+tagName;
        setUserInfoCardSubTitle("发现新版本" + tagName);
    }
    else
    {
        qInfo()<<"当前为最新版本"+local_version;
        setUserInfoCardSubTitle("当前为最新版本^_^");
    }
    ChangeSetting_Lan(settings->value("/soft/lan").toString());
    qInfo()<<"MainWindow加载完成！";
}

MainWindow::~MainWindow()
{
    qInfo()<<"MainWindow关闭……";
    delete ui;
}

/*对话框显示隐藏*/
void MainWindow::show_dialogwin_from_tachie()
{
    if(isDialogOpen)
    {
        dialog_win->handleWheelDown();
        qInfo()<<"隐藏对话框……";
        //设置窗口透明度效果
        QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(dialog_win);
        dialog_win->setGraphicsEffect(opacityEffect);
        //创建淡出动画
        QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, "opacity");
        animation->setDuration(150); //动画持续时间（毫秒）
        animation->setStartValue(1.0); //开始时完全不透明
        animation->setEndValue(0.0); //结束时完全透明
        //动画结束后关闭窗口
        connect(animation, &QPropertyAnimation::finished, dialog_win, &QWidget::close);
        //启动动画
        animation->start();
        isDialogOpen = false;
    }
    else
    {
        qInfo()<<"显示对话框……";
        dialog_win->show();
        //设置窗口透明度效果
        QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(dialog_win);
        dialog_win->setGraphicsEffect(opacityEffect); //确保设置到目标窗口
        //创建动画，设置属性为 opacity
        QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, "opacity");
        animation->setDuration(150); //动画持续时间（毫秒）
        animation->setStartValue(0.0); //起始透明度（0.0 表示完全透明）
        animation->setEndValue(1.0); //结束透明度（1.0 表示完全不透明）
        animation->start();
        isDialogOpen = true;
    }
}
/*重载角色配置*/
void MainWindow::reloadActorSetting()
{
    qInfo()<<"重载角色配置……";
    QSettings *settings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/Setting.ini", QSettings::IniFormat);
    QSettings *settings_actor = new QSettings(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/characters/" + settings->value("actor/name").toString() + "/config.ini", QSettings::IniFormat);
    QString imagePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/characters/" + settings->value("actor/name").toString() + "/正常.png";
    QPixmap originalPixmap(imagePath);
    if (originalPixmap.isNull())
    {
        qWarning("无法加载图片: %s", qPrintable(imagePath));
    }
    else
    {
        int x = (originalPixmap.width() - 128) / 2;
        int y;
        x = qMax(0, x);
        y = 0;
        QPixmap croppedPixmap = originalPixmap.copy(x, y, 128, 128);
        setUserInfoCardPixmap(croppedPixmap);
        setUserInfoCardPixmap(croppedPixmap);
        setUserInfoCardPixmap(croppedPixmap);
    }
    setting_actor_win->findChild<QLabel*>("label_editActor")->setText("当前配置角色："+settings->value("actor/name").toString());
    setting_actor_win->findChild<QSpinBox*>("spinBox_tachie_size")->setValue(settings_actor->value("/tachie/size").toInt());
    setting_actor_win->findChild<QLineEdit*>("lineEdit_llm_agent")->setText(settings_actor->value("/llm/agent").toString());
    setting_actor_win->findChild<QComboBox*>("comboBox_vits_api")->setCurrentIndex(settings_actor->value("/vits/api").toInt());
    setting_actor_win->findChild<QComboBox*>("comboBox_vits_model")->setCurrentText(settings_actor->value("/vits/vitsmodel").toString());
    setting_actor_win->findChild<QLineEdit*>("lineEdit_vits_customUrl")->setText(settings_actor->value("/vits/custom_url").toString());
    setting_actor_win->findChild<QComboBox*>("comboBox_vits_language")->setCurrentText(settings_actor->value("/vits/lan").toString());
    setting_actor_win->findChild<QLineEdit*>("lineEdit_vits_id")->setText(settings_actor->value("/vits/id").toString());
    setting_actor_win->findChild<QLineEdit*>("lineEdit_speechInput_wakeWord")->setText(settings_actor->value("/speechInput/wake_word").toString());
    setting_actor_win->findChild<QLineEdit*>("lineEdit_speechInput_endWord")->setText(settings_actor->value("/speechInput/end_word").toString());
}
/*get请求（用于获取版本）*/
QByteArray MainWindow::getUrl(const QString &input)
{
    QEventLoop loop;
    QNetworkReply *reply = m_manager->get(QNetworkRequest(QUrl(input)));
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if (reply->error() == QNetworkReply::NoError) return reply->readAll();
    else return "error";
}
/*保存立绘位置*/
void MainWindow::changeTachieLocation_from_tachie(int x,int y)
{
    QSettings *settings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/Setting.ini", QSettings::IniFormat);
    settings->setValue("/tachie/location_x",x);
    settings->setValue("/tachie/location_y",y);
}
/*保存配置函数*/
void MainWindow::saveSetting(const QString &key, const QVariant &value) {
    static QScopedPointer<QSettings> settings(new QSettings(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/Setting.ini", QSettings::IniFormat)); // 使用 QScopedPointer 来自动管理资源。
    settings->setValue(key, value);
}
void MainWindow::saveActorSetting(const QString &key, const QVariant &value) {
    QScopedPointer<QSettings> settings(new QSettings(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/Setting.ini", QSettings::IniFormat)); // 使用 QScopedPointer 来自动管理资源。
    QSettings *settings_actor = new QSettings(qApp->applicationDirPath()+"/characters/"+settings->value("actor/name").toString()+"/config.ini",QSettings::IniFormat);
    settings_actor->setValue(key, value);
}
/*配置项修改和保存*/
//立绘大小修改
void MainWindow::ChangeSetting_tachieSize(int arg1)
{
    saveActorSetting("/tachie/size",arg1);
    emit init_to_tachie();
}
//角色选择修改
void MainWindow::ChangeSetting_ActorChoose(const QString &arg1)
{
    if(already_init) saveSetting("/actor/name",arg1);
    reloadActorSetting();
    emit init_to_tachie();
}
//语言修改
void MainWindow::ChangeSetting_Lan(const QString &arg1)
{
    static QScopedPointer<QSettings> settings(new QSettings(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/Setting.ini", QSettings::IniFormat)); // 使用 QScopedPointer 来自动管理资源。
    QTranslator translator;
    qInfo()<<"切换语言到"<<arg1;
    if (translator.load(":/translations/translations/" + arg1 + ".qm"))
    {
        qInfo() << "切换成功" << arg1;
        qApp->installTranslator(&translator);
        settings->setValue("/soft/lan",arg1);
    }
    else
    {
        qWarning() << "加载翻译文件失败：" << arg1;
    }
    ui->retranslateUi(this);
    setting_actor_win->refreshUI();
    setting_ai_win->refreshUI();
    setting_general_win->refreshUI();
    setting_vits_win->refreshUI();
    setting_voiceinput_win->refreshUI();
    dialog_win->refreshUI();
}
//vits模型修改
void MainWindow::ChangeSetting_VitsModel(const QString &arg1)
{
    if(already_init) saveActorSetting("/vits/vitsmodel",arg1);
}
//开机自启修改
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
//vitsAPI修改
void MainWindow::ChangeSetting_VitsAPI(int index)
{
    if(already_init) saveActorSetting("/vits/api",index);
}
//vits语言修改
void MainWindow::ChangeSetting_VitsLanguage(const QString &arg1)
{
    if(already_init) saveActorSetting("/vits/lan",arg1);
}
//dialog大小设置
void MainWindow::ChangeSetting_dialogSize(int arg1)
{
    saveSetting("/dialog/size",arg1);
    emit init_to_dialog();
}
//语言输入API修改
void MainWindow::ChangeSetting_speechInputAPI(int index)
{
    if(already_init) saveSetting("/speechInput/api",index);
}
//语言唤醒修改
void MainWindow::ChangSetting_speechInputWake(bool checked)
{
    emit init_to_dialog();
}
/*托盘*/
//托盘主界面
void MainWindow::on_showMainAction()
{
    this->show();
}
//托盘退出
void MainWindow::on_exitAppAction()
{
    qApp->exit();
}
//托盘重启
void MainWindow::on_restartAppAction()
{
    QString program = QCoreApplication::applicationFilePath(); //获取当前程序的路径
    QStringList arguments = QCoreApplication::arguments(); //获取当前程序的启动参数
    QProcess::startDetached(program, arguments); //启动新进程
    QCoreApplication::quit(); //退出当前程序
}
//托盘ithub
void MainWindow::on_openGithub()
{
    QDesktopServices::openUrl(QUrl("https://github.com/Zao-chen/ZcChat"));
}
//托盘重置立绘
void MainWindow::on_resetTachie()
{
    emit resetlocation_to_tachie();
}
/*窗口隐藏重写*/
void MainWindow::hideWindow()
{
    this->hide();
}
/*能量显示*/
void MainWindow::getEnergy_from_gal(int energy)
{
    currentEnergy = energy; // 更新当前能量值
}
void MainWindow::updateEnergyDisplay() {
    setting_voiceinput_win->findChild<QLCDNumber*>()->display(currentEnergy);
}
