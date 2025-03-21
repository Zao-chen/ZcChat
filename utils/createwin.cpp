#include "createwin.h"
#include "../dialog/messagewindow.h"


// 全局函数：创建并显示自定义 message 窗口
void createwin(const QString &message)
{
    messagewindow msgWin(message);
    msgWin.exec();
}
