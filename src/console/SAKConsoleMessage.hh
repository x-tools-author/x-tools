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
#ifndef SAKCONSOLEMESSAGE_HH
#define SAKCONSOLEMESSAGE_HH

#include <QMutex>
#include <QObject>
#include <QtGlobal>

/// @brief 消息类
class SAKConsoleMessage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString color READ color CONSTANT)
    Q_PROPERTY(QString msg READ msg CONSTANT)
public:
    SAKConsoleMessage(QtMsgType type, QString color, QString function, QString msg, QObject *parent = Q_NULLPTR);
    ~SAKConsoleMessage();
private:
    QString _color;
    QString color();

    QString _msg;
    QString msg();
};

#endif
