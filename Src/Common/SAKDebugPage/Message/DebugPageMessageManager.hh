/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#ifndef DEBUGPAGEMESSAGEMANAGER_HH
#define DEBUGPAGEMESSAGEMANAGER_HH

#include <QObject>
#include <QTextBrowser>

class SAKDebugPage;

class DebugPageMessageManager:public QObject
{
    Q_OBJECT
public:
    DebugPageMessageManager(SAKDebugPage *debugPage, QObject *parent = nullptr);

    /**
     * @brief outputMessage 输出消息（提示信息）
     * @param msg 消息
     * @param isInfoMsg 如果该值为false，输出消息将高亮显示（红色显示）
     */
    void outputMessage(QString msg, bool isInfoMsg = true);
private:
    SAKDebugPage *debugPage;
    QTextBrowser *messageTextBrowser;
};

#endif
