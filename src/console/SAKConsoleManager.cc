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
#include <QCoreApplication>

#include "SAKConsoleMessage.hh"
#include "SAKConsoleManager.hh"

SAKConsoleManager* SAKConsoleManager::instancePtr = Q_NULLPTR;
SAKConsoleManager::SAKConsoleManager(QObject *parent)
    :QObject(parent)
{
    instancePtr = this;
}

SAKConsoleManager::~SAKConsoleManager()
{
    instancePtr = Q_NULLPTR;
}

SAKConsoleManager* SAKConsoleManager::instance()
{
    if (!instancePtr){
        new SAKConsoleManager;
    }

    Q_ASSERT_X(instancePtr, __FUNCTION__, "Oh, a null pointer!");
    return instancePtr;
}

void SAKConsoleManager::consoleOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString function = context.function ? QString(context.function) : QString("Unknow");
    QString color;
    switch (type) {
    case QtDebugMsg:
    case QtInfoMsg:
        color = QString("#FFFFFFFF");
        break;
    case QtWarningMsg:
    case QtCriticalMsg:
    case QtFatalMsg:
        color = QString("#FFFF0000");
        break;
    }

    instancePtr->messagesListMutex.lock();
    if (instancePtr){
       SAKConsoleMessage *msgPtr = new SAKConsoleMessage(type, color, function, msg, instancePtr);
       instancePtr->_messagesList.prepend(QVariant::fromValue(msgPtr));

       while (instancePtr->_messagesList.length() > 1000) {
           instancePtr->_messagesList.removeLast();
       }

       emit instancePtr->messagesListChanged();
    }
    instancePtr->messagesListMutex.unlock();
}

QVariantList SAKConsoleManager::messagesList()
{
    return _messagesList;
}
