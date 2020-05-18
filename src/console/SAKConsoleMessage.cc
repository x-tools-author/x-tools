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
#include <QDateTime>
#include "SAKConsoleMessage.hh"

SAKConsoleMessage::SAKConsoleMessage(QtMsgType type, QString color, QString function, QString msg, QObject *parent)
    :QObject(parent)
    ,_color(color)
    ,_msg(msg)
{
    QString dateTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss ");

    QString typeString;
    switch (type) {
    case QtDebugMsg:
        typeString = QString("[D] ");
        break;
    case QtInfoMsg:
        typeString = QString("[I] ");
        break;
    case QtWarningMsg:
        typeString = QString("[W] ");
        break;
    case QtCriticalMsg:
        typeString = QString("[C] ");
        break;
    case QtFatalMsg:
        typeString = QString("[F] ");
        break;
    }

    _msg = (typeString + dateTime + function + _msg);
}

SAKConsoleMessage::~SAKConsoleMessage()
{

}

QString SAKConsoleMessage::color()
{
    return _color;
}

QString SAKConsoleMessage::msg()
{
    return _msg;
}
