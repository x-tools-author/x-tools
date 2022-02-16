/******************************************************************************
 * 版权声明 2022 小猎隼(qsaker@outlook.com)。保留所有版权。
 *
 * 本文件使用“utf8 with bom”格式编码, 它是“小猎隼多功能调试工具软件”项目的一部分（项目曾
 * 用名：瑞士军刀）。详情可查看
 * 连接：
 * https://gitee.com/qsaker/QtSwissArmyKnife.git
 *
 * “小猎隼多功能调试软件”项目授权协议请查看根目录的LICENCE文件。
 *****************************************************************************/
#ifndef SAKSYSTEMTRAYICON_HH
#define SAKSYSTEMTRAYICON_HH

#include <QSystemTrayIcon>

class SAKSystemTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit SAKSystemTrayIcon(QObject *parent = Q_NULLPTR);
    ~SAKSystemTrayIcon();
signals:
    void invokeExit();
    void invokeShowMainWindow();
};

#endif // SAKSYSTEMTRAYICON_HH
