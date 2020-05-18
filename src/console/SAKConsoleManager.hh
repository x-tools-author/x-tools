/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKCONSOLEMANAGER_HH
#define SAKCONSOLEMANAGER_HH

#include <QMutex>
#include <QObject>
#include <QtGlobal>
#include <QVariantList>

/// @brief 终端输出管理类
class SAKConsoleManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList messagesList READ messagesList NOTIFY messagesListChanged)
private:
    SAKConsoleManager(QObject *parent = Q_NULLPTR);
    ~SAKConsoleManager();
public:
    /**
     * @brief instance 获取实例指针
     * @return 实力指针
     */
    static SAKConsoleManager* instance();

    /**
     * @brief consoleOutput 终端输出
     * @param type 消息类型
     * @param context 消息上下文
     * @param msg 消息文本
     */
    static void consoleOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
private:
    static SAKConsoleManager* instancePtr;
    QMutex messagesListMutex;
private:
    QVariantList _messagesList;
    QVariantList messagesList();
signals:
    void messagesListChanged();
};

#endif
