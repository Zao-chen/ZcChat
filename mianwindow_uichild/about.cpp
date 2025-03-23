#include "about.h"
#include "ui_about.h"

#include <Qboxlayout>
#include <QStandardPaths>
#include <QNetworkReply>
#include <QStandardItemModel>
#include <QFont>

#include "../third_party/json/json.hpp"
#include "../mainwindow.h"

about::about(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::about)
{
    ui->setupUi(this);

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

    /*更新记录获取*/
    bool yourversion = false;
    ui->tableView->setSelectionMode(QAbstractItemView::NoSelection);  // 禁止选择
    try
    {
        m_manager = new QNetworkAccessManager(this);  // 新建QNetworkAccessManager对象
        QString result = getUrl("https://api.github.com/repos/Zao-chen/ZcChat/releases");
        // 检查是否获取到有效数据
        if (result.isEmpty())
        {
            qWarning()<<"Failed to get data from the URL.";
        }
        nlohmann::json releases = nlohmann::json::parse(result.toUtf8().constData());
        // 创建模型
        QStandardItemModel* model = new QStandardItemModel;
        // 遍历所有的 release 数据
        for (const auto& release : releases) {
            if (release.contains("name") && release.contains("published_at")) {
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
    }
    catch (const std::exception& e)
    {
        ui->label_msg->setText("获取更新失败");
        qWarning() << "发生错误:" << e.what();  // 打印错误信息
    }
    catch (...)
    {
        ui->label_msg->setText("未知错误");
        qWarning() << "发生未知错误";
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
