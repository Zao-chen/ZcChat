#include "about.h"
#include "ui_about.h"

#include <Qboxlayout>
#include <QStandardPaths>
#include <QNetworkReply>
#include <QStandardItemModel>
#include <QFont>
#include <QFile>
#include <QProcess>

#include "../third_party/json/json.hpp"
#include "../mainwindow.h"

about::about(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::about)
{
    ui->setupUi(this);
    mainWin = qobject_cast<MainWindow *>(this->parent());
    /*按钮设置*/
    ui->pushButton->setUrl("https://github.com/Zao-chen/ZcChat");
    ui->pushButton->setCardPixmap(QPixmap(":/img/img/github-mark.svg"));
    ui->pushButton->setTitle("Github");
    ui->pushButton->setSubTitle("关于ZcChat的一切都在这儿，下载/配置/帮助/反馈/资源/文档");
    ui->pushButton_2->setUrl("https://github.com/Zao-chen/ZcChat/issues/new/choose");
    ui->pushButton_2->setCardPixmap(QPixmap(":/img/img/circle-dot-regular.svg"));
    ui->pushButton_2->setTitle("Issue");
    ui->pushButton_2->setSubTitle("需要帮助/Bug反馈/功能建议？来提一个issue吧！");
    ui->pushButton_3->setUrl(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/log.txt");
    ui->pushButton_3->setCardPixmap(QPixmap(":/img/img/file-solid.svg"));
    ui->pushButton_3->setTitle("软件日志");
    ui->pushButton_3->setSubTitle("ZcChat的运行日志，用于排除问题，提Issue的时候别忘了带上它");
    ui->pushButton_4->setUrl(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ZcChat/characters");
    ui->pushButton_4->setCardPixmap(QPixmap(":/img/img/snowman-solid.svg"));
    ui->pushButton_4->setTitle("角色文件夹");
    ui->pushButton_4->setSubTitle("点击打开角色文件的存放位置");
    //界面设置
    ui->progressBar->setVisible(false);
    /*更新记录获取*/
    bool yourversion = false;
    m_manager = new QNetworkAccessManager(this);  // 新建QNetworkAccessManager对象
    ui->tableView->setSelectionMode(QAbstractItemView::NoSelection);  // 禁止选择
    try
    {
        result = getUrl("https://api.github.com/repos/Zao-chen/ZcChat/releases");
        nlohmann::json releases = nlohmann::json::parse(result.toUtf8().constData());
        // 创建模型
        QStandardItemModel* model = new QStandardItemModel;
        // 遍历所有的 release 数据
        for (const auto& release : releases)
        {
            if (release.contains("name") && release.contains("published_at"))
            {
                QString title = QString::fromStdString(release["name"].get<std::string>());
                mainWin = qobject_cast<MainWindow *>(this->parent());
                // 判断是否是目标版本
                if (title.split(" ")[0] == mainWin->local_version) yourversion = true;  // 找到版本后，设置为 true
                QString dateTime = QString::fromStdString(release["published_at"].get<std::string>());
                QString date = dateTime.left(10);  // 提取日期部分
                // 将数据添加到模型中
                QList<QStandardItem*> rowItems;
                if (!yourversion) rowItems << new QStandardItem("□") << new QStandardItem(title.split(" ")[0]) << new QStandardItem(date) << new QStandardItem(title.split(" ")[1]);
                else rowItems << new QStandardItem("■")<< new QStandardItem(title.split(" ")[0]) << new QStandardItem(date) << new QStandardItem(title.split(" ")[1]);
                model->appendRow(rowItems);  // 每行一个 QStandardItem 列表
            }
        }
        // 设置 QLabel 显示内容
        ui->tableView->setModel(model);
        // 设置列宽自适应
        ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
        ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
        ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
        //是否最新版本检查
        if(result == "error" || mainWin->tagName.isEmpty())
        {
            ui->pushButton_upDate->setText(tr("获取新版本失败"));
        }
        else if(mainWin->local_version != mainWin->tagName)
        {
            ui->pushButton_upDate->setText(tr("发现新版本") + mainWin->tagName);
            ui->pushButton_upDate->setEnabled(true);
        }
        else
        {
            ui->pushButton_upDate->setText(tr("当前为最新正式版"));
        }
    }
    catch (const std::exception& e)
    {
        ui->label_msg->setText("获取更新失败");
        qWarning() << "发生错误:" << e.what();  // 打印错误信息
    }
}
about::~about()
{
    delete ui;
}
/*get请求（用于获取版本）*/
QByteArray about::getUrl(const QString &input)
{
    QEventLoop loop;
    QNetworkReply *reply = m_manager->get(QNetworkRequest(QUrl(input)));
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if (reply->error() == QNetworkReply::NoError) return reply->readAll();
    else return "error";
}
//更新版本
void about::on_pushButton_upDate_clicked()
{
    ui->pushButton_upDate->setEnabled(false);
    ui->pushButton_upDate->setText(tr("下载中"));
    QString result = mainWin->reply;
    if(result.isEmpty() || result == "error") return;
    // 使用 nlohmann::json 解析 JSON 数据
    nlohmann::json jsonData = nlohmann::json::parse(result.toStdString());
    // 创建 Qt 网络管理器
    QNetworkAccessManager manager;
    // 查找 exe 文件并下载
    for (const auto& asset : jsonData["assets"])
    {
        std::string fileName = asset["name"];
        if (fileName.find(".exe") != std::string::npos)
        {
            // 找到 .exe 文件，获取下载链接
            std::string downloadUrl = asset["browser_download_url"];
            qDebug() << "Found .exe file, downloading from:" << QString::fromStdString(downloadUrl);
            // 下载文件
            QUrl fileUrl(QString::fromStdString(downloadUrl));
            QNetworkRequest request(fileUrl);
            QNetworkReply *reply = manager.get(request);
            // 创建一个事件循环来等待下载完成
            QEventLoop loop;
            // 连接下载进度信号
            ui->progressBar->setVisible(true);
            QObject::connect(reply, &QNetworkReply::downloadProgress, [=](qint64 bytesReceived, qint64 bytesTotal)
            {
                if (bytesTotal > 0)
                {
                    qDebug() << "Download progress:" << (bytesReceived * 100) / bytesTotal << "%";
                    ui->progressBar->setValue((bytesReceived * 100) / bytesTotal);
                }
            });
            // 连接下载完成信号
            QObject::connect(reply, &QNetworkReply::finished, [&]()
            {
                if (reply->error() != QNetworkReply::NoError)
                {
                    qWarning() << "Download failed:" << reply->errorString();
                    reply->deleteLater();
                    loop.quit();  // 退出事件循环
                    ui->progressBar->setVisible(false);
                    ui->pushButton_upDate->setEnabled(true);
                    ui->pushButton_upDate->setText(tr("下载失败"));
                    return;
                }
                // 获取系统的下载文件夹路径
                QString downloadFolder = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
                QString saveFilePath = downloadFolder + "/" + QString::fromStdString(fileName);  // 保存文件的完整路径
                // 下载完成，保存文件
                QByteArray fileData = reply->readAll();
                QFile file(saveFilePath);  // 使用下载文件夹路径
                if (file.open(QIODevice::WriteOnly)) {
                    file.write(fileData);
                    file.close();
                    qDebug() << "Download complete, starting .exe...";
                    // 启动下载的 exe 文件
                    QProcess::startDetached(saveFilePath);  // 启动下载的文件
                    ui->pushButton_upDate->setEnabled(true);
                    ui->pushButton_upDate->setText(tr("下载成功，正在打开"));
                }
                else
                {
                    ui->pushButton_upDate->setEnabled(true);
                    ui->pushButton_upDate->setText(tr("安装包打开失败，请前往“下载”文件夹手动安装"));
                }
                reply->deleteLater();
                loop.quit();  // 退出事件循环
            });
            loop.exec();  // 启动事件循环，直到下载完成
            ui->progressBar->setVisible(false);
            return;  // 找到并下载文件后退出
        }
    }
}
